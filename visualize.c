/**
* Spotify Artist Visualization
* @author Gregory Ecklund
* @version November 10, 2025
* This file contains the function definitions/implementations
* necessary for the visualization process
* @e : exit (1) - Invalid input file
*/

//Include necessary header file
#include "./visualize.h"

int get_artist_index(struct Globals* global_vars, char* name) {
    for (int i = 0; i < global_vars->total_artist_count; i++) {
        //TODO: Compare Names
        if (strcmp(global_vars->all_artists[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}

struct Artist* create_artist(struct Globals* global_vars, char* name) {
    //Creates an artist and assigns values
    struct Artist* new_artist = (struct Artist*) malloc(sizeof(struct Artist));
    new_artist->name = name;
    new_artist->id = global_vars->total_artist_count;
    new_artist->links = (struct Link**) calloc(MAX_LINK_PER_ARTIST, (sizeof(struct Link*)));
    new_artist->link_count = 0;
    
    //Update global variables
    global_vars->all_artists[global_vars->total_artist_count] = new_artist;
    global_vars->total_artist_count++;

    //Return a pointer to the newly created artist
    return new_artist;
}

struct Song* create_song(struct Globals* global_vars, char* name, int* artist_ids, int num_of_artists) {
    //Creates the song and sets attributes
    struct Song* new_song = (struct Song*) malloc(sizeof(struct Song));
    new_song->song_name = name;
    new_song->artist_count = num_of_artists;
    new_song->artists = (struct Artist**) calloc(num_of_artists, sizeof(struct Artist*));
    for (int i = 0; i < num_of_artists; i++) {
        new_song->artists[i] = global_vars->all_artists[artist_ids[i]];
    }

    //Update global variables
    global_vars->all_songs[global_vars->total_song_count] = new_song;
    global_vars->total_song_count++;

    //Returns a pointer to the newly created song
    return new_song;
}

struct Link* create_link(struct Globals* global_vars, struct Artist* first_artist, struct Artist* second_artist, struct Song* song) {
    //Creates the link and sets attributes
    struct Link* new_link = (struct Link*) malloc(sizeof(struct Link));
    new_link->artist_one = first_artist;
    new_link->artist_two = second_artist;
    new_link->song = song;

    //Update global variables
    global_vars->all_links[global_vars->total_link_count] = new_link;
    global_vars->total_link_count++;

    //Returns a pointer to the newly created link
    return new_link;

}

void print_artist(struct Artist* artist) {
    printf("Artist Name: '%s' {\n", artist->name);
    printf("\tid: %d\n", artist->id);
    printf("\tlink count: %d\n", artist->link_count);
    printf("}\n");
}

void print_song(struct Song* song) {
    printf("Song Name: '%s' {\n", song->song_name);
    for (int i = 0; i < song->artist_count; i++) {
        printf("\tArtist: '%s'\n", song->artists[i]->name);
    }
    printf("}\n");
}

void save_to_file(char* output_filename, struct Globals* global_vars) {
    //File manip
    FILE* output_file = fopen(output_filename, "w");
    fprintf(output_file, "graph ArtistTree{\n");

    //Nodes
    for (int i = 0; i < global_vars->total_artist_count; i++) {
        struct Artist* current_artist = global_vars->all_artists[i];
        fprintf(output_file, "\t%d[label=\"%s\"];\n", current_artist->id, current_artist->name);
    }
    //Edges
    for (int i = 0; i < global_vars->total_link_count; i++) {
        struct Link* current_link = global_vars->all_links[i];
        int artist_one_id = current_link->artist_one->id;
        int artist_two_id = current_link->artist_two->id;
        fprintf(output_file, "\t%d -- %d [label=\"%s\"];\n",
            artist_one_id, artist_two_id, current_link->song->song_name);
    }
    fprintf(output_file, "}");
    //Close file to avoid leaks
    fclose(output_file);
}

void free_all_memory(struct Globals* global_vars) {
    //Free all memory
    for (int i = 0; i < global_vars->total_link_count; i++) {
        free(global_vars->all_links[i]);
    }
    for (int i = 0; i < global_vars->total_song_count; i++) {
        free(global_vars->all_songs[i]->artists);
        free(global_vars->all_songs[i]->song_name);
        free(global_vars->all_songs[i]);
    }
    for (int i = 0; i < global_vars->total_artist_count; i++) {
        free(global_vars->all_artists[i]->links);
        free(global_vars->all_artists[i]->name);
        free(global_vars->all_artists[i]);
    }
}

void read_from_file(char* input_filename, struct Globals* global_vars) {
    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        free_all_memory(global_vars);
        fprintf(stderr, "ERROR: could not open %s for reading\n.", input_filename);
        exit(1);
    }

    char input_line[MAX_LINE_LENGTH]; //A temporary variable containing the current line from the input file
    char temp_songname[MAX_LINE_LENGTH]; //The song from the current line
    
    //Iterates over the lines in the file
    while(fgets(input_line, MAX_LINE_LENGTH, input_file) != NULL) {
        int current_character_index = 0; //The index of the current character in the line
        char current_character = input_line[current_character_index]; //The current character in the line
        while (current_character != '\\') {
            temp_songname[current_character_index] = current_character;
            current_character_index++;
            current_character=input_line[current_character_index];
        }
        //current_character_index now has the index of the backslash
        temp_songname[current_character_index] = '\0';
        char* song_name = (char*) malloc(MAX_SONG_LENGTH * sizeof(char));
        strcpy(song_name, temp_songname);
        
        //Get artists
        int end_of_line = FALSE; //If the pointer gets to the end of the line
        int number_of_artists_in_song = 0;
        int artist_indexes[MAX_ARTIST_PER_SONG];
        for (int i = 0; i < MAX_ARTIST_PER_SONG; i++) {
            if (end_of_line == FALSE) {
                current_character_index++; //Index should now point to first letter of artist name
                current_character = input_line[current_character_index]; //The current character in the line
                int length_of_artist_name = 0; //The amount of characters in the artist's name.
                char* first_character_in_name = &input_line[current_character_index]; //A pointer to the first letter in the Artist's name
                char* artist_name = (char*) malloc(MAX_ARTIST_LENGTH * sizeof(char)); //An array to contain the name of the artist
                
                while (current_character != ',' && current_character != '\n' && current_character != '\0') {
                    length_of_artist_name++;
                    current_character_index++;
                    current_character = input_line[current_character_index];
                }
                if (current_character == '\n') {
                    end_of_line = TRUE;
                }
                
                input_line[current_character_index] = '\0';
                length_of_artist_name++; //Adds the nullcharacter to the length
                strncpy(artist_name, first_character_in_name, length_of_artist_name);
                number_of_artists_in_song++;
                
                //Checks to see if the artist already exists.
                int current_artist_index = get_artist_index(global_vars, artist_name);
                if (current_artist_index == -1) {
                    create_artist(global_vars, artist_name);
                    current_artist_index = global_vars->total_artist_count - 1;
                }
                else {
                    //Free the memory if it already exists
                    free(artist_name);
                }
                artist_indexes[(number_of_artists_in_song - 1)] = current_artist_index;
            }
        }

        create_song(global_vars, song_name, artist_indexes, number_of_artists_in_song);

    }

    fclose(input_file);
}

void create_all_links(struct Globals* global_vars) {
    for (int song_index = 0; song_index < global_vars->total_song_count; song_index++) {
        struct Song* current_song = global_vars->all_songs[song_index];
        for (int i = 0; i < current_song->artist_count; i++) {
            struct Artist* artist_one = current_song->artists[i];
            for (int j = (i + 1); j < current_song->artist_count; j++) {
                struct Artist* artist_two = current_song->artists[j];
                create_link(global_vars, artist_one, artist_two, current_song);
            }
        }
    }
}

int main(void) {
    //Set global variables
    struct Globals global_variables;
    global_variables.all_artists = (struct Artist**) calloc(MAX_ARTIST_COUNT, sizeof(struct Artist*));
    global_variables.all_links = (struct Link**) calloc(MAX_LINK_COUNT, sizeof(struct Link*));
    global_variables.all_songs = (struct Song**) calloc(MAX_SONG_COUNT, sizeof(struct Song*));
    global_variables.total_artist_count = 0;
    global_variables.total_song_count = 0;
    global_variables.total_link_count = 0;
    
    //Reads data from input file
    read_from_file("songs.txt", &global_variables);

    //Creates the links
    create_all_links(&global_variables);

    //Saves data to .dot file
    save_to_file("ArtistTree.dot", &global_variables);

    free_all_memory(&global_variables);


    printf("Visualization Complete!\n");
}