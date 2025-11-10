/**
* Spotify Artist Visualization
* @author Gregory Ecklund
* @version November 10, 2025
* This file contains the function definitions/implementations
* necessary for the visualization process
*/

//Include necessary header file
#include "./visualize.h"

int main(void) {

    //Create songs
    struct Song* ya_es_hora = (struct Song*) malloc(sizeof(struct Song));
    ya_es_hora->song_name = "Ya Es Hora";
    ya_es_hora->artist_count = 3;
    ya_es_hora->artists = (struct Artist**) calloc((ya_es_hora->artist_count), sizeof(struct Artist*));

    struct Song* muchacha = (struct Song*) malloc(sizeof(struct Song));
    muchacha->song_name = "Muchacha";
    muchacha->artist_count = 2;
    muchacha->artists = (struct Artist**) calloc((muchacha->artist_count), sizeof(struct Artist*));

    struct Song* momento = (struct Song*) malloc(sizeof(struct Song));
    momento->song_name = "Momento";
    momento->artist_count = 2;
    momento->artists = (struct Artist**) calloc((momento->artist_count), sizeof(struct Artist*));

    struct Song* mamiii = (struct Song*) malloc(sizeof(struct Song));
    mamiii->song_name = "MAMIII";
    mamiii->artist_count = 2;
    mamiii->artists = (struct Artist**) calloc((mamiii->artist_count), sizeof(struct Artist*));
    
    //Create some artists
    struct Artist** all_artists = (struct Artist**) calloc(MAX_ARTIST_COUNT, sizeof(struct Artist*));
    int current_artist_index = 0;

    struct Artist* becky_g = (struct Artist*) malloc(sizeof(struct Artist));
    becky_g->name = "Becky G";
    becky_g->link_count = 4;
    becky_g->links = (struct Link**) calloc((becky_g->link_count), sizeof(struct Link*));
    becky_g->id = current_artist_index;
    all_artists[current_artist_index] = becky_g;
    current_artist_index++;
    
    struct Artist* gente_de_zona = (struct Artist*) malloc(sizeof(struct Artist));
    gente_de_zona->name = "Gente De Zona";
    gente_de_zona->link_count = 2;
    gente_de_zona->links = (struct Link**) calloc((gente_de_zona->link_count), sizeof(struct Link*));
    gente_de_zona->id = current_artist_index;
    all_artists[current_artist_index] = gente_de_zona;
    current_artist_index++;

    struct Artist* ana_mena = (struct Artist*) malloc(sizeof(struct Artist));
    ana_mena->name = "Ana Mena";
    ana_mena->link_count = 3;
    ana_mena->links = (struct Link**) calloc((ana_mena->link_count), sizeof(struct Link*));
    ana_mena->id = current_artist_index;
    all_artists[current_artist_index] = ana_mena;
    current_artist_index++;

    struct Artist* de_la_ghetto = (struct Artist*) malloc(sizeof(struct Artist));
    de_la_ghetto->name = "De La Ghetto";
    de_la_ghetto->link_count = 2;
    de_la_ghetto->links = (struct Link**) calloc((de_la_ghetto->link_count), sizeof(struct Link*));
    de_la_ghetto->id = current_artist_index;
    all_artists[current_artist_index] = de_la_ghetto;
    current_artist_index++;

    struct Artist* karol_g = (struct Artist*) malloc(sizeof(struct Artist));
    karol_g->name = "Karol G";
    karol_g->link_count = 1;
    karol_g->links = (struct Link**) calloc((karol_g->link_count), sizeof(struct Link*));
    karol_g->id = current_artist_index;
    all_artists[current_artist_index] = karol_g;
    current_artist_index++;


    //Create Links
    struct Link** all_links = (struct Link**) calloc(MAX_LINK_COUNT, sizeof(struct Link*));
    int current_link_index = 0;
    struct Link ab;
    ab.artist_one = de_la_ghetto;
    ab.artist_two = becky_g;
    ab.song = ya_es_hora;
    all_links[current_link_index] = &ab;
    current_link_index++;

    struct Link ac;
    ac.artist_one = de_la_ghetto;
    ac.artist_two = ana_mena;
    ac.song = ya_es_hora;
    all_links[current_link_index] = &ac;
    current_link_index++;

    struct Link bc;
    bc.artist_one = becky_g;
    bc.artist_two = ana_mena;
    bc.song = ya_es_hora;
    all_links[current_link_index] = &bc;
    current_link_index++;

    struct Link ce;
    ce.artist_one = ana_mena;
    ce.artist_two = gente_de_zona;
    ce.song = momento;
    all_links[current_link_index] = &ce;
    current_link_index++;

    struct Link be;
    be.artist_one = becky_g;
    be.artist_two = gente_de_zona;
    be.song = muchacha;
    all_links[current_link_index] = &be;
    current_link_index++;

    struct Link bd;
    bd.artist_one = becky_g;
    bd.artist_two = karol_g;
    bd.song = mamiii;
    all_links[current_link_index] = &bd;
    current_link_index++;


    //File manip
    FILE* output_file = fopen("ArtistTree.dot", "w");
    fprintf(output_file, "graph ArtistTree{\n");

    //Nodes
    for (int i = 0; i < current_artist_index; i++) {
        struct Artist* current_artist = all_artists[i];
        fprintf(output_file, "\t%d[label=\"%s\"];\n", current_artist->id, current_artist->name);
    }
    //Edges
    for (int i = 0; i < current_link_index; i++) {
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
    free(karol_g->links);
    free(karol_g);
    free(de_la_ghetto->links);
    free(de_la_ghetto);
    free(ana_mena->links);
    free(ana_mena);
    free(gente_de_zona->links);
    free(gente_de_zona);
    free(becky_g->links);
    free(becky_g);
    free(mamiii->artists);
    free(mamiii);
    free(muchacha->artists);
    free(muchacha);
    free(momento->artists);
    free(momento);
    free(ya_es_hora->artists);
    free(ya_es_hora);


    printf("Hello there!\n");
}