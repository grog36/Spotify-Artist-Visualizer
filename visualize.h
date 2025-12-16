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
#define MAX_ARTIST_COUNT 2000
#define MAX_ARTIST_LENGTH 50
#define MAX_ARTIST_PER_SONG 10
#define MAX_LINK_COUNT 2500
#define MAX_LINK_PER_ARTIST 100
#define MAX_SONG_COUNT 2000
#define MAX_SONG_LENGTH 50
#define MAX_LINE_LENGTH 250

/** VISUALIZATION VALUES */
#define DEFAULT_FILLCOLOR "white"
#define START_FILLCOLOR "green"
#define CHAIN_FILLCOLOR "#592C88"
#define END_FILLCOLOR "#C1A875"
#define DEFAULT_PENWIDTH 1
#define PATH_PENWIDTH 2
#define DEFAULT_LINK_COLOR "black"
#define DEFAULT_SONGNAME_COLOR "black"
#define PATH_SONGNAME_COLOR "#592C88"
#define DEFAULT_SONGNAME_FONTSIZE 14
#define PATH_SONGNAME_FONTSIZE 20


/**
* Struct for an Artist (Vertex) for the graph
*
* char* name - The name of the artist
* int id - Unique id for each artist
* char* fillcolor - The background color of the node in the graph
* Link** links - List of links associated with the artist
* int link_count - Number of links associated with the artist
*/
struct Artist {
    char* name;
    int id;
    char* fillcolor;
    struct Link** links;
    int link_count;
};

/**
* Struct for a Link (Edge) for the graph
*
* Artist* artist_one - One of the artists connected by the link
* Artist* artist_two - The other artist connected by the link
* char* color - The color to make the edge in the graph
* int penwidth - The size of the edge in the graph
* Song* song - A reference to the song that creates the link
* char* songname_color - The color to make the song name in the graph
* int songname_fontsize - The size to make the font of the song name in the graph
*/
struct Link {
    struct Artist* artist_one;
    struct Artist* artist_two;
    char* color;
    int penwidth;
    struct Song* song;
    char* songname_color;
    int songname_fontsize;
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
int check_for_uniqueness(int* set, int set_size, int new_num);
struct Link* find_link(struct Globals* global_vars, struct Artist* artist_one, struct Artist* artist_two);
int link_already_exists(struct Globals* global_vars, struct Artist* artist_one, struct Artist* artist_two);
struct Artist* create_artist(struct Globals* global_vars, char* name);
struct Song* create_song(struct Globals* global_vars, char* name, int* artist_ids, int num_of_artists);
struct Link* create_link(struct Globals* global_vars, struct Artist* first_artist, struct Artist* second_artist, struct Song* song);
void print_artist(struct Artist* artist);
void print_song(struct Song* song);
void print_link(struct Link* link);
void free_all_memory(struct Globals* global_vars);
void create_all_links(struct Globals* global_vars);
//Main Functions
void save_to_file(char* output_filename, struct Globals* global_vars);
int breadth_first_search(struct Globals* global_vars, char* start_artist_name, char* end_artist_name);
void read_from_file(char* input_filename, struct Globals* global_vars);
void run_visualizer(char* input_filename, char* output_filename);

#endif //End preprocessor directive to define macros