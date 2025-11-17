/**
* Spotify Artist Visualization
* @author Gregory Ecklund
* @version November 10, 2025
* This file contains the function definitions/implementations
* necessary for the visualization process
*/

//Include necessary header file
#include "./visualize.h"

int get_artist_index(struct Artist** all_artists, int* total_artist_count, char* name) {
    for (int i = 0; i < (*total_artist_count); i++) {
        //TODO: Compare Names
        if (strcmp(all_artists[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}


struct Artist* create_artist(struct Artist** all_artists, int* total_artist_count, char* name) {
    //Creates an artist and assigns values
    struct Artist* new_artist = (struct Artist*) malloc(sizeof(struct Artist));
    new_artist->name = name;
    new_artist->id = (*total_artist_count);
    new_artist->links = (struct Link**) calloc(MAX_LINK_PER_ARTIST, (sizeof(struct Link*)));
    new_artist->link_count = 0;
    
    //Update global variables
    all_artists[(*total_artist_count)] = new_artist;
    (*total_artist_count) = (*total_artist_count) + 1;

    //Return a pointer to the newly created artist
    return new_artist;
}

struct Song* create_song(struct Artist** all_artists, int* total_song_count, struct Song** all_songs, char* name, int* artist_ids, int num_of_artists) {
    //Creates the song and sets attributes
    struct Song* new_song = (struct Song*) malloc(sizeof(struct Song));
    new_song->song_name = name;
    new_song->artist_count = num_of_artists;
    new_song->artists = (struct Artist**) calloc(num_of_artists, sizeof(struct Artist*));
    for (int i = 0; i < num_of_artists; i++) {
        new_song->artists[i] = all_artists[artist_ids[i]];
    }

    //Update global variables
    all_songs[(*total_song_count)] = new_song;
    (*total_song_count) = (*total_song_count) + 1;

    //Returns a pointer to the newly created song
    return new_song;
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

int main(void) {
    //Define global variables
    struct Artist** all_artists = (struct Artist**) calloc(MAX_ARTIST_COUNT, sizeof(struct Artist*));
    struct Link** all_links = (struct Link**) calloc(MAX_LINK_COUNT, sizeof(struct Link*));
    struct Song** all_songs = (struct Song**) calloc(MAX_SONG_COUNT, sizeof(struct Song*));
    int total_artist_count = 0;
    int total_song_count = 0;
    int total_link_count = 0;
    int temp_artist_list[MAX_ARTIST_PER_SONG];
    
    //Create some artists
    create_artist(all_artists, &total_artist_count, "Becky G");
    create_artist(all_artists, &total_artist_count, "Gente De Zona");
    create_artist(all_artists, &total_artist_count, "Ana Mena");
    create_artist(all_artists, &total_artist_count, "De La Ghetto");

    //Create songs
    temp_artist_list[0] = get_artist_index(all_artists, &total_artist_count, "Becky G");
    temp_artist_list[1] = get_artist_index(all_artists, &total_artist_count, "Ana Mena");
    temp_artist_list[2] = get_artist_index(all_artists, &total_artist_count, "De La Ghetto");
    create_song(all_artists, &total_song_count, all_songs, "Ya Es Hora", temp_artist_list, 3);

    //File manip
    FILE* output_file = fopen("ArtistTree.dot", "w");
    fprintf(output_file, "graph ArtistTree{\n");

    //Nodes
    for (int i = 0; i < total_artist_count; i++) {
        struct Artist* current_artist = all_artists[i];
        fprintf(output_file, "\t%d[label=\"%s\"];\n", current_artist->id, current_artist->name);
    }
    //Edges
    for (int i = 0; i < total_link_count; i++) {
        struct Link* current_link = all_links[i];
        int artist_one_id = current_link->artist_one->id;
        int artist_two_id = current_link->artist_two->id;
        fprintf(output_file, "\t%d -- %d [label=\"%s\"];\n",
            artist_one_id, artist_two_id, current_link->song->song_name);
    }
    fprintf(output_file, "}");
    //Close file to avoid leaks
    fclose(output_file);

    //Free all memory
    for (int i = 0; i < total_link_count; i++) {
        free(all_links[i]);
    }
    for (int i = 0; i < total_song_count; i++) {
        free(all_songs[i]->artists);
        free(all_songs[i]);
    }
    for (int i = 0; i < total_artist_count; i++) {
        free(all_artists[i]->links);
        free(all_artists[i]);
    }
    free(all_links);
    free(all_songs);
    free(all_artists);


    printf("Visualization Complete!\n");
}