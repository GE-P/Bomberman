/*
** ETNA PROJECT, 20/10/2022 by eibl_g
** /group-988943/Bomberman
** File description:
**      Bomberman game in C17 + SDL2
**      Game logic file
**
*/

#include "game.h"

// Map
char *table[] = {
    "111111111111111",
    "100000000000001",
    "101010101010101",
    "100000000000001",
    "101010101010101",
    "100000000000001",
    "101010101010101",
    "100000000000001",
    "101010101010101",
    "100000000000001",
    "101010101010101",
    "100000000000001",
    "101010101010101",
    "100000000000001",
    "111111111111111"};

// Game initialisation
stGame *game_init()
{
    stGame *game = NULL;
    game = malloc(sizeof(stGame));

    memset(game->message, '\0', 128);

    game->screenSize.x = MAP_SIZE_x * TILE_SIZE_x; // Those two are going to be removed, doing maths for nothing, plus won't help for review.
    game->screenSize.y = MAP_SIZE_y * TILE_SIZE_y; // Also, without this crap we could easyly change sizes.
    game->pWindow = NULL;
    game->pRenderer = NULL;

    game->pTexturePlayer = NULL;
    game->pTextureMap = NULL;
    game->pTextureBomb = NULL;
    game->pTextureTile = NULL;
    game->pTextureBombAnim_1 = NULL;
    game->pTextureBombAnim_2 = NULL;

    game->playerPositionRect.x = 300; // This will change to a init -> next architecture thing.
    game->playerPositionRect.y = 300; // Same
    game->playerPositionRect.w = 40;
    game->playerPositionRect.h = 40;

    game->player2PositionRect.x = 300; 
    game->player2PositionRect.y = 300; 
    game->player2PositionRect.w = 40;
    game->player2PositionRect.h = 40;

    game->mapPositionRect.x = game->screenSize.x / 600;
    game->mapPositionRect.y = game->screenSize.y / 600;
    game->mapPositionRect.w = 600;
    game->mapPositionRect.h = 600;

    game->bombPositionRect.x = -40;
    game->bombPositionRect.y = -40;
    game->bombPositionRect.w = 40;
    game->bombPositionRect.h = 40;

    game->Rect_bombAnim_1.x = -40;
    game->Rect_bombAnim_1.y = -40;
    game->Rect_bombAnim_1.w = 30;
    game->Rect_bombAnim_1.h = 30;

    game->Rect_bombAnim_2.x = -40;
    game->Rect_bombAnim_2.y = -40;
    game->Rect_bombAnim_2.w = 30;
    game->Rect_bombAnim_2.h = 30;

    TTF_Init();

    // SDL initialisation
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL Error : %s\n", SDL_GetError());
        game_destroy(game);
        return NULL;
    }

    // Mixer initialisation
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
    {
        fprintf(stderr, "SDLmixer error : %s\n", Mix_GetError());
        game_destroy(game);
        return NULL;
    }

    // Window creation
    game->pWindow = SDL_CreateWindow("Bomberman alpha 0.09", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game->screenSize.x, game->screenSize.y, SDL_WINDOW_SHOWN);

    if (game->pWindow)
    {

        // Renderer creation
        game->pRenderer = SDL_CreateRenderer(game->pWindow, -1, SDL_RENDERER_ACCELERATED);
        if (!game->pRenderer)
        {
            fprintf(stderr, "Renderer Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
    }
    else
    {
        fprintf(stderr, "Window creation Error : %s\n", SDL_GetError());
        game_destroy(game);
        return NULL;
    }

    // Loading menu texture
    SDL_Surface *surfaceMap = IMG_Load("./src/textures/menu_test2.png");
    if (!surfaceMap)
    {
        fprintf(stderr, "Texture loading Error : %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    }
    else
    {
        game->pTextureMap = SDL_CreateTextureFromSurface(game->pRenderer, surfaceMap);
        if (!game->pTextureMap)
        {
            fprintf(stderr, "Texture treatment Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceMap);
    }

    // Loading the tile texture
    SDL_Surface *surfaceTile = IMG_Load("./src/textures/tile.png");
    if (!surfaceTile)
    {
        fprintf(stderr, "Texture loading Error : %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    }
    else
    {
        game->pTextureTile = SDL_CreateTextureFromSurface(game->pRenderer, surfaceTile);
        if (!game->pTextureTile)
        {
            fprintf(stderr, "Texture treatment Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceTile);
    }

    // Loading the bloc texture
    SDL_Surface *surfaceBloc = IMG_Load("./src/textures/bloc.png");
    if (!surfaceBloc)
    {
        fprintf(stderr, "Texture loading Error : %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    }
    else
    {
        game->pTextureBloc = SDL_CreateTextureFromSurface(game->pRenderer, surfaceBloc);
        if (!game->pTextureBloc)
        {
            fprintf(stderr, "Texture treatment Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceBloc);
    }

    // Loading player texture
    SDL_Surface *surfacePlayer = IMG_Load("./src/textures/bomberman2.png");
    if (!surfacePlayer)
    {
        fprintf(stderr, "Texture loading Error : %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    }
    else
    {
        game->pTexturePlayer = SDL_CreateTextureFromSurface(game->pRenderer, surfacePlayer);
        if (!game->pTexturePlayer)
        {
            fprintf(stderr, "Texture treatment Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfacePlayer);
    }

    // Loading bomb texture
    SDL_Surface *surfaceBomb = IMG_Load("./src/textures/bomb2.png");
    if (!surfaceBomb)
    {
        fprintf(stderr, "Texture loading Error : %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    }
    else
    {
        game->pTextureBomb = SDL_CreateTextureFromSurface(game->pRenderer, surfaceBomb);
        if (!game->pTextureBomb)
        {
            fprintf(stderr, "Texture treatment Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceBomb);
    }

    // Loading bombAnim1 texture
    SDL_Surface *surfaceBombAnim1 = IMG_Load("./src/textures/bomb_anim_1.png");
    if (!surfaceBombAnim1)
    {
        fprintf(stderr, "Anim1 loading Error : %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    }
    else
    {
        game->pTextureBombAnim_1 = SDL_CreateTextureFromSurface(game->pRenderer, surfaceBombAnim1);
        if (!game->pTextureBombAnim_1)
        {
            fprintf(stderr, "Anim 1Texture treatment Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceBombAnim1);
    }

    // Loading bombAnim2 texture
    SDL_Surface *surfaceBombAnim2 = IMG_Load("./src/textures/bomb_anim_2.png");
    if (!surfaceBombAnim2)
    {
        fprintf(stderr, "Anim 2 Texture loading Error : %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    }
    else
    {
        game->pTextureBombAnim_2 = SDL_CreateTextureFromSurface(game->pRenderer, surfaceBombAnim2);
        if (!game->pTextureBombAnim_2)
        {
            fprintf(stderr, "Anim 2 Texture treatment Error : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceBombAnim2);
    }

    return game;
}

// Destroy function
void game_destroy(stGame *game)
{

    if (game)
    {

        if (game->pTexturePlayer)
        {
            SDL_DestroyTexture(game->pTexturePlayer);
        }

        if (game->pTextureMap)
        {
            SDL_DestroyTexture(game->pTextureMap);
        }

        if (game->pTextureBombAnim_1)
        {
            SDL_DestroyTexture(game->pTextureBombAnim_1);
        }

        if (game->pTextureBombAnim_2)
        {
            SDL_DestroyTexture(game->pTextureBombAnim_2);
        }

        if (game->pTextureBloc)
        {
            SDL_DestroyTexture(game->pTextureBloc);
        }

        if (game->pTextureTile)
        {
            SDL_DestroyTexture(game->pTextureTile);
        }

        if (game->pTextureBomb)
        {
            SDL_DestroyTexture(game->pTextureBomb);
        }

        if (game->pRenderer)
        {
            SDL_DestroyRenderer(game->pRenderer);
        }

        if (game->pWindow)
        {
            SDL_DestroyWindow(game->pWindow);
        }
        SDL_Quit();
        free(game);
    }
}

// Game Render function
void game_draw(stGame *game)
{

    // This is very important, with this we define our tiles sizes. Looks like crap isn't it ?
    int i, j;
    game->Rect_source_tile.w = TILE_SIZE_x;
    game->Rect_source_tile.h = TILE_SIZE_y;
    game->Rect_source_bloc.w = TILE_SIZE_x;
    game->Rect_source_bloc.h = TILE_SIZE_y;
    game->Rect_dest.w = TILE_SIZE_x;
    game->Rect_dest.h = TILE_SIZE_y;
    // So don't remove them or u will have a nice dark crap screen.

    // Clean screen
    SDL_SetRenderDrawColor(game->pRenderer, 0, 0, 0, 255);

    SDL_RenderClear(game->pRenderer);

    // Render the map
    for (i = 0; i < MAP_SIZE_x; i++)
    {

        for (j = 0; j < MAP_SIZE_y; j++)
        {
            // This is the map calculation. Think it like i start on first line, all 1 is one texture all 0 is another.
            //  And then we start again on the next line, and so on.
            //  think X Y !
            game->Rect_dest.x = i * TILE_SIZE_x;
            game->Rect_dest.y = j * TILE_SIZE_y;
            // Here we render the white tiles. 1
            game->Rect_source_tile.x = (table[j][i] - '0') * TILE_SIZE_x;
            // Why not y ? because we render line by line !
            game->Rect_source_tile.y = 0;
            // Here we render all the black tiles. 2
            game->Rect_source_bloc.x = (table[j][i] - '1') * TILE_SIZE_x;
            // Again, same logic for second blocs.
            game->Rect_source_bloc.y = 0;
            // 1
            SDL_RenderCopy(game->pRenderer, game->pTextureTile, &game->Rect_source_tile, &game->Rect_dest);
            // 2
            SDL_RenderCopy(game->pRenderer, game->pTextureBloc, &game->Rect_source_bloc, &game->Rect_dest);
        }
    }

    // SDL_RenderCopy(game->pRenderer, game->pTextureMap, NULL, &game->mapPositionRect);

    // Offset calculation for the player texture
    game->playertextureRect = game->playerPositionRect;
    game->playertextureRect.x -= game->playertextureRect.w / 2;
    game->playertextureRect.y -= game->playertextureRect.h / 2;

    game->player2textureRect = game->player2PositionRect;
    game->player2textureRect.x -= game->player2textureRect.w / 2;
    game->player2textureRect.y -= game->player2textureRect.h / 2;

    // Render the player
    SDL_RenderCopy(game->pRenderer, game->pTexturePlayer, NULL, &game->playertextureRect);

    //printf("number player: %d", game->oponent_connected);

    if (game->oponent_connected > 0){

        // Render the oponent
        SDL_RenderCopy(game->pRenderer, game->pTexturePlayer, NULL, &game->player2textureRect);
    }  

    //game->bombPositionRect.x -= game->bombPositionRect.w / 2;
    //game->bombPositionRect.y -= game->bombPositionRect.h / 2;

    // Render the bomb
    SDL_RenderCopy(game->pRenderer, game->pTextureBomb, NULL, &game->bombPositionRect);

    // Render the bombAnim1
    SDL_RenderCopy(game->pRenderer, game->pTextureBombAnim_1, NULL, &game->Rect_bombAnim_1);

    // Render the bombAnim2
    SDL_RenderCopy(game->pRenderer, game->pTextureBombAnim_2, NULL, &game->Rect_bombAnim_2);

    // Print screen
    SDL_RenderPresent(game->pRenderer);
}

void display_text(stGame *game, char *text, int x, int y)
{

    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect pos;
    SDL_Color color = {255, 255, 255, 0};
    SDL_SetRenderDrawColor(game->pRenderer, 0, 75, 150, 255);
    surface = TTF_RenderText_Solid(game->font, text, color);
    texture = SDL_CreateTextureFromSurface(game->pRenderer, surface);
    // pos.w = surface->w;
    // pos.h = surface->h;
    // pos.w = 250;
    // pos.h = 50;
    // pos.x = 180;
    // pos.y = 250;
    pos.w = surface->w * 2;
    pos.h = surface->h * 2;
    pos.x = x;
    pos.y = y;
    SDL_FreeSurface(surface);
    // SDL_RenderCopy(game->pRenderer, game->pTextureMap, NULL, &game->mapPositionRect);
    SDL_RenderCopy(game->pRenderer, texture, NULL, &pos);
}

void menu_display(stGame *game)
{

    SDL_Event e;
    int pressed = false;
    while (!pressed)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_c)
                {
                    game->menu = 'c';
                    pressed = true;
                }
                else if (e.key.keysym.sym == SDLK_s)
                {
                    game->menu = 's';
                    pressed = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(game->pRenderer);
        display_text(game, "[- BOMBERMAN v0.9 -]", 125, 50);
        game->menu_message = "[s]erver or [c]lient?";
        display_text(game, game->menu_message, 150, 200);
        SDL_RenderPresent(game->pRenderer);
    }
}

void menu_display2(stGame *game)
{

    SDL_Event e;
    int pressed = false;
    while (!pressed)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_y)
                {
                    game->menu2 = 'y';
                    pressed = true;
                }
                else if (e.key.keysym.sym == SDLK_n)
                {
                    game->menu2 = 'n';
                    pressed = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(game->pRenderer);
        display_text(game, "[- BOMBERMAN v0.9 -]", 125, 50);        
        display_text(game, "Play game ? [Y]es - [N]o", 125, 200);
        display_text(game, "Server launched", 150, 400);
        SDL_RenderPresent(game->pRenderer);
    }
}

void get_address(stGame *game, char *ip)
{
    memset(ip, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || e.key.keysym.sym == SDLK_p)
                {
                    if (position > 14)
                    {
                        position = 14;
                    }
                    if (e.key.keysym.sym == SDLK_p)
                    {
                        e.key.keysym.sym = '.';
                    }
                    ip[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    position--;
                    if (position < 0)
                    {
                        position = 0;
                    }
                    ip[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    ip[position] = 0;
                    ok = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(game->pRenderer);
        display_text(game, "[- BOMBERMAN v0.9 -]", 125, 50);
        game->menu_message = "Type the IP :";
        display_text(game, game->menu_message, 150, 200);

        display_text(game, ip, 240, 240);
        SDL_RenderPresent(game->pRenderer);
    }
}

int game_event(stGame *game)
{
    int result = 0;
    int trigger = 250;
    int trigger_anim = 75;

    if (game->clocking != 0)
    {
        clock_t difference = clock() - game->clocking;
        game->msec = difference * 1000 / CLOCKS_PER_SEC;
        fprintf(stderr, "msec ici : %d\n", game->msec);

        if (game->msec > trigger)
        {
            fprintf(stderr, "Boom");
            game->clocking_anim = clock();
            game->Rect_bombAnim_1.x = game->bombPositionRect.x;
            game->Rect_bombAnim_1.y = game->bombPositionRect.y;
            Mix_AllocateChannels(32);                       // Allocate 32 channels, could be 10 whatever
            Mix_Volume(1, MIX_MAX_VOLUME / 2);              // Medium volume for channel 1
            Mix_VolumeChunk(game->son, MIX_MAX_VOLUME / 2); // Volume for the wav loaded the chunk
            Mix_PlayChannel(1, game->son, 0);               // Plays the sound 1 in the channel 1 and plays one time 0 + 1
            game->clocking = 0;
            game->bombPositionRect.x = -40;
            game->bombPositionRect.y = -40;
        }
    }
    else if (game->clocking_anim != 0)
    {
        clock_t difference = clock() - game->clocking_anim;
        game->msec = difference * 1000 / CLOCKS_PER_SEC;
        fprintf(stderr, "msec ici : %d\n", game->msec);

        if (game->msec > trigger_anim)
        {
            fprintf(stderr, "Animation finish");
            game->Rect_bombAnim_1.x = -40;
            game->Rect_bombAnim_1.y = -40;
            game->clocking_anim = 0;
        }
    }

    SDL_Event e;
    if (SDL_PollEvent(&e))
    {

        if (e.type == SDL_QUIT)
        {
            result = -1;
        }
        else if (e.type == SDL_KEYDOWN)
        {

            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                result = -1;
                break;
            case SDLK_1:
            case SDLK_2:
            case SDLK_3:
            case SDLK_DOWN:
            case SDLK_UP:
            case SDLK_LEFT:
            case SDLK_RIGHT:
            case SDLK_SPACE:
                game_movePlayer(game, e.key.keysym.sym);
                break;
            default:
                fprintf(stderr, "Unknown key ! %d\n", e.key.keysym.sym);
                break;
            }
        }
    }

    return result;
}

int game_connect(stGame *game)
{
    int quit;
    int mysocket;
    int val;
    struct sockaddr_in server;
    char message[128];
    char message2[128];
    struct timeval timeout;
    fd_set readfs;
    int value;
    int value2;
    int value3;
    int value4;
    //char test[128] = {'c','o','d','e','\n'};
    regex_t reegex;

    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mysocket == -1)
    {

        perror("socket()");
        exit(errno);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(5555);

    if (connect(mysocket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {

        perror("connect()");
        exit(errno);
    }

    fcntl(mysocket, F_SETFL, O_NONBLOCK);

    while (quit!=-1)
    {
        
        //strcpy(game->position_message, test);

        timeout.tv_sec = 0;
        timeout.tv_usec = 500;

        FD_ZERO(&readfs);
        FD_SET(0, &readfs);

        val = select(1, &readfs, NULL, NULL, &timeout);

        //printf("position: %s\n", game->position_message);
        //printf("val: %d\n", val);

        strcpy(message, game->position_message);

        if (val == -1)
        {
            perror("select()");
        }
        else if (val)
        {
            //memset(message, '\0', 128);
            //fgets(message, 128, stdin);
            //message[0] = game->playerPositionRect.x;
            //message[1] = game->playerPositionRect.y;

            //strcpy(message, game->position_message);

            //message = game->position_message;
        }

        if (send(mysocket, message, strlen(message), MSG_NOSIGNAL) < 0)
        {

            puts("send failed");
            close(mysocket);
            return 1;
        }
        else
        {
            if (strlen(message) > 1) {
                
                printf("sended : %s\n", message);
                memset(message, '\0', 128);
                memset(game->position_message, '\0', 128);

            }
            
        }

        if (recvfrom(mysocket, message2, 128, 0, NULL, 0) > 0)
        {
            printf("received : %s\n", message2);

            value = regcomp( &reegex, "up", 0);
            value = regexec( &reegex, message2, 0, NULL, 0);

            value2 = regcomp( &reegex, "down", 0);
            value2 = regexec( &reegex, message2, 0, NULL, 0);

            value3 = regcomp( &reegex, "left", 0);
            value3 = regexec( &reegex, message2, 0, NULL, 0);

            value4 = regcomp( &reegex, "right", 0);           
            value4 = regexec( &reegex, message2, 0, NULL, 0);


            if (value == 0){
                game->player2PositionRect.y -= 40;
            }
            if (value2 == 0){
                game->player2PositionRect.y += 40;
            }
            if (value3 == 0){
                game->player2PositionRect.x -= 40;
            }
            if (value4 == 0){
                game->player2PositionRect.x += 40;
            }


            memset(message2, '\0', 128);


            game->oponent_connected = 1;

            

            //puts("empty\n");

            // puts("receive failed");
            // close(mysocket);
            // return 1;
        } 
       /* else
        {
            if (strlen(message2) > 0) {
                printf("received : %s\n", message2);
                memset(message2, '\0', 128);
            }
            
        }*/
        //game_event(game);

        game_draw(game);

        //printf("number player: %d", game->oponent_connected);
        
        //printf("looping\n");
        usleep(30);
        quit = game_event(game);
    }

    close(mysocket);

    printf("Closed client");

    game_destroy(game);

    return 0;
}

int read_client(stGame *game, int client)
{
    int n;
    char buff[128];

    if (client == -1)
    {
        return 1;
    }

    n = 0;
    memset(buff, '\0', 128);
    while ((n = recv(client, buff, 128, 0)) > 1)
    {
        if (n == 0)
        {
            return -1;
        }

        printf("From Player id: %d\n", client);
        printf("received: %s\n", buff);
        
        strcpy(game->message, buff);

        if (buff[n - 1] == '\n')
        {
            memset(buff, '\0', 128);
            break;
        }
    }
    return 0;
}

int game_host(stGame *game)
{
    int sock;
    int client1 = -1;
    int client2 = -1;
    socklen_t client_addr_len;
    struct sockaddr_in server;
    struct sockaddr_in client_addr;

    fd_set readfs;
    struct timeval timeout;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket()");
        return 1;
    }

    fcntl(sock, F_SETFL, O_NONBLOCK);

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(5555);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind()");
        return 1;
    }

    listen(sock, 5);

    puts("waiting clients...");
    // usleep(3000);
    puts("waiting for players : ");
    printf("Waiting for player 1\n");
    while (client1 < 0)
    {
        client1 = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);
        game->players[0] = init_players(client1);
    }

    if (client1 > 0)
    {
        printf("Waiting for player 2\n");
        while (client2 < 0)
        {
            client2 = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);
            game->players[1] = init_players(client2);
        }
    }

    // client1 = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);
    // client2 = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client1 < 0 || client2 < 0)
    {
        perror("accept()");
        return 1;
    }
    puts("new clients");

    while (1)
    {

        timeout.tv_sec = 0;
        timeout.tv_usec = 100;

        FD_ZERO(&readfs);

        FD_SET(client1, &readfs);
        FD_SET(client2, &readfs);

        select(client2 + 1, &readfs, NULL, NULL, &timeout);

        if (FD_ISSET(client1, &readfs))
        {
            if (read_client(game, client1) == -1)
            {
                puts("client 1 disconnected");
                close(client1);
                client1 = -1;
            }

            // sendto(sock, tab, sizeof(int16_t) * 2, 0,(struct sockaddr *) &serv_addr, serv_addr_size)

            send(client2, game->message, 128, MSG_NOSIGNAL);
            memset(game->message, '\0', 128);
        }
        else if (FD_ISSET(client2, &readfs))
        {
            if (read_client(game, client2) == -1)
            {
                puts("client 2 disconnected");
                close(client2);
                client2 = -1;
            }

            send(client1, game->message, 128, MSG_NOSIGNAL);
            memset(game->message, '\0', 128);
        }

        if (client1 == -1 && client2 == -1)
        {
            break;
        }

        //printf("p1: %d\n", client1);
        //printf("p2: %d\n", client2);

        //puts("looping");
        usleep(TICS);
    }
    close(sock);
    return 0;
}

struct Player init_players(int fd)
{
    struct Player player;
    player.fd = fd; 
    return player;  
}

/*struct Player add_pos(SDL_Rect position)
{
    struct Player player;
    player.position = position; 
    return player;  
}*/

// The controller for movement
void game_movePlayer(stGame *game, SDL_Keycode direction)
{
    int tileX = game->playerPositionRect.x / TILE_SIZE_x;
    int tileY = game->playerPositionRect.y / TILE_SIZE_y;
    int pos_left = tileX - 1;
    int pos_right = tileX + 1;
    int pos_up = tileY - 1;
    int pos_down = tileY + 1;

    

    if (direction == SDLK_UP)
    {

        if (game->playerPositionRect.y > 0 && table[pos_up][tileX] == 48)
        {
            game->playerPositionRect.y -= TILE_SIZE_y;
            char pos[128] = {'u', 'p', '\n'};
            strcpy(game->position_message, pos);

        }
    }
    else if (direction == SDLK_DOWN)
    {

        if (game->playerPositionRect.y < (game->screenSize.y - game->playerPositionRect.h) && table[pos_down][tileX] == 48)
        {
            game->playerPositionRect.y += TILE_SIZE_y;
            char pos[128] = {'d', 'o', 'w', 'n', '\n'};
            strcpy(game->position_message, pos);
        }
    }
    else if (direction == SDLK_LEFT)
    {

        if (game->playerPositionRect.x > 0 && table[tileY][pos_left] == 48)
        {
            game->playerPositionRect.x -= TILE_SIZE_x;
            char pos[128] = {'l', 'e', 'f', 't', '\n'};
            strcpy(game->position_message, pos);
        }
    }
    else if (direction == SDLK_RIGHT)
    {

        if (game->playerPositionRect.x < (game->screenSize.x - game->playerPositionRect.w) && table[tileY][pos_right] == 48)
        {
            game->playerPositionRect.x += TILE_SIZE_x;
            char pos[128] = {'r', 'i', 'g', 'h', 't', '\n'};
            strcpy(game->position_message, pos);
        }
    }
    else if (direction == SDLK_SPACE)
    {
        game->clocking = clock();
        game->bombPositionRect = game->playertextureRect;
        //game->bombPositionRect.x = game->playerPositionRect.x;
        //game->bombPositionRect.y = game->playerPositionRect.y;
    }
    else
    {
        printf("Bad direction ! n");
    }
}
