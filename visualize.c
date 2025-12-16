/**
* Spotify Artist Visualization
* @author Gregory Ecklund
* @version December 16, 2025
* This file contains the function definitions/implementations
* necessary for the visualization process
* @e : exit (1) - Invalid input file
*/

//Include necessary header file
#include "./visualize.h"
#include <string.h>

//HELPER FUNCTIONS

/**
* Gets the index for a requested artist in the list of all artists
* @param global_vars : The global variables for the program
* @param name : The name of the artist to get the index of
* @return: Returns the index of the artist in global_vars->all_artists
*/
int get_artist_index(struct Globals* global_vars, char* name) {
    for (int i = 0; i < global_vars->total_artist_count; i++) {
        //TODO: Compare Names
        if (strcmp(global_vars->all_artists[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
* Checks to see if an integer is already in an array of integers
* Assures uniqueness upon items in the set
* @param int* set : The set to check
* @param int set_size : The current number of elements in the set
* @param int new_num : The number to check
* @return: Returns TRUE(1) if the set does not contain the number,
* otherwise returns FALSE(0) if the number is already in the set
*/
int check_for_uniqueness(int* set, int set_size, int new_num) {
    for (int i = 0; i < set_size; i++) {
        if (set[i] == new_num) {
            return FALSE;
        }
    }
    return TRUE;
}

/**
* Attempts to find a link within the global variables that contains the two artists
* @param artist_one : One of the artists in the desired link
* @param artist_two : The other artist in the desired link
* @return: Returns a pointer to the link if a link can be found containing the two artists,
* otherwise returns NULL
*/
struct Link* find_link(struct Globals* global_vars, struct Artist* artist_one, struct Artist* artist_two) {
    for (int i = 0; i < global_vars->total_link_count; i++) {
        struct Link* current_link = global_vars->all_links[i];
        if (current_link->artist_one == artist_one && current_link->artist_two == artist_two) {
            return current_link;
        }
        if (current_link->artist_two == artist_one && current_link->artist_one == artist_two) {
            return current_link;
        }
    }
    return NULL;
}

/**
* Checks to see if a link between two artists already exists
* @param global_vars : The global variables for the program
* @param artist_one : One of the artists
* @param artist_two : The other artist
* @return: Returns TRUE(1) if a link between the two artists already exists, FALSE(0) if one does not exist
*/
int link_already_exists(struct Globals* global_vars, struct Artist* artist_one, struct Artist* artist_two) {
    for (int i = 0; i < global_vars->total_link_count; i++) {
        struct Link* current_link = global_vars->all_links[i];
        if (current_link->artist_one == artist_one && current_link->artist_two == artist_two) {
            return TRUE;
        }
        if (current_link->artist_one == artist_two && current_link->artist_two == artist_one) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
* Creates an Artist struct for a given artist
* @param global_vars : The global variables for the program
* @param name : The name of the artist
* @return: Returns a pointer to the created Artist struct
*/
struct Artist* create_artist(struct Globals* global_vars, char* name) {
    //Creates an artist and assigns values
    struct Artist* new_artist = (struct Artist*) malloc(sizeof(struct Artist));
    new_artist->name = name;
    new_artist->id = global_vars->total_artist_count;
    new_artist->fillcolor = DEFAULT_FILLCOLOR;
    new_artist->links = (struct Link**) calloc(MAX_LINK_PER_ARTIST, (sizeof(struct Link*)));
    new_artist->link_count = 0;
    
    //Update global variables
    global_vars->all_artists[global_vars->total_artist_count] = new_artist;
    global_vars->total_artist_count++;

    //Return a pointer to the newly created artist
    return new_artist;
}

/**
* Creates a Song struct for a given song
* @param global_vars : The global variables for the program
* @param name : The name of the song
* @param artist_ids : A list of the ids of the artists in the song
* @param num_of_artists : The number of artists on the song (same as the number of artists in artist_ids)
* @return: Returns a pointer to the created Song struct
*/
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

/**
* Creates a Link struct for a given link between two artists
* @param global_vars : The global variables for the program
* @param first_artist : One of the artists on the link
* @param second_artist : The other artist on the link
* @param song : The song that links the two artists together
* @return: Returns a pointer to the created Link struct
*/
struct Link* create_link(struct Globals* global_vars, struct Artist* first_artist, struct Artist* second_artist, struct Song* song) {
    //Creates the link and sets attributes
    struct Link* new_link = (struct Link*) malloc(sizeof(struct Link));
    new_link->artist_one = first_artist;
    new_link->artist_two = second_artist;
    new_link->color = DEFAULT_LINK_COLOR;
    new_link->penwidth = DEFAULT_PENWIDTH;
    new_link->song = song;
    new_link->songname_color = DEFAULT_SONGNAME_COLOR;
    new_link->songname_fontsize = DEFAULT_SONGNAME_FONTSIZE;

    //Update artist's links
    first_artist->links[first_artist->link_count] = new_link;
    first_artist->link_count++;
    second_artist->links[second_artist->link_count] = new_link;
    second_artist->link_count++;

    //Update global variables
    global_vars->all_links[global_vars->total_link_count] = new_link;
    global_vars->total_link_count++;

    //Returns a pointer to the newly created link
    return new_link;

}

/**
* A helper function to print a given artist to stdout
* @param artist : The artist to print to stdout
*/
void print_artist(struct Artist* artist) {
    printf("Artist Name: '%s' {\n", artist->name);
    printf("\tid: %d\n", artist->id);
    printf("\tfill color: %s\n", artist->fillcolor);
    printf("\tlink count: %d\n", artist->link_count);
    printf("}\n");
}

/**
* A helper function to print a given song to stdout
* @param song : The song to print to stdout
*/
void print_song(struct Song* song) {
    printf("Song Name: '%s' {\n", song->song_name);
    for (int i = 0; i < song->artist_count; i++) {
        printf("\tArtist: '%s'\n", song->artists[i]->name);
    }
    printf("}\n");
}

/**
* A helper function to print a given link to stdout
* @param link : The link to print to stdout
*/
void print_link(struct Link* link) {
    printf("Link: {\n\tSong: '%s'\n", link->song->song_name);
    printf("\tArtist One: '%s'\n", link->artist_one->name);
    printf("\tArtist Two: '%s'\n}\n", link->artist_two->name);
}

/**
* Frees all of the memory used for the visualization process
* @param global_vars : The global variables for the program
*/
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

/**
* A function to create the links to use as edges for the graph
* @param global_vars : The global variables for the program
*/
void create_all_links(struct Globals* global_vars) {
    for (int song_index = 0; song_index < global_vars->total_song_count; song_index++) {
        struct Song* current_song = global_vars->all_songs[song_index];
        for (int i = 0; i < current_song->artist_count; i++) {
            struct Artist* artist_one = current_song->artists[i];
            for (int j = (i + 1); j < current_song->artist_count; j++) {
                struct Artist* artist_two = current_song->artists[j];
                if (!link_already_exists(global_vars, artist_one, artist_two)) {
                    create_link(global_vars, artist_one, artist_two, current_song);
                }
            }
        }
    }
}

//MAIN FUNCTIONS

/**
* Reads the data from a txt file following a specified format and saves the data in global vars
* @param input_filename : The name of the file to read data from
* @param global_vars : The global variables for the program
*/
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

/**
* Employs a Breadth-First Search (BFS) upon the graph to find the shortest path between
* a starting artist and an ending artist
* @param global_vars : The global variables for the program
* @param start_artist_name : The name of the artist to start the search on
* @param end_artist_name_ : The name of the artist to end the search at
* @return: Returns TRUE(1) if a path was found between the start and ending artists,
* otherwise returns FALSE(0) if no path could be made
*/
int breadth_first_search(struct Globals* global_vars, char* start_artist_name, char* end_artist_name) {
    //Ensures the ending artist actually exists
    int start_artist_index = get_artist_index(global_vars, start_artist_name);
    if (start_artist_index == -1) {
        return FALSE;
    }
    struct Artist* start_artist = global_vars->all_artists[start_artist_index];

    //Queue for the artists to check
    struct Artist** queue = (struct Artist**) calloc(MAX_ARTIST_COUNT, sizeof(struct Artist*));
    struct Artist** initial_queue_index = queue;
    queue[0] = start_artist;
    int queue_size = 1;
    //Set containing artist ids that have already been checked
    int* set = calloc(MAX_ARTIST_COUNT, sizeof(int));
    set[0] = start_artist_index;
    int set_size = 1;
    //List containing the parent node for each artist whilst searching
    int* parent = calloc(MAX_ARTIST_COUNT, sizeof(int));
    
    int PATH_FOUND = FALSE;
    //Iterates over queue until a path is found
    while (queue_size > 0 && PATH_FOUND == FALSE) {
        //Pulls from front of queue
        struct Artist* current_artist = queue[0];
        queue++;
        queue_size--;

        //Iterates over edges
        for (int i = 0; i < current_artist->link_count; i++) {
            struct Link* current_link = current_artist->links[i];
            struct Artist* artist_to_check;
            
            //Checks to see which artist isn't the current artist
            if (current_link->artist_one == current_artist) {
                artist_to_check = current_link->artist_two;
            }
            else {
                artist_to_check = current_link->artist_one;
            }

            
            //Makes sure we haven't already found a path before checking
            if (PATH_FOUND == FALSE) {
                if (strcmp(end_artist_name, artist_to_check->name) == 0) {
                    printf("ARTIST FOUND!\n");
                    PATH_FOUND = TRUE;
                }
                //Checks if it hasn't already been added to queue
                if (check_for_uniqueness(set, set_size, artist_to_check->id)) {
                    queue[queue_size] = artist_to_check;
                    queue_size++;
                    //Adds artist id to set
                    set[set_size] = artist_to_check->id;
                    set_size++;
                    //Updates the parent node for the artist we are checking
                    parent[artist_to_check->id] = current_artist->id;
                }
            }
        }
    }

    if (PATH_FOUND) {
        int end_artist_name_index = get_artist_index(global_vars, end_artist_name);
        struct Artist* prev_artist = global_vars->all_artists[end_artist_name_index];
        struct Artist* current_artist = global_vars->all_artists[parent[prev_artist->id]];
        prev_artist->fillcolor = END_FILLCOLOR;

        while (strcmp(start_artist->name, prev_artist->name) != 0) {
            //Find the link
            struct Link* link = find_link(global_vars, prev_artist, current_artist);

            //Change visual attributes on the graph accordingly
            link->color = CHAIN_FILLCOLOR;
            link->penwidth = PATH_PENWIDTH;
            link->songname_color = PATH_SONGNAME_COLOR;
            link->songname_fontsize = PATH_SONGNAME_FONTSIZE;
            current_artist->fillcolor = CHAIN_FILLCOLOR;
            
            //Update Artists
            prev_artist = current_artist;
            current_artist = global_vars->all_artists[parent[current_artist->id]];
        }
        prev_artist->fillcolor = START_FILLCOLOR;
    }

    //Frees the used memory
    free(initial_queue_index);
    free(set);
    free(parent);

    return PATH_FOUND;
}

/**
* Saves the artists and songs as nodes and edges to a .dot file
* @param output_filename : The name of the (.dot) file to save the data to
* @param global_vars : The global variables for the program
*/
void save_to_file(char* output_filename, struct Globals* global_vars) {
    //File manip
    FILE* output_file = fopen(output_filename, "w");
    fprintf(output_file, "graph ArtistTree{\n");

    //Nodes
    for (int i = 0; i < global_vars->total_artist_count; i++) {
        struct Artist* current_artist = global_vars->all_artists[i];
        fprintf(output_file, "\t%d[label=\"%s\" style=filled fillcolor=\"%s\"];\n", current_artist->id, current_artist->name, current_artist->fillcolor);
    }
    //Edges
    for (int i = 0; i < global_vars->total_link_count; i++) {
        struct Link* current_link = global_vars->all_links[i];
        int artist_one_id = current_link->artist_one->id;
        int artist_two_id = current_link->artist_two->id;
        fprintf(output_file, "\t%d -- %d [label=\"%s\" fontcolor=\"%s\" fontsize=%d color=\"%s\" penwidth=%d];\n",
            artist_one_id, artist_two_id, current_link->song->song_name, current_link->songname_color,
            current_link->songname_fontsize, current_link->color, current_link->penwidth);
    }
    fprintf(output_file, "}");
    //Close file to avoid leaks
    fclose(output_file);
}

/**
* The entrypoint to the program.
*/
void run_visualizer(char* input_filename, char* output_filename) {
    //Set global variables
    struct Globals global_variables;
    global_variables.all_artists = (struct Artist**) calloc(MAX_ARTIST_COUNT, sizeof(struct Artist*));
    global_variables.all_links = (struct Link**) calloc(MAX_LINK_COUNT, sizeof(struct Link*));
    global_variables.all_songs = (struct Song**) calloc(MAX_SONG_COUNT, sizeof(struct Song*));
    global_variables.total_artist_count = 0;
    global_variables.total_song_count = 0;
    global_variables.total_link_count = 0;
    
    //Reads data from input file
    read_from_file(input_filename, &global_variables);
    
    printf("DONE READING FROM FILE!\n");

    //Creates the links
    create_all_links(&global_variables);

    //Runs BFS
    breadth_first_search(&global_variables, "Neton Vega", "Isabela Merced");
    
    //Saves data to .dot file
    save_to_file(output_filename, &global_variables);


    free_all_memory(&global_variables);


    printf("Visualization Complete!\n");
}
