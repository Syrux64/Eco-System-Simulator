#ifndef FOX_CPP
#define FOX_CPP

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include<iostream>
#include<string.h>

#include "random_number.h"
#include "globals.h"

class Fox{
    protected:
        float movement_speed = 3.5;
        int velx, vely;
        SDL_Surface *fox_surface[4];
        SDL_Surface *info_bar_surface[3];
        SDL_Rect hunger_bar_rect, rest_bar_rect, mate_bar_rect;

    public:
        // Essentials

        bool is_dead = false;
        int traits[4]; //[vision, hunger, rest, mate]
        int location_x, location_y, destination_x, destination_y, move_x, move_y;
        float hunger_level = random_number(70, 100);
        int timer_movement, time_wait, timer_pregnancy, timer_growth, time_alive, timer_dead, timer_spawn;
        int timer_low_hunger, timer_low_mate, timer_low_rest;
        bool target_found_rabbit = false, hungry, rest, mate;
        bool is_kit;
        bool found_partner;
        bool match;
        bool wait = false;
        float hunger_level_max = hunger_level;
        float rest_level, rest_level_max;
        float mate_level, mate_level_max;
        int vision_radius;
        int gender; // 0:male / 1:female
        int width, height;
        bool is_pregnant = false;bool kit_born;
        SDL_Rect fox_rect, fox_vision, target_mate;
        SDL_Texture *fox_texture, *fox_dead_texture;
        SDL_Texture *info_hungry_texture, *info_rest_texture, *info_mate_texture;
        float amp;float freq;
        
        // flags
        int flag_once_eat = 1, timer_eat, flag_once_mate = 1, spawn_flag_once;
        int renderer_flag_once, flag_once_dead;
        int timer_flag_hunger, timer_flag_mate, timer_flag_rest;

        //info
        float info_distance_travelled_px, info_vision_px, info_prey_consumed_temp;
        int info_distance_travelled_m, info_prey_consumed;
        int info_mate_partners;
        int info_generation;
        char cause_of_death[50];
        int cause_of_death_calc; // 1:starve 2:no mate 3:stuck

        Fox(){
            // Spawn
            location_x = random_number(40, SCREEN_WIDTH - 256);
            location_y = random_number(40, 740);
            height = 32;width = 32;
            fox_rect.w = 32; fox_rect.h = 32;
            fox_rect.x = location_x; fox_rect.y = location_y; 

            // Movement
            timer_movement = SDL_GetTicks();
            time_wait = 0;
            move_x = random_number(0, 1);
            move_y = random_number(0, 1);
            velx = random_number(-2, 2);
            vely = random_number(-2, 2);

            // Traits
            gender = random_number(0, 1);
            vision_radius = random_number(80, 128);           
            hunger_level = random_number(70, 100);
            rest_level = random_number(70, 100);
            mate_level = random_number(70, 100);
            hunger_level_max = hunger_level;
            rest_level_max = rest_level;
            mate_level_max = mate_level;
            hungry = false;rest = false; mate = false;
            fox_surface[0] = IMG_Load("images/fox_2.png");
            fox_surface[1] = IMG_Load("images/fox_1.png");
            fox_surface[2] = IMG_Load("images/fox_2_dead.png");
            fox_surface[3] = IMG_Load("images/fox_1_dead.png");
            info_bar_surface[0] = IMG_Load("images/hungry_fox.png");
            info_bar_surface[1] = IMG_Load("images/rest.png");
            info_bar_surface[2] = IMG_Load("images/mate.png");
            
            // Activities
            is_dead = false;
            target_found_rabbit = false;
            flag_once_eat = 1; timer_eat;    
            found_partner = false;
            match = false;
            renderer_flag_once = 1;
            is_kit = false;kit_born = false;
            timer_growth = SDL_GetTicks();
            flag_once_dead = 1;
            timer_flag_rest = 0;
            spawn_flag_once = 1;
            
            // info
            info_distance_travelled_px = 0;
            info_distance_travelled_m = 0;
            info_mate_partners = 0;
            info_prey_consumed = 0;
            info_vision_px = 0;
            info_vision_px += (abs(vision_radius) * 0.020);
            info_generation = 1;
            strcpy(cause_of_death, "Still Alive");
        }

        void info(){
            std::cout<<"Generation: "<<info_generation;
            std::cout<<"\tDistance Travelled: "<<info_distance_travelled_m<<"m";
            std::cout<<"\tTime Alive: "<<time_alive;
            std::cout<<"\tMate Partners: "<<info_mate_partners;
            std::cout<<"\tPrey Consumed: "<<info_prey_consumed;
            std::cout<<"\tVision Radius: "<<info_vision_px<<"m";
            std::cout<<"\tCause Of Death: "<<cause_of_death<<std::endl;
        }

        void render(SDL_Renderer *renderer, int visibility){   
            info_prey_consumed = int(info_prey_consumed_temp);
            info_distance_travelled_m = int(info_distance_travelled_px);
  
            if(!is_kit){
                fox_rect = {location_x ,location_y, width, height};
            }
            if(is_kit){
                fox_rect = {location_x ,location_y, width, height};
                mate_level = mate_level_max;
            }
            fox_vision = {location_x - (vision_radius - width)/2, location_y - (vision_radius - height)/2, vision_radius, vision_radius};
            hunger_bar_rect = {location_x - 8, location_y - 17, 16, 16};
            rest_bar_rect = {location_x + 8 ,location_y - 17, 16, 16};
            mate_bar_rect = {location_x + 24 ,location_y - 17, 16, 16};
            if(gender == 1)
                    SDL_SetRenderDrawColor(renderer, 251, 146, 43, 225);

            if(gender == 0)
                    SDL_SetRenderDrawColor(renderer, 119, 52, 33, 225);


            if(renderer_flag_once == 1){
                fox_texture = SDL_CreateTextureFromSurface(renderer, fox_surface[gender]);
                if(gender == 1){
                    fox_dead_texture = SDL_CreateTextureFromSurface(renderer, fox_surface[3]);
                }
                if(gender == 0)
                    fox_dead_texture = SDL_CreateTextureFromSurface(renderer, fox_surface[2]);
                renderer_flag_once += 1;
                SDL_FreeSurface(fox_surface[0]);
                SDL_FreeSurface(fox_surface[1]);
                SDL_FreeSurface(fox_surface[2]);
                SDL_FreeSurface(fox_surface[3]);
            }

            if(is_dead){
                SDL_RenderCopy(renderer, fox_dead_texture, NULL, &fox_rect);
            }
            if(!is_dead){
                SDL_RenderCopy(renderer, fox_texture, NULL, &fox_rect);
                if(visibility)
                    SDL_RenderDrawRect(renderer, &fox_vision);
            }
        }

        void movement(int current_timer){ // Hopping simulation
            if(!wait ){
                if (current_timer - timer_movement < (random_number(1, 3)*random_number(800, 2500))){
                    if(!match){
                    location_x += velx;
                    location_y += vely;
                    if(velx > vely)
                        info_distance_travelled_px += (abs(velx) * 0.020);
                    else
                        info_distance_travelled_px += (abs(vely) * 0.020);
                    }
                }
                else{
                    time_wait = SDL_GetTicks();
                    move_x = random_number(0, 1);
                    move_y = random_number(0, 1);
                    wait = true;
                }         
            }  

            if(wait){
                if (current_timer - time_wait < 500){
                    location_x += 0;
                    location_y += 0;
                }
                else{
                    timer_movement = SDL_GetTicks();
                    move_x = random_number(0, 1);
                    move_y = random_number(0, 1);
                    velx = random_number(-2, 2);
                    vely = random_number(-2, 2);
                    wait = false;    
                }
            }
        }

        void collision_detection_border(){
            if(location_x < 35){
                location_x += 1;
            }
            if(location_x > SCREEN_WIDTH - 256){
                location_x -= 1;
            }
            if(location_y > SCREEN_HEIGHT - 32){
                location_y -= 2;
            }
            if(location_y < 35){
                location_y += 1;
            }
        } 

        SDL_Rect get_rect_vision() const{
            return fox_vision;
        }
        SDL_Rect get_rect_fox() const{
            return fox_rect;
        }

        int eat(int current_timer, SDL_Rect rabbit_target){ // When rabbit finds grass
            if (hungry){
                if(target_found_rabbit){
                    wait = true;
                    if(flag_once_eat == 2){
                        if (fox_rect.x < rabbit_target.x - 5){
                            location_x += 1;
                        }
                        if (fox_rect.x > rabbit_target.x - 5){
                            location_x -= 1;
                        }
                        if(fox_rect.y < rabbit_target.y - 5){
                            location_y += 1;
                        }
                        if(fox_rect.y > rabbit_target.y - 5){
                            location_y -= 1;
                        }
                    }                    
                }
            }
        }

        void essentials(SDL_Renderer *renderer, int current_timer){
            // Freeing Surface
            if(renderer_flag_once == 2){
                info_hungry_texture = SDL_CreateTextureFromSurface(renderer, info_bar_surface[0]);
                SDL_FreeSurface(info_bar_surface[0]);
                info_rest_texture = SDL_CreateTextureFromSurface(renderer, info_bar_surface[1]);
                SDL_FreeSurface(info_bar_surface[1]);
                info_mate_texture = SDL_CreateTextureFromSurface(renderer, info_bar_surface[2]);
                SDL_FreeSurface(info_bar_surface[2]);
                renderer_flag_once += 1;
            }

            // Hunger
            if(hunger_level > 0.1f){
                hunger_level -= 0.080f;
            }
            else{

                //SDL_FreeSurface(info_bar_surface[0]);
                if(!is_dead)
                    SDL_RenderCopy(renderer, info_hungry_texture, NULL, &hunger_bar_rect);
                if(timer_flag_hunger == 0){
                    timer_low_hunger = SDL_GetTicks();
                    timer_flag_hunger += 1;
                }
                hungry = true;
            }
                   
            // Thirst 
            if(rest_level > 0.1f){
                if(timer_flag_rest == 0){
                    rest_level -= 0.080f;

                }

            }
            else {
                if(timer_flag_rest == 0){
                    timer_low_rest = SDL_GetTicks();
                    timer_flag_rest += 1;
                }
                rest = true;
            }

            if(rest){         
                if(!is_dead)
                    SDL_RenderCopy(renderer, info_rest_texture, NULL, &rest_bar_rect);
                wait = 1;
                if(rest_level < rest_level_max){
                    rest_level += 0.5;
                }   
                else{
                wait = false;
                rest = false;
                timer_flag_rest = 0;
                } 

            }
            

            // Mate
            if(mate_level > 0.1f){
                mate_level -= 0.080f;
            }

            else if(mate_level < 0.1f){
                if(timer_flag_mate == 0){
                    timer_low_mate = SDL_GetTicks();
                    timer_flag_mate += 1;
                }
                mate = true;
            }
            if(mate){
                if(current_timer - timer_low_mate > 60000){
                    if(timer_flag_mate == 1){
                        is_dead = true;
                        cause_of_death_calc = 2;
                    }
                }
                if(!is_dead)
                    SDL_RenderCopy(renderer, info_mate_texture, NULL, &mate_bar_rect);
                if(match){
                    if(mate_level < mate_level_max)
                        mate_level += 1.6f;
                    else{
                        if(gender == 1){
                            is_pregnant = true;
                            timer_pregnancy = SDL_GetTicks();
                        }
                        match = false;
                        wait = false;
                        mate = false;
                        info_mate_partners += 1;
                        found_partner = false;

                    }
                }
            }


        }

        void find_mate(){                
            if(found_partner){
                if (SDL_HasIntersection(&fox_rect, &target_mate)){
                    match = true;
                }
                // wait = true;
                if (fox_rect.x < target_mate.x - 32){
                    location_x += 1;
                }
                if (fox_rect.x > target_mate.x - 32){
                    location_x -= 1;
                }
                if(fox_rect.y < target_mate.y){
                    location_y += 1;
                }
                if(fox_rect.y > target_mate.y){
                    location_y -= 1;
                }
            }
        }

        void pregnancy(int current_time){
            if(is_pregnant){
                mate_level = mate_level_max;
                if(current_time - timer_pregnancy > 30000){
                    kit_born = true;
                    is_pregnant = false;
                }
            }
        }

        void growth(int current_timer){
            if(!is_dead)
                time_alive = (current_timer - timer_growth)/1000;
            if(is_kit && (time_alive > 30) && height <= 31){
                width += 1;
                height += 1;
                vision_radius = traits[0];
                hunger_level_max = traits[1];
                rest_level_max = traits[2];
                mate_level_max = traits[3];
                if(height == 31){
                    is_kit = false;
                }

            }
        }
        
        int dead_cooldown(int current_time){
            if(is_dead){
                if(flag_once_dead == 1){
                    timer_dead = SDL_GetTicks();
                    flag_once_dead += 1;
                }

                if(cause_of_death_calc == 1){
                    strcpy(cause_of_death, "Starvation");
                    mate_level = mate_level_max;
                }
                if(cause_of_death_calc == 2){
                    strcpy(cause_of_death, "No Mate");
                    hunger_level = hunger_level_max;
                }
                if(cause_of_death_calc == 3){
                    hunger_level = hunger_level_max;
                    mate_level = mate_level_max;
                    strcpy(cause_of_death, "Removed By Player");
                }

                if(current_time - timer_dead > 10000){
                    fox_rect.x = -32;
                    return 1;
                }
            }
            return 0;
        }

        void collision_detection_water(SDL_Rect water){
            if(SDL_HasIntersection(&fox_rect, &water)){
                if (fox_rect.x < water.x - 1){
                    location_x -= 1;
                }
                if (fox_rect.x > water.x - 1){
                    location_x += 1;
                }
                if(fox_rect.y < water.y - 1){
                    location_y -= 1;
                }
                if(fox_rect.y > water.y - 1){
                    location_y += 1;
                }
            }
        }

        void spawn(SDL_Renderer *renderer, int current_timer){
            if(spawn_flag_once == 1){
                timer_spawn = SDL_GetTicks();
                spawn_flag_once += 1;
            }
            if(spawn_flag_once == 3){
                if (current_timer - timer_spawn < 3000){
                    SDL_Rect spawn_rect = {location_x, location_y, 32, 32};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &spawn_rect);
                }
            }

        }
        
};

#endif 