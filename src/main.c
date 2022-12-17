/*
** ETNA PROJECT, 20/10/2022 by eibl_g
** /group-988943/Bomberman
** File description:
**      Bonberman main file
*/

#include "game.h"

int main()
{
    printf("Welcome to Bomberman\n");

    stGame *game = game_init();
    Mix_Music *musique;                                  // Creation of pointer of type Mix_Music
    musique = Mix_LoadMUS("./src/sounds/disco.mp3");     // Loading music into variable
    game->son = Mix_LoadWAV("./src/sounds/EXPLODE.wav"); // Loading the wav into a pointer see struct
    game->font = TTF_OpenFont("./src/fonts/Pixel.ttf", 24);
    Mix_PlayMusic(musique, -1); // Plays music indefinitly
    // ix_VolumeMusic(MIX_MAX_VOLUME / 100);
    Mix_VolumeMusic(0);

    //int quit = 0;

    menu_display(game);
    if (game->menu == 'c')
    {
        game_connect(game);
    }

    if (game->menu == 's')
    {
        pid_t pid = fork();
        if (pid == 0)
        { 
            system("./output/serveur_v1");
            
        }
        kill(pid, SIGTERM);
        menu_display2(game);
        if (game->menu2 == 'y') {
        game_connect(game);
        }
        kill(pid, SIGKILL);
       
    }

    

    /*while (quit!=-1)
    {
        game_draw(game);
        quit = game_event(game);
    }*/
   
    Mix_FreeMusic(musique);   // Music remove
    Mix_FreeChunk(game->son); // Bomb sound remove
    Mix_CloseAudio();         // Closing the audio API

    game_destroy(game);

    return (EXIT_SUCCESS);
}
