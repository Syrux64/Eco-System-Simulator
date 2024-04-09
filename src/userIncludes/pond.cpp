#ifndef POND_CPP
#define POND_CPP

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include<iostream>
#include<string.h>

#include "random_number.h"
#include "globals.h"


class Pond{
    private:
        SDL_Surface *pond_surface_1, *pond_surface_2 ;
    
    public:
        SDL_Rect pond_rect_1, pond_rect_2;
        SDL_Texture *pond_texture_1, *pond_texture_2 ;
        int location_x, location_y;

        Pond(SDL_Renderer *renderer){
            location_x = random_number(100, 600);
            location_y = random_number(70, 200);
            pond_rect_1 = {location_x, location_y, 170, 170};
            location_x = random_number(200, 400);
            location_y = random_number(450, 600);
            pond_rect_2 = {location_x, location_y, 170, 170};
            
            pond_surface_1 = IMG_Load("images/pond_2.png");
            pond_texture_1 = SDL_CreateTextureFromSurface(renderer, pond_surface_1);
            SDL_FreeSurface(pond_surface_1);

            pond_surface_2 = IMG_Load("images/pond_3.png");
            pond_texture_2 = SDL_CreateTextureFromSurface(renderer, pond_surface_2);
            SDL_FreeSurface(pond_surface_2);
        }

        void render(SDL_Renderer *renderer){
            SDL_RenderCopy(renderer, pond_texture_1, NULL, &pond_rect_1);
            SDL_RenderCopy(renderer, pond_texture_2, NULL, &pond_rect_2);
        }
};

#endif 