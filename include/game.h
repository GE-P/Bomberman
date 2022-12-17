/*
** ETNA PROJECT, 20/10/2022 by eibl_g
** //group-988943/Bomberman
** File description:
**      Header file for bomberman game.c
*/

#ifndef GAME_H
#define GAME_H
#define MAP_SIZE_x    15
#define MAP_SIZE_y    15
#define TILE_SIZE_x    40
#define TILE_SIZE_y    40
#define BUFFSIZE    128
#define TICS 1000/16

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_mixer.h"
#include <SDL2/SDL_ttf.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>
#include <regex.h>
#include <sys/wait.h> 

struct Player {
    int fd;
    SDL_Rect position; 
    SDL_Texture *texture;
    
};

typedef struct {    

    //SDL variables
    SDL_Point screenSize;
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    int clocking;
    int quit;
    int clocking_anim;
    int msec;
    int msec_anim;
    int connect_button;
    int host_button;
    Mix_Chunk *son;
    TTF_Font *font;
    int number_of_players;

    char menu;
    char menu2;
    char *menu_message;
    char *menu_ip;
    char message[128];

    int oponent_connected;
    char position_message[128];

    //struct Player playerArr[2];
    struct Player players[2];

    //Textures
    SDL_Texture* pTexturePlayer;    
    SDL_Texture* pTextureMap;  //Old map Image --> to be removed  
    SDL_Texture* pTextureBomb;    
    SDL_Texture* pTextureTile;
    SDL_Texture* pTextureBloc; 
    SDL_Texture* pTextureBombAnim_1;
    SDL_Texture* pTextureBombAnim_2;    

    //Coord 
    SDL_Rect playerPositionRect;
    SDL_Rect player2PositionRect;
    SDL_Rect playertextureRect;
    SDL_Rect player2textureRect;
    SDL_Rect mapPositionRect; //Old one for the image map --> to be removed
    SDL_Rect bombPositionRect;
    SDL_Rect Rect_dest;
    SDL_Rect Rect_source_tile;
    SDL_Rect Rect_source_bloc;
    SDL_Rect Rect_bombAnim_1;
    SDL_Rect Rect_bombAnim_2;

    

} stGame;




stGame* game_init();
void game_destroy(stGame* game);
void game_draw(stGame* game);
int game_event(stGame* game);
int game_connect();
int game_host();
void game_movePlayer(stGame* game, SDL_Keycode direction);
struct Player init_players(int fd);

/// Menu ///
void display_text(stGame *game, char *text, int x, int y);
void menu_display(stGame *game);
void menu_display2(stGame *game);
void get_address(stGame *game, char *ip);

#endif