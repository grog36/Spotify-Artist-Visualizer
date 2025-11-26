/**
* Spotify Artist Visualization header file
* @author Gregory Ecklund
* @version November 26, 2025
* This file contains constant definitions, struct definitions,
* and function prototypes used for the Spotify Artist Visualizer
*/

/** Preprocessor directive to define macros */
#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <stdio.h> //Standard input-output
#include <stdlib.h> //Standard library for C (for malloc)
#include <string.h> //Standard library for C strings

/** True and False Macros */
#define TRUE 1
#define FALSE 0

/** CONSTANTS */
#define MAX_ARTIST_COUNT 250
#define MAX_ARTIST_LENGTH 40
#define MAX_ARTIST_PER_SONG 10
#define MAX_LINK_COUNT 1000
#define MAX_LINK_PER_ARTIST 15
#define MAX_SONG_COUNT 500
#define MAX_SONG_LENGTH 50
#define MAX_LINE_LENGTH 250


/**
* Struct for an Artist (Vertex) for the graph
*
* char* name - The name of the artist
* int id - Unique id for each artist
* Link** links - List of links associated with the artist
* int link_count - Number of links associated with the artist
*/
struct Artist {
    char* name;
    int id;
    struct Link** links;
    int link_count;
};

/**
* Struct for a Link (Edge) for the graph
*
* Artist* artist_one - One of the artists connected by the link
* Artist* artist_two - The other artist connected by the link
* Song* song - A reference to the song that creates the link
*/
struct Link {
    struct Artist* artist_one;
    struct Artist* artist_two;
    struct Song* song;
};

/**
* Struct for a Song for the graph
* char* song_name - The name of the song
* Artist** - A list of references to the artists in the song
* int artist_count - The number of artists on the song
*/
struct Song {
    char* song_name;
    struct Artist** artists;
    int artist_count;
};

/**
* Struct to hold all of the global variables for the project
*
* Artist** all_artists - A list of all of the artists (vertices) in the graph
* Link** all_links - A list containing all of the links (edges) in the graph
* Song** all_songs - A list containing all of the songs in the graph
* int total_artist_count - The total number of artists (vertices) in the graph
* int total_song_count - The total number of songs for the project
* int total_link_count - The total number of links (edges) in the graph
*/
struct Globals {
    struct Artist** all_artists;
    struct Link** all_links;
    struct Song** all_songs;
    int total_artist_count;
    int total_song_count;
    int total_link_count;
};

/** FUNCTION PROTOTYPES */
//Helper functions
int get_artist_index(struct Globals* global_vars, char* name);
struct Artist* create_artist(struct Globals* global_vars, char* name);
struct Song* create_song(struct Globals* global_vars, char* name, int* artist_ids, int num_of_artists);
struct Link* create_link(struct Globals* global_vars, struct Artist* first_artist, struct Artist* second_artist, struct Song* song);
void print_artist(struct Artist* artist);
void print_song(struct Song* song);
void free_all_memory(struct Globals* global_vars);
void create_all_links(struct Globals* global_vars);
//Actual functions
void save_to_file(char* output_filename, struct Globals* global_vars);
void read_from_file(char* input_filename, struct Globals* global_vars);
void run_visualizer();

#endif //End preprocessor directive to define macros