/**
* Spotify Artist Visualization header file
* @author Gregory Ecklund
* @version November 10, 2025
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
* Struct for an Artist for the graph
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
* Struct for a Link for the graph
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
* char* song_name - The name of the song
* Artist** - A list of references to the artists in the song
* int artist_count - The number of artists on the song
*/
struct Song {
    char* song_name;
    struct Artist** artists;
    int artist_count;
};

struct Globals {
    struct Artist** all_artists;
    struct Link** all_links;
    struct Song** all_songs;
    int total_artist_count;
    int total_song_count;
    int total_link_count;
};


#endif //End preprocessor directive to define macros