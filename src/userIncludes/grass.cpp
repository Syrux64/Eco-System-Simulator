#ifndef GRASS_CPP
#define GRASS_CPP

#include "rabbit.cpp"


class Grass: public Rabbit{
    protected:
        int change_state;
        SDL_Surface *images[4];
        int once_texture_flag;
    public:
        SDL_Rect grass_rect;
        int current_state;
        SDL_Texture* grass_texture[4];
        int timer_growth;

        Grass(){
        images[0] = IMG_Load("images/grass_0.png");
        images[1] = IMG_Load("images/grass_1.png");
        images[2] = IMG_Load("images/grass_2.png");
        images[3] = IMG_Load("images/grass_3.png");
        current_state = random_number(-3,3);
        grass_rect = {location_x, location_y, 32, 32};
        change_state = 1;
        timer_growth = SDL_GetTicks();
        once_texture_flag = 1;
        }

        void render_grass(SDL_Renderer *grass_renderer){
                if(once_texture_flag == 1){
                    for(int i=0;i<4;i++){
                        grass_texture[i] = SDL_CreateTextureFromSurface(grass_renderer, images[i]);
                        SDL_FreeSurface(images[i]);
                    }
                    once_texture_flag ++;
                }
                if(current_state >= 0){
                    SDL_RenderCopy(grass_renderer, grass_texture[current_state], NULL, &grass_rect);
                }
        }

        void grass_growth(int current_timer){
            if(current_timer - timer_growth > (random_number(15, 30)*random_number(1000, 2000))){
                if(current_state != 3){
                    current_state += 1;  
                    timer_growth = SDL_GetTicks();
                }
            }
        }

        // Rabbit Eating Grass Function -->
        bool rabbit_eat_flag(SDL_Rect b){
                SDL_HasIntersection(&grass_rect, &b);
        }    
};

#endif 