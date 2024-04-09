#ifndef LAKE_CPP
#define LAKE_CPP

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include<iostream>

#include "random_number.h"
#include "globals.h"


class Lake{
    private:
        SDL_Surface *lake_surface = IMG_Load("images/pond_4.png");
    public:
        SDL_Rect lake_rect;
        SDL_Texture *lake_texture;

        Lake(SDL_Renderer *renderer){
            lake_rect = {SCREEN_WIDTH - lake_surface->w, 0, lake_surface->w, 768};
            if (lake_surface == NULL)
                std::cout<<"Error loading image: "<< IMG_GetError();
            lake_texture = SDL_CreateTextureFromSurface(renderer, lake_surface);
            SDL_FreeSurface(lake_surface);
        }

        void render(SDL_Renderer* renderer){
            SDL_RenderCopy(renderer, lake_texture, NULL, &lake_rect);
        }

};

#endif 