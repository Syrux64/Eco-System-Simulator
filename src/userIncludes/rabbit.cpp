#ifndef RABBIT_CPP
#define RABBIT_CPP

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include<iostream>
#include<string.h>

#include "random_number.h"
#include "globals.h"


// Classes -->
class Rabbit{
    protected:
        float movement_speed = 3.5;
        int velx, vely;
        SDL_Surface *rabbit_surface[4];
        SDL_Surface *info_bar_surface[3];
        SDL_Rect hunger_bar_rect, thirst_bar_rect, mate_bar_rect;

    public:  

        // Essentials
        bool is_dead;
        int traits[4]; //[vision, hunger, thirst, mate]
        float hunger_level = random_number(70, 100);
        bool target_found_grass = false, hungry, thirst, mate;
        bool is_kit;
        bool found_partner;
        bool match;
        bool wait = false;
        float hunger_level_max = hunger_level;
        float thirst_level, thirst_level_max;
        float mate_level, mate_level_max;
        int vision_radius;
        int gender; // 0:male / 1:female
        int width, height;
        // SDL
        SDL_Rect rabbit_rect, rabbit_vision, target_mate;
        SDL_Texture *rabbit_texture, *rabbit_dead_texture;
        SDL_Texture *info_hungry_texture, *info_thirst_texture, *info_mate_texture;
        int timer_flag_hunger, timer_flag_thirst, timer_flag_mate;
        int timer_low_hunger, timer_low_thirst, timer_low_mate, timer_dead;
        int timer_movement, time_wait, timer_pregnancy, timer_growth, time_alive, timer_spawn;
        int location_x, location_y, destination_x, destination_y, move_x, move_y;
        bool is_pregnant;bool kit_born;
        float amp;float freq;

        // flags (for running only once)
        int renderer_flag_once, flag_once_dead;
        int flag_once_eat = 1, timer_eat, flag_once_mate = 1, spawn_flag_once;

        //info
        float info_distance_travelled_px, info_vision_px;
        int info_distance_travelled_m, info_grasses_consumed;
        int info_mate_partners;
        int info_generation;
        char cause_of_death[50];
        int cause_of_death_calc; // 1:starve 2:thirst 3:no mate 4:eaten by fox 5:drown 6: stuck

        Rabbit(){
            // Spawn
            location_x = random_number(40, SCREEN_WIDTH - 256);
            location_y = random_number(40, 740);
            height = 32;width = 32;

            rabbit_rect.w = 32; rabbit_rect.h = 32;
            rabbit_rect.x = location_x; rabbit_rect.y = location_y; 

            // Movement
            timer_movement = SDL_GetTicks();
            time_wait = 0;
            move_x = random_number(0, 1);
            move_y = random_number(0, 1);
            velx = random_number(-2, 2);
            vely = random_number(-2, 2);
            is_pregnant = false;

            // Traits
            gender = random_number(0, 1);
            vision_radius = random_number(80, 128);           
            hunger_level = random_number(70, 100);
            thirst_level = random_number(70, 100);
            mate_level = random_number(70, 100);
            hunger_level_max = hunger_level;
            thirst_level_max = thirst_level;
            mate_level_max = mate_level;
            hungry = false;thirst = false; mate = false;

            // Image Rendering
            rabbit_surface[0] = IMG_Load("images/rabbit_2.png");
            rabbit_surface[1] = IMG_Load("images/rabbit_1.png");
            info_bar_surface[0] = IMG_Load("images/hungry.png");
            info_bar_surface[1] = IMG_Load("images/thirst.png");
            info_bar_surface[2] = IMG_Load("images/mate.png");
            rabbit_surface[2] = IMG_Load("images/rabbit_2_dead.png");
            rabbit_surface[3] = IMG_Load("images/rabbit_1_dead.png");
            
            // Activities
            timer_flag_hunger = timer_flag_thirst = timer_flag_mate = 0;
            is_dead = false;
            target_found_grass = false;
            flag_once_eat = 1; timer_eat;    
            found_partner = false;
            match = false;
            renderer_flag_once = 1;flag_once_dead = 1;
            is_kit = false;kit_born = false;
            timer_growth = SDL_GetTicks();
            
            // info
            info_distance_travelled_px = 0;
            info_distance_travelled_m = 0;
            info_mate_partners = 0;
            info_grasses_consumed = 0;
            info_vision_px = 0;
            info_vision_px += (abs(vision_radius) * 0.020);
            info_generation = 1;
            spawn_flag_once = 1;
            strcpy(cause_of_death, "Still Alive");
        }

        void info(){
            std::cout<<"Generation: "<<info_generation;
            std::cout<<"\tDistance Travelled: "<<info_distance_travelled_m<<"m";
            std::cout<<"\tTime Alive: "<<time_alive;
            std::cout<<"\tMate Partners: "<<info_mate_partners;
            std::cout<<"\tGrasses Consumed: "<<info_grasses_consumed;
            std::cout<<"\tVision Radius: "<<info_vision_px<<"m";
            std::cout<<"\tCause Of Death: "<<cause_of_death<<std::endl;
        }

        void render(SDL_Renderer *renderer, int visibility){     
            if(!is_kit){
                rabbit_rect = {location_x ,location_y, width, height};
            }
            if(is_kit){
                rabbit_rect = {location_x ,location_y, width, height};
                mate_level = mate_level_max;
            }
            rabbit_vision = {location_x - (vision_radius - width)/2, location_y - (vision_radius - height)/2, vision_radius, vision_radius};
            hunger_bar_rect = {location_x - 8, location_y - 17, 16, 16};
            thirst_bar_rect = {location_x + 8 ,location_y - 17, 16, 16};
            mate_bar_rect = {location_x + 24 ,location_y - 17, 16, 16};

            if(gender == 1)
                SDL_SetRenderDrawColor(renderer, 225, 225, 225, 225);
            else if(gender == 0)
                SDL_SetRenderDrawColor(renderer, 184, 111, 80, 225);

            if(renderer_flag_once == 1){
                rabbit_texture = SDL_CreateTextureFromSurface(renderer, rabbit_surface[gender]);
                if(gender == 1){
                    rabbit_dead_texture = SDL_CreateTextureFromSurface(renderer, rabbit_surface[3]);
                }
                if(gender == 0){
                    rabbit_dead_texture = SDL_CreateTextureFromSurface(renderer, rabbit_surface[2]);
                }
                SDL_FreeSurface(rabbit_surface[0]);
                SDL_FreeSurface(rabbit_surface[1]);
                SDL_FreeSurface(rabbit_surface[2]);
                SDL_FreeSurface(rabbit_surface[3]);
                renderer_flag_once += 1;
            }

            if(is_dead)
                SDL_RenderCopy(renderer, rabbit_dead_texture, NULL, &rabbit_rect);
            if(!is_dead){
                SDL_RenderCopy(renderer, rabbit_texture, NULL, &rabbit_rect);
                if(visibility)
                    SDL_RenderDrawRect(renderer, &rabbit_vision);
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
                    info_distance_travelled_m = int(info_distance_travelled_px);
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
                if (current_timer - time_wait < 700){
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
            if(location_x > SCREEN_WIDTH - 128){ // Drown
                is_dead = true;
                cause_of_death_calc = 5;
            }
            if(location_y > SCREEN_HEIGHT - 32){
                location_y -= 2;
            }
            if(location_y < 35){
                location_y += 1;
            }
        } 

        SDL_Rect get_rect_vision() const{
            return rabbit_vision;
        }
        SDL_Rect get_rect_rabbit() const{
            return rabbit_rect;
        }

        int eat(int current_timer, SDL_Rect grass_target){ // When rabbit finds grass
            if (hungry){
                if(target_found_grass){
                    wait = true;
                    if(flag_once_eat == 2){
                        if (rabbit_rect.x < grass_target.x - 5){
                            location_x += 1;
                        }
                        if (rabbit_rect.x > grass_target.x - 5){
                            location_x -= 1;
                        }
                        if(rabbit_rect.y < grass_target.y - 5){
                            location_y += 1;
                        }
                        if(rabbit_rect.y > grass_target.y - 5){
                            location_y -= 1;
                        }
                    }                    
                }
            }
        }

        void essentials(SDL_Renderer *renderer, SDL_Rect lake, SDL_Rect pond, int current_timer){
            // Freeing Surface
            if(renderer_flag_once == 2){
                info_hungry_texture = SDL_CreateTextureFromSurface(renderer, info_bar_surface[0]);
                SDL_FreeSurface(info_bar_surface[0]);
                info_thirst_texture = SDL_CreateTextureFromSurface(renderer, info_bar_surface[1]);
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
            if(thirst_level > 0.1f){
                thirst_level -= 0.080f;
            }
            else if(thirst_level < 0.1f){
                if(timer_flag_thirst == 0){
                    timer_low_thirst = SDL_GetTicks();
                    timer_flag_thirst += 1;
                }
                thirst = true;
            }

            if(thirst){         
                if(current_timer - timer_low_thirst > 60000){
                    if(timer_flag_mate == 1){
                        is_dead = true;
                        cause_of_death_calc = 2;
                    }
                }
                if(!is_dead)
                    SDL_RenderCopy(renderer, info_thirst_texture, NULL, &thirst_bar_rect);

                if(SDL_HasIntersection(&rabbit_vision, &lake)){
                    wait = 1;
                    if(!(SDL_HasIntersection(&rabbit_rect, &lake)))
                        location_x += 1;
                        if(thirst_level < thirst_level_max){
                            thirst_level += 0.6;
                        }   
                        else{
                        wait = false;
                        thirst = false;
                        timer_flag_thirst = 0;
                        }                        
                    }
                if(SDL_HasIntersection(&rabbit_vision, &pond)){
                    wait = 1;
                    if (rabbit_rect.x < pond.x - 5){
                        location_x += 1;
                    }
                    if (rabbit_rect.x > pond.x - 5){
                        location_x -= 1;
                    }
                    if(rabbit_rect.y < pond.y - 5){
                        location_y += 1;
                    }
                    if(rabbit_rect.y > pond.y - 5){
                        location_y -= 1;
                    }
                    if(thirst_level < thirst_level_max){
                        thirst_level += 0.6;
                    }   
                    else{
                        wait = false;
                        thirst = false;
                        timer_flag_thirst = 0;
                    } 
                }

            }

            // Mate
            if(mate_level > 0.1f)
                mate_level -= 0.080f;
            
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
                        cause_of_death_calc = 3;
                    }
                }

                if(!is_dead)
                    SDL_RenderCopy(renderer, info_mate_texture, NULL, &mate_bar_rect);

                if(match){
                    if(mate_level < mate_level_max)
                        mate_level += 0.6f;
                    else{
                        if(gender == 1){
                            is_pregnant = true;
                            timer_pregnancy = SDL_GetTicks();
                        }
                        match = false;
                        wait = false;
                        mate = false;
                        found_partner = false;
                        timer_flag_mate = 0;
                        info_mate_partners += 1;
                        flag_once_mate = 1;

                    }
                }
            }
        }

        void find_mate(){
            if(found_partner){
                if (SDL_HasIntersection(&rabbit_rect, &target_mate)){
                    match = true;
                }
                if (rabbit_rect.x < target_mate.x - 32){
                    location_x += 1;
                }
                if (rabbit_rect.x > target_mate.x - 32){
                    location_x -= 1;
                }
                if(rabbit_rect.y < target_mate.y){
                    location_y += 1;
                }
                if(rabbit_rect.y > target_mate.y){
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
                thirst_level_max = traits[2];
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
                    thirst_level = thirst_level_max;
                    mate_level = mate_level_max;
                    strcpy(cause_of_death, "Starvation");
                }
                if(cause_of_death_calc == 2){
                    hunger_level = hunger_level_max;
                    mate_level = mate_level_max;
                    strcpy(cause_of_death, "Thirst");
                }
                if(cause_of_death_calc == 3){
                    hunger_level = hunger_level_max;
                    thirst_level = thirst_level_max; 
                    strcpy(cause_of_death, "No Mate");
                }
                if(cause_of_death_calc == 4){
                    hunger_level = hunger_level_max;
                    thirst_level = thirst_level_max; 
                    mate_level = mate_level_max;
                    strcpy(cause_of_death, "Eaten By Fox");
                }
                if(cause_of_death_calc == 5){
                    hunger_level = hunger_level_max;
                    thirst_level = thirst_level_max; 
                    mate_level = mate_level_max;
                    strcpy(cause_of_death, "Drown");
                }
                if(cause_of_death_calc == 6){
                    hunger_level = hunger_level_max;
                    thirst_level = thirst_level_max; 
                    mate_level = mate_level_max;
                    strcpy(cause_of_death, "Removed By Player");
                }
                
                if(current_time - timer_dead > 10000){
                    rabbit_rect.x = -32;
                    return 1;
                }
            }
            return 0;
        }
        
        void collision_detection_water(SDL_Rect water){

            if(SDL_HasIntersection(&rabbit_rect, &water)){

                if (rabbit_rect.x < water.x - 1){
                    location_x -= 1;
                }
                if (rabbit_rect.x > water.x - 1){
                    location_x += 1;
                }
                if(rabbit_rect.y < water.y - 1){
                    location_y -= 1;
                }
                if(rabbit_rect.y > water.y - 1){
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