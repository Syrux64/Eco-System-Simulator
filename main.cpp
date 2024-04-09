#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<conio.h>

#include "src/userIncludes/random_number.h"
#include "src/userIncludes/globals.h"
#include "src/userIncludes/rabbit.cpp"
#include "src/userIncludes/grass.cpp"
#include "src/userIncludes/fox.cpp"
#include "src/userIncludes/pond.cpp"
#include "src/userIncludes/lake.cpp"




int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int total_grasses = 0;
int total_rabbits = 0;
int total_foxes = 0;


// Main -->
int main(int argc, char* _argv[]){
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    int windows[] = {1, 2, 3}; // 1: entry screen 2: game screen 3: extras 
    int current_window = 1;

    int clock = SDL_GetTicks();
    int current_time = 0;

    // Window Resolution

    char ch;
    choice:
        std::cout<<"1.1024x768\n2.1280x768\n3.1360x768\n";
        std::cout<<"Enter Your Screen Resolution:";
        std::cin>>ch;
        switch (ch){
            case '1':
                SCREEN_WIDTH = 1024;
                SCREEN_HEIGHT = 768;
                break;
            case '2':
                SCREEN_WIDTH = 1280;
                SCREEN_HEIGHT = 768;
                break;    
            case '3':
                SCREEN_WIDTH = 1360;
                SCREEN_HEIGHT = 768;
                break;      
            default:
                std::cout<<"Invalid Choice\n";
                goto choice;
                break;
        }


   // Window Setup
    SDL_Window* window = SDL_CreateWindow("Eco System Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *icon = IMG_Load("images/rabbit_1.png");
    SDL_SetWindowIcon(window, icon);

    const Uint8 *keyboardstate = SDL_GetKeyboardState(NULL);

    int mouse_x, mouse_y;
    int render_menu_flag = 1;

    SDL_Event event;
    bool flag = true;

    // Instantiation
    Rabbit rabbits[100];
    Grass grasses[300];
    Fox foxes[50];
    Lake lake(renderer);
    Pond pond(renderer);

    Rabbit eye_candy_r[5];
    Fox eye_candy_f[5];

    int grow_grass = SDL_GetTicks(); // For growing more grasses

    // Clear unwanted collision before showing the main window
    int temp = 1;
    while(temp < 11){
        for(int i=0;i<300;i++){
            if(SDL_HasIntersection(&grasses[i].grass_rect, &pond.pond_rect_1)){
                if (grasses[i].grass_rect.x < pond.pond_rect_1.x ){
                    grasses[i].grass_rect.x -= 10;
                }
                if (grasses[i].grass_rect.x > pond.pond_rect_1.x){
                    grasses[i].grass_rect.x += 10;
                }
                if(grasses[i].grass_rect.y < pond.pond_rect_1.y ){
                    grasses[i].grass_rect.y -= 10;
                }
                if(grasses[i].grass_rect.y > pond.pond_rect_1.y ){
                    grasses[i].grass_rect.y += 10;
                }
            }
            
            if(SDL_HasIntersection(&grasses[i].grass_rect, &pond.pond_rect_2)){
                if (grasses[i].grass_rect.x < pond.pond_rect_2.x ){
                    grasses[i].grass_rect.x -= 10;
                }
                if (grasses[i].grass_rect.x > pond.pond_rect_2.x ){
                    grasses[i].grass_rect.x += 10;
                }
                if(grasses[i].grass_rect.y < pond.pond_rect_2.y ){
                    grasses[i].grass_rect.y -= 10;
                }
                if(grasses[i].grass_rect.y > pond.pond_rect_2.y - 1){
                    grasses[i].grass_rect.y += 10;
                }
            }            
        }
        temp ++;

    }

    int visibility = 0;
    bool is_visibility_checked = false;
    bool visibility_pressed = false;


    // intro sine wave
    float amplitude = 5.0f; 
    float frequency = 0.1f; 
    float time_s = 0.0f; 

    eye_candy_r[0].amp = 13.14f;eye_candy_r[0].freq = 0.1;
    eye_candy_r[1].amp = 11.25f;eye_candy_r[1].freq = 0.1;
    eye_candy_r[2].amp = 17.1f;eye_candy_r[2].freq = 0.1;
    eye_candy_r[3].amp = 14.3f;eye_candy_r[3].freq = 0.1;
    eye_candy_r[4].amp = 14.8f;eye_candy_r[4].freq = 0.1;

    eye_candy_f[0].amp = 14.1f;eye_candy_f[0].freq = 0.1;
    eye_candy_f[1].amp = 13.6f;eye_candy_f[1].freq = 0.1;
    eye_candy_f[2].amp = 18.1f;eye_candy_f[2].freq = 0.1;
    eye_candy_f[3].amp = 16.2f;eye_candy_f[3].freq = 0.1;
    eye_candy_f[4].amp = 11.4f;eye_candy_f[4].freq = 0.1;

    for(int i=0;i<5;i++){
        eye_candy_r[i].location_x = random_number(0, 1280);
        eye_candy_f[i].location_x = random_number(0, 1280);

    }


    // Window Loop
    while (flag){
        // Entry Screen
        if(current_window  == 1){
            if(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    flag = false;
                }
            }

            // For rendering images -->
            SDL_Surface *option_surface_play, *option_surface_help, *option_surface_devs, *option_surface_version, *option_surface_name,
                        *option_surface_exit;
            SDL_Surface *option_surface_play_hover, *option_surface_help_hover, *option_surface_exit_hover;

            SDL_Texture *option_texture_play, *option_texture_help, *option_texture_devs, *option_texture_version, *option_texture_name,
                        *option_texture_exit;
            SDL_Texture *option_texture_play_hover, *option_texture_help_hover, *option_texture_exit_hover;
            if(render_menu_flag == 1){
                option_surface_play = IMG_Load("images/play.png");
                option_texture_play = SDL_CreateTextureFromSurface(renderer, option_surface_play);
                option_surface_play_hover = IMG_Load("images/play_hover.png");
                option_texture_play_hover = SDL_CreateTextureFromSurface(renderer, option_surface_play_hover);

                option_surface_help = IMG_Load("images/help.png");
                option_texture_help = SDL_CreateTextureFromSurface(renderer, option_surface_help);
                option_surface_help_hover = IMG_Load("images/help_hover.png");
                option_texture_help_hover = SDL_CreateTextureFromSurface(renderer, option_surface_help_hover);

                option_surface_devs = IMG_Load("images/devs.png");
                option_texture_devs = SDL_CreateTextureFromSurface(renderer, option_surface_devs);
                option_surface_version = IMG_Load("images/version.png");
                option_texture_version = SDL_CreateTextureFromSurface(renderer, option_surface_version);
                option_surface_name = IMG_Load("images/name.png");
                option_texture_name = SDL_CreateTextureFromSurface(renderer, option_surface_name);

                option_surface_exit = IMG_Load("images/exit.png");
                option_texture_exit = SDL_CreateTextureFromSurface(renderer, option_surface_exit);
                option_surface_exit_hover = IMG_Load("images/exit_hover.png");
                option_texture_exit_hover = SDL_CreateTextureFromSurface(renderer, option_surface_exit_hover);

                SDL_FreeSurface(option_surface_play);SDL_FreeSurface(option_surface_help);SDL_FreeSurface(option_surface_devs);
                SDL_FreeSurface(option_surface_version);SDL_FreeSurface(option_surface_name);SDL_FreeSurface(option_surface_exit);
                SDL_FreeSurface(option_surface_exit_hover);SDL_FreeSurface(option_surface_help_hover);SDL_FreeSurface(option_surface_play_hover);
                render_menu_flag = 1;
            }

            const Uint32 mouse_pos = SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};


            SDL_SetRenderDrawColor(renderer, 112, 174, 110, 255);
            SDL_RenderClear(renderer);

            SDL_Rect backgroud = {1280, 768, 0, 0};
            SDL_RenderFillRect(renderer, &backgroud);


            // make animals move in sine wave
            for(int i=0;i<5;i++){
                float y = eye_candy_r[i].amp * sin(eye_candy_r[i].freq * time_s);
                float y_ = eye_candy_f[i].amp * sin(eye_candy_f[i].freq * time_s);
                eye_candy_r[i].render(renderer, 0);
                eye_candy_f[i].render(renderer, 0);
                eye_candy_r[i].location_x += random_number(2, 4);
                eye_candy_r[i].location_y += y;
                eye_candy_f[i].location_x += random_number(2, 4);
                eye_candy_f[i].location_y += y_;

                if(eye_candy_r[i].location_x > SCREEN_WIDTH)
                    eye_candy_r[i].location_x = 0;
                if(eye_candy_r[i].location_y > SCREEN_HEIGHT)
                    eye_candy_r[i].location_y = 0;  
                if(eye_candy_r[i].location_y < 0)
                    eye_candy_r[i].location_y = SCREEN_HEIGHT; 
                      
                if(eye_candy_f[i].location_x > SCREEN_WIDTH)
                    eye_candy_f[i].location_x = 0;
                if(eye_candy_f[i].location_y > SCREEN_HEIGHT)
                    eye_candy_f[i].location_y = 0;  
                if(eye_candy_f[i].location_y < 0)
                    eye_candy_f[i].location_y = SCREEN_HEIGHT;   
            }
            time_s += 0.05;

            SDL_SetRenderDrawColor(renderer, 87, 0, 0, 255);

            // Place Button -->
            SDL_Rect name = {(SCREEN_WIDTH - 256)/2, ((SCREEN_HEIGHT - 128)/2) - 190, 256, 128};
            SDL_RenderCopy(renderer, option_texture_name, NULL, &name);

            SDL_Rect play = {(SCREEN_WIDTH - 128)/2, ((SCREEN_HEIGHT - 64)/2) - 32, 128, 64};
            SDL_RenderCopy(renderer, option_texture_play, NULL, &play);
            if(SDL_HasIntersection(&mouse, &play))
                SDL_RenderCopy(renderer, option_texture_play_hover, NULL, &play);

            SDL_Rect help = {(SCREEN_WIDTH - 128)/2, ((SCREEN_HEIGHT - 64)/2) + 48, 128, 64};
            SDL_RenderCopy(renderer, option_texture_help, NULL, &help);
            if(SDL_HasIntersection(&mouse, &help))
                SDL_RenderCopy(renderer, option_texture_help_hover, NULL, &help);
            
            SDL_Rect exit = {(SCREEN_WIDTH - 128)/2, ((SCREEN_HEIGHT - 64)/2) + 128, 128, 64};
            SDL_RenderCopy(renderer, option_texture_exit, NULL, &exit);
            if(SDL_HasIntersection(&mouse, &exit))
                SDL_RenderCopy(renderer, option_texture_exit_hover, NULL, &exit);

            SDL_Rect devs = {(SCREEN_WIDTH - 256), (SCREEN_HEIGHT - 32), 256, 32};
            SDL_RenderCopy(renderer, option_texture_devs, NULL, &devs);

            SDL_Rect version = {0, (SCREEN_HEIGHT - 32), 128, 32};
            SDL_RenderCopy(renderer, option_texture_version, NULL, &version);

            if((SDL_HasIntersection(&mouse, &play)) && (mouse_pos &SDL_BUTTON_LEFT)){
                SDL_DestroyTexture(option_texture_play);SDL_DestroyTexture(option_texture_help);SDL_DestroyTexture(option_texture_devs);
                SDL_DestroyTexture(option_texture_help_hover);SDL_DestroyTexture(option_texture_name);SDL_DestroyTexture(option_texture_exit);
                SDL_DestroyTexture(option_texture_play_hover);SDL_DestroyTexture(option_texture_exit_hover);SDL_DestroyTexture(option_texture_version);
                current_window = 2;
                render_menu_flag = 2;
            }

            if((SDL_HasIntersection(&mouse, &help)) && (mouse_pos &SDL_BUTTON_LEFT)){
                SDL_DestroyTexture(option_texture_play);SDL_DestroyTexture(option_texture_help);SDL_DestroyTexture(option_texture_devs);
                SDL_DestroyTexture(option_texture_help_hover);SDL_DestroyTexture(option_texture_name);SDL_DestroyTexture(option_texture_exit);
                SDL_DestroyTexture(option_texture_play_hover);SDL_DestroyTexture(option_texture_exit_hover);SDL_DestroyTexture(option_texture_version);
                current_window = 3;
                render_menu_flag = 3;
            }
            
            if((SDL_HasIntersection(&mouse, &exit)) && (mouse_pos &SDL_BUTTON_LEFT)){
                SDL_DestroyTexture(option_texture_play);SDL_DestroyTexture(option_texture_help);SDL_DestroyTexture(option_texture_devs);
                SDL_DestroyTexture(option_texture_help_hover);SDL_DestroyTexture(option_texture_name);SDL_DestroyTexture(option_texture_exit);
                SDL_DestroyTexture(option_texture_play_hover);SDL_DestroyTexture(option_texture_exit_hover);SDL_DestroyTexture(option_texture_version);
                flag = false;
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(1000/60);
        }

        // Help Menu
        if(current_window == 3){
            if(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    flag = false;
                }
            }

            const Uint32 mouse_pos = SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};

            SDL_Surface *ok_surface, *help_surface;
            SDL_Texture *ok_texture, *help_texture;
            SDL_Surface* ok_surface_hover;
            SDL_Texture* ok_texture_hover;

            if(render_menu_flag == 3){
                ok_surface = IMG_Load("images/ok.png");
                ok_texture = SDL_CreateTextureFromSurface(renderer, ok_surface);
                help_surface = IMG_Load("images/help_message.png");
                help_texture = SDL_CreateTextureFromSurface(renderer, help_surface);
                ok_surface_hover = IMG_Load("images/ok_hover.png");
                ok_texture_hover = SDL_CreateTextureFromSurface(renderer, ok_surface_hover);
                render_menu_flag = 10;
            }

            SDL_SetRenderDrawColor(renderer, 112, 174, 110, 255);
            SDL_RenderClear(renderer);

            SDL_Rect backgroud = {1280, 768, 0, 0};
            SDL_RenderFillRect(renderer, &backgroud);


            SDL_Rect ok = {(SCREEN_WIDTH - 128)/2, SCREEN_HEIGHT - 128 , 128, 64};
            SDL_RenderCopy(renderer, ok_texture, NULL, &ok);
            if(SDL_HasIntersection(&mouse, &ok))
                SDL_RenderCopy(renderer, ok_texture_hover, NULL, &ok);

            SDL_Rect message = {(SCREEN_WIDTH - 700)/2, SCREEN_HEIGHT - 650 , 700, 700};
            SDL_RenderCopy(renderer, help_texture, NULL, &message);

            if((SDL_HasIntersection(&mouse, &ok)) && (mouse_pos &SDL_BUTTON_LEFT)){
                SDL_DestroyTexture(ok_texture);SDL_DestroyTexture(help_texture);SDL_DestroyTexture(ok_texture_hover);

                current_window = 1;
                render_menu_flag = 1;

            }

            SDL_RenderPresent(renderer);
            SDL_Delay(1000/60);

        }

        // Pause Screen
        if(current_window == 4){
            if(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    flag = false;
                }
            }
            SDL_Surface *option_surface_paused, *option_surface_resume, *option_surface_end, *option_surface_exit;
            SDL_Texture *option_texture_paused, *option_texture_resume, *option_texture_end, *option_texture_exit;

            SDL_Surface *option_surface_resume_hover, *option_surface_back_hover, *option_surface_end_hover, *option_surface_exit_hover;
            SDL_Texture *option_texture_resume_hover, *option_texture_back_hover, *option_texture_end_hover, *option_texture_exit_hover;

            if(render_menu_flag == 4){
                option_surface_paused = IMG_Load("images/paused.png");
                option_texture_paused = SDL_CreateTextureFromSurface(renderer, option_surface_paused);
                
                option_surface_resume = IMG_Load("images/resume.png");
                option_texture_resume = SDL_CreateTextureFromSurface(renderer, option_surface_resume);
                option_surface_resume_hover = IMG_Load("images/resume_hover.png");
                option_texture_resume_hover = SDL_CreateTextureFromSurface(renderer, option_surface_resume_hover);

                option_surface_end = IMG_Load("images/end.png");
                option_texture_end = SDL_CreateTextureFromSurface(renderer, option_surface_end);
                option_surface_end_hover = IMG_Load("images/end_hover.png");
                option_texture_end_hover = SDL_CreateTextureFromSurface(renderer, option_surface_end_hover);

                option_surface_exit = IMG_Load("images/exit.png");
                option_texture_exit = SDL_CreateTextureFromSurface(renderer, option_surface_exit);
                option_surface_exit_hover = IMG_Load("images/exit_hover.png");
                option_texture_exit_hover = SDL_CreateTextureFromSurface(renderer, option_surface_exit_hover);

                SDL_FreeSurface(option_surface_paused); SDL_FreeSurface(option_surface_resume);
                SDL_FreeSurface(option_surface_end); SDL_FreeSurface(option_surface_exit);SDL_FreeSurface(option_surface_resume_hover);
                SDL_FreeSurface(option_surface_end_hover); SDL_FreeSurface(option_surface_exit_hover);

                render_menu_flag = 10;
            }

            const Uint32 mouse_pos = SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};

            SDL_SetRenderDrawColor(renderer, 112, 174, 110, 255);
            SDL_RenderClear(renderer);

            SDL_Rect backgroud = {1280, 768, 0, 0};
            SDL_RenderFillRect(renderer, &backgroud);

            SDL_SetRenderDrawColor(renderer, 87, 0, 0, 255);

            SDL_Rect paused = {(SCREEN_WIDTH - 256)/2, ((SCREEN_HEIGHT - 64)/2) - 190, 256, 64};
            SDL_RenderCopy(renderer, option_texture_paused, NULL, &paused);

            SDL_Rect resume = {(SCREEN_WIDTH - 128)/2, ((SCREEN_HEIGHT - 64)/2) - 64, 128, 64};
            SDL_RenderCopy(renderer, option_texture_resume, NULL, &resume);
            if(SDL_HasIntersection(&mouse, &resume))
                SDL_RenderCopy(renderer, option_texture_resume_hover, NULL, &resume);     

            SDL_Rect end = {(SCREEN_WIDTH - 128)/2, ((SCREEN_HEIGHT - 64)/2) + 10 , 128, 64};
            SDL_RenderCopy(renderer, option_texture_end, NULL, &end);
            if(SDL_HasIntersection(&mouse, &end))
                SDL_RenderCopy(renderer, option_texture_end_hover, NULL, &end);    
                        
            SDL_Rect exit = {(SCREEN_WIDTH - 128)/2, ((SCREEN_HEIGHT - 64)/2) + 85, 128, 64};
            SDL_RenderCopy(renderer, option_texture_exit, NULL, &exit);
            if(SDL_HasIntersection(&mouse, &exit))
                SDL_RenderCopy(renderer, option_texture_exit_hover, NULL, &exit);

            if((SDL_HasIntersection(&mouse, &resume)) && (mouse_pos &SDL_BUTTON_LEFT)){
                SDL_DestroyTexture(option_texture_paused);SDL_DestroyTexture(option_texture_resume);SDL_DestroyTexture(option_texture_end);
                SDL_DestroyTexture(option_texture_exit);SDL_DestroyTexture(option_texture_resume_hover);SDL_DestroyTexture(option_texture_back_hover);
                SDL_DestroyTexture(option_texture_end_hover);SDL_DestroyTexture(option_texture_exit_hover);
                current_window = 2;
                render_menu_flag = 2;
            }

            if((SDL_HasIntersection(&mouse, &end)) && (mouse_pos &SDL_BUTTON_LEFT)){
                SDL_DestroyTexture(option_texture_paused);SDL_DestroyTexture(option_texture_resume);SDL_DestroyTexture(option_texture_end);
                SDL_DestroyTexture(option_texture_exit);SDL_DestroyTexture(option_texture_resume_hover);SDL_DestroyTexture(option_texture_back_hover);
                SDL_DestroyTexture(option_texture_end_hover);SDL_DestroyTexture(option_texture_exit_hover);
                current_window = 5;
            }

            if((SDL_HasIntersection(&mouse, &exit)) && (mouse_pos &SDL_BUTTON_LEFT)){
                SDL_DestroyTexture(option_texture_paused);SDL_DestroyTexture(option_texture_resume);SDL_DestroyTexture(option_texture_end);
                SDL_DestroyTexture(option_texture_exit);SDL_DestroyTexture(option_texture_resume_hover);SDL_DestroyTexture(option_texture_back_hover);
                SDL_DestroyTexture(option_texture_end_hover);SDL_DestroyTexture(option_texture_exit_hover);
                flag = false;
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(1000/60);
        }

        
        // End Screen
        if(current_window == 5){
            flag = false;

            int highest_generation = rabbits[0].info_generation;
            int highest_partners = rabbits[0].info_mate_partners;
            int highest_consumed = rabbits[0].info_grasses_consumed;
            float highest_vision = rabbits[0].info_vision_px;
            int highest_cause_death = rabbits[0].cause_of_death_calc;
            int highest_time_alive = rabbits[0].time_alive;
            float most_distance_travelled = rabbits[0].info_distance_travelled_m;

            for(int i=0;i<total_rabbits;i++){
                if(highest_generation < rabbits[i].info_generation)
                    highest_generation = rabbits[i].info_generation;
                if(highest_partners < rabbits[i].info_mate_partners)
                    highest_partners = rabbits[i].info_mate_partners;
                if(highest_consumed < rabbits[i].info_grasses_consumed)
                    highest_consumed < rabbits[i].info_grasses_consumed;
                if(highest_vision < rabbits[i].info_vision_px)
                    highest_vision < rabbits[i].info_vision_px;
                if(highest_cause_death < rabbits[i].cause_of_death_calc)
                    highest_cause_death = rabbits[i].cause_of_death_calc;
                if(highest_time_alive < rabbits[i].time_alive)
                    highest_time_alive = rabbits[i].time_alive;
                if(most_distance_travelled < rabbits[i].info_distance_travelled_m)
                    most_distance_travelled = rabbits[i].info_distance_travelled_m;
            }

            std::cout<<"\n\t\t\tResults"<<std::endl;
            std::cout<<"Rabbits"<<std::endl;
            std::cout<<"======="<<std::endl;
            std::cout<<"Total Rabbits: "<<total_rabbits<<std::endl;
            std::cout<<"Highest Generation: "<<highest_generation<<std::endl;
            std::cout<<"Longest Time Alive: "<<highest_time_alive<<'s'<<std::endl;
            std::cout<<"Most Distance Travelled: "<<most_distance_travelled<<'m'<<std::endl;
            std::cout<<"Most Mate Partners: "<<highest_partners<<std::endl;
            std::cout<<"Most Grass Consumed: "<<highest_consumed<<std::endl;
            std::cout<<"Longest Vision Radius: "<<highest_vision<<'m'<<std::endl;
            std::cout<<"Most Cause Of Death: ";
            
            switch (highest_cause_death){
                case 1:
                    std::cout<<"Starvation";
                    break;
                
                case 2:
                    std::cout<<"Thirst";
                    break;
                case 3:
                    std::cout<<"No Mate";
                    break;
                case 4:
                    std::cout<<"Eaten By Fox";
                    break;
                case 5:
                    std::cout<<"Drown";
                    break;
                case 6:
                    std::cout<<"Removed By Player";
                    break;
                default:
                    std::cout<<"None";
                }
            std::cout<<"\n\n";

            for(int i=0;i<total_foxes;i++){
                if(highest_generation < foxes[i].info_generation)
                    highest_generation = foxes[i].info_generation;
                if(highest_partners < foxes[i].info_mate_partners)
                    highest_partners = foxes[i].info_mate_partners;
                if(highest_consumed < foxes[i].info_prey_consumed)
                    highest_consumed < foxes[i].info_prey_consumed;
                if(highest_vision < foxes[i].info_vision_px)
                    highest_vision < foxes[i].info_vision_px;
                if(highest_cause_death < foxes[i].cause_of_death_calc)
                    highest_cause_death = foxes[i].cause_of_death_calc;
                if(highest_time_alive < foxes[i].time_alive)
                    highest_time_alive = foxes[i].time_alive;
                if(most_distance_travelled < foxes[i].info_distance_travelled_m)
                    most_distance_travelled = foxes[i].info_distance_travelled_m;
            }

            std::cout<<"Foxes"<<std::endl;
            std::cout<<"======="<<std::endl;
            std::cout<<"Total Foxes: "<<total_foxes<<std::endl;
            std::cout<<"Highest Generation: "<<highest_generation<<std::endl;
            std::cout<<"Longest Time Alive: "<<highest_time_alive<<'s'<<std::endl;
            std::cout<<"Most Distance Travelled: "<<most_distance_travelled<<'m'<<std::endl;
            std::cout<<"Most Mate Partners: "<<highest_partners<<std::endl;
            std::cout<<"Most Prey Consumed: "<<highest_consumed<<std::endl;
            std::cout<<"Longest Vision Radius: "<<highest_vision<<'m'<<std::endl;
            std::cout<<"Most Cause Of Death: ";

            switch (highest_cause_death){
                case 1:
                    std::cout<<"Starvation";
                    break;
                
                case 2:
                    std::cout<<"No Mate";
                    break;
                case 3:
                    std::cout<<"Stuck";
                    break;
                default:
                    std::cout<<"None";

            }
            std::cout<<"\n\n\t\t=====End Of Simulation=====";

        }

        // Game loop Window
        if(current_window  == 2){
            current_time = SDL_GetTicks();    
            SDL_SetRenderDrawColor(renderer, 130, 205, 71, 255);
            SDL_RenderClear(renderer);

            // Water - main activites
            lake.render(renderer);
            pond.render(renderer);

            // Buttons 
            SDL_Surface *option_surface_pause_button, *option_surface_visible_button_on,*option_surface_visible_button_off,
                        *option_surface_spawn_rabbit, *option_surface_spawn_fox, *option_surface_spawn_grass;
            SDL_Texture *option_texture_pause_button, *option_texture_visible_button_on,*option_texture_visible_button_off,
                        *option_texture_spawn_rabbit, *option_texture_spawn_fox, *option_texture_spawn_grass;

            SDL_Surface *option_surface_pause_button_hover, *option_surface_visible_button_on_hover,*option_surface_visible_button_off_hover,
                        *option_surface_spawn_rabbit_hover, *option_surface_spawn_fox_hover, *option_surface_spawn_grass_hover;
            SDL_Texture *option_texture_pause_button_hover, *option_texture_visible_button_on_hover,*option_texture_visible_button_off_hover,
                        *option_texture_spawn_rabbit_hover, *option_texture_spawn_fox_hover, *option_texture_spawn_grass_hover;

            if(render_menu_flag == 2){
                option_surface_pause_button = IMG_Load("images/paused_button.png");
                option_texture_pause_button = SDL_CreateTextureFromSurface(renderer, option_surface_pause_button);
                option_surface_pause_button_hover = IMG_Load("images/paused_button_hover.png");
                option_texture_pause_button_hover = SDL_CreateTextureFromSurface(renderer, option_surface_pause_button_hover);               

                option_surface_visible_button_on = IMG_Load("images/visible_radius_on.png");
                option_texture_visible_button_on = SDL_CreateTextureFromSurface(renderer, option_surface_visible_button_on);
                option_surface_visible_button_on_hover = IMG_Load("images/visible_radius_on_hover.png");
                option_texture_visible_button_on_hover = SDL_CreateTextureFromSurface(renderer, option_surface_visible_button_on_hover);

                option_surface_visible_button_off = IMG_Load("images/visible_radius_off.png");
                option_texture_visible_button_off = SDL_CreateTextureFromSurface(renderer, option_surface_visible_button_off);
                option_surface_visible_button_off_hover = IMG_Load("images/visible_radius_off_hover.png");
                option_texture_visible_button_off_hover = SDL_CreateTextureFromSurface(renderer, option_surface_visible_button_off_hover);

                option_surface_spawn_rabbit = IMG_Load("images/spawn_rabbit.png");
                option_texture_spawn_rabbit = SDL_CreateTextureFromSurface(renderer, option_surface_spawn_rabbit);
                option_surface_spawn_rabbit_hover = IMG_Load("images/spawn_rabbit_hover.png");
                option_texture_spawn_rabbit_hover = SDL_CreateTextureFromSurface(renderer, option_surface_spawn_rabbit_hover);

                option_surface_spawn_fox = IMG_Load("images/spawn_fox.png");
                option_texture_spawn_fox = SDL_CreateTextureFromSurface(renderer, option_surface_spawn_fox);
                option_surface_spawn_fox_hover = IMG_Load("images/spawn_fox_hover.png");
                option_texture_spawn_fox_hover = SDL_CreateTextureFromSurface(renderer, option_surface_spawn_fox_hover);

                option_surface_spawn_grass = IMG_Load("images/spawn_grass.png");
                option_texture_spawn_grass = SDL_CreateTextureFromSurface(renderer, option_surface_spawn_grass);
                option_surface_spawn_grass_hover = IMG_Load("images/spawn_grass_hover.png");
                option_texture_spawn_grass_hover = SDL_CreateTextureFromSurface(renderer, option_surface_spawn_grass_hover);

                SDL_FreeSurface(option_surface_pause_button); SDL_FreeSurface(option_surface_visible_button_on);
                SDL_FreeSurface(option_surface_visible_button_off);SDL_FreeSurface(option_surface_spawn_fox);
                SDL_FreeSurface(option_surface_spawn_rabbit);SDL_FreeSurface(option_surface_spawn_grass);

                SDL_FreeSurface(option_surface_pause_button_hover); SDL_FreeSurface(option_surface_visible_button_on_hover);
                SDL_FreeSurface(option_surface_visible_button_off_hover); SDL_FreeSurface(option_surface_spawn_rabbit_hover);
                SDL_FreeSurface(option_surface_spawn_fox_hover);SDL_FreeSurface(option_surface_spawn_grass_hover);
                
                render_menu_flag += 10;
            }
            const Uint32 mouse_pos = SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};

            SDL_Rect pause_button = {(SCREEN_WIDTH - 64) - 43, ((SCREEN_HEIGHT - 64)/2) - 64, 64, 64};
            SDL_RenderCopy(renderer, option_texture_pause_button, NULL, &pause_button);
            if(SDL_HasIntersection(&mouse, &pause_button))
                SDL_RenderCopy(renderer, option_texture_pause_button_hover, NULL, &pause_button);  
            
            SDL_Rect visibility_button = {(SCREEN_WIDTH - 64) - 5, ((SCREEN_HEIGHT - 64)/2) + 15, 64, 64};
            if(visibility == 1)
                SDL_RenderCopy(renderer, option_texture_visible_button_on, NULL, &visibility_button);
                if(SDL_HasIntersection(&mouse, &visibility_button))
                    SDL_RenderCopy(renderer, option_texture_visible_button_on_hover, NULL, &visibility_button);
            if(visibility == 0)
                SDL_RenderCopy(renderer, option_texture_visible_button_off, NULL, &visibility_button);
                if(SDL_HasIntersection(&mouse, &visibility_button))
                    SDL_RenderCopy(renderer, option_texture_visible_button_off_hover, NULL, &visibility_button);
            
            SDL_Rect spawn_rabbit_rect = {(SCREEN_WIDTH - 64) - 5, ((SCREEN_HEIGHT - 64)/2) + 96, 64, 64};
            SDL_RenderCopy(renderer, option_texture_spawn_rabbit, NULL, &spawn_rabbit_rect);
            if(SDL_HasIntersection(&mouse, &spawn_rabbit_rect))
                SDL_RenderCopy(renderer, option_texture_spawn_rabbit_hover, NULL, &spawn_rabbit_rect);

            SDL_Rect spawn_fox_rect = {(SCREEN_WIDTH - 64) - 80, ((SCREEN_HEIGHT - 64)/2) + 96, 64, 64};
            SDL_RenderCopy(renderer, option_texture_spawn_fox, NULL, &spawn_fox_rect);
            if(SDL_HasIntersection(&mouse, &spawn_fox_rect))
                SDL_RenderCopy(renderer, option_texture_spawn_fox_hover, NULL, &spawn_fox_rect);

            SDL_Rect spawn_grass_rect = {(SCREEN_WIDTH - 64) - 80, ((SCREEN_HEIGHT - 64)/2) + 15, 64, 64};
            SDL_RenderCopy(renderer, option_texture_spawn_grass, NULL ,&spawn_grass_rect);
            if(SDL_HasIntersection(&mouse, &spawn_grass_rect))
                SDL_RenderCopy(renderer, option_texture_spawn_grass_hover, NULL, &spawn_grass_rect);            


            if((SDL_HasIntersection(&mouse, &pause_button)) && (mouse_pos &SDL_BUTTON_LEFT)){
                current_window = 4;
                render_menu_flag = 4;
            }                    

            if(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    flag = false;
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if(SDL_HasIntersection(&mouse, &visibility_button)){
                        visibility_pressed = !visibility_pressed;
                        if(visibility_pressed)
                            visibility = 1;
                        else 
                            visibility = 0;
                    }

                    if(SDL_HasIntersection(&mouse, &spawn_rabbit_rect)){
                        total_rabbits += 1;
                        rabbits[total_rabbits - 1].spawn(renderer, current_time);
                        rabbits[total_rabbits - 1].is_dead = false; 
                        rabbits[total_rabbits - 1].is_kit = false;
                        rabbits[total_rabbits - 1].timer_growth = SDL_GetTicks();
                        rabbits[total_rabbits - 1].spawn_flag_once = 3;
                        rabbits[total_rabbits - 1].hunger_level = rabbits[total_rabbits - 1].hunger_level_max;
                        rabbits[total_rabbits - 1].thirst_level = rabbits[total_rabbits - 1].thirst_level_max;
                        rabbits[total_rabbits - 1].mate_level = rabbits[total_rabbits - 1].mate_level_max;
                        
                        
                    }

                    if(SDL_HasIntersection(&mouse, &spawn_fox_rect)){
                        total_foxes += 1;
                        foxes[total_foxes - 1].hunger_level = foxes[total_foxes - 1].hunger_level_max;
                        foxes[total_foxes - 1].rest_level = foxes[total_foxes - 1].rest_level_max;
                        foxes[total_foxes - 1].mate_level = foxes[total_foxes - 1].mate_level_max;
                        foxes[total_foxes - 1].timer_growth = SDL_GetTicks();
                        foxes[total_foxes - 1].spawn(renderer, current_time);
                        foxes[total_foxes - 1].spawn_flag_once = 3;
                    }

                    for(int i=0; i<total_rabbits;i++){
                        if(SDL_HasIntersection(&(rabbits[i].rabbit_rect), &mouse)) 
                            rabbits[i].info();
                    }
                    for(int i=0; i<total_foxes;i++){
                        if(SDL_HasIntersection(&(foxes[i].fox_rect), &mouse)) 
                            foxes[i].info();
                    }   

                    

                    if(SDL_HasIntersection(&mouse, &spawn_grass_rect)){
                        total_grasses += 1;
                        grasses[total_grasses - 1].current_state = 0;
                        grasses[total_grasses - 1].timer_growth = SDL_GetTicks();
                    }

                }
                else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                // Reset the flag when the mouse button is released
                    is_visibility_checked = false;
                }

                else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
                    for(int i=0; i<total_rabbits;i++){
                        if(SDL_HasIntersection(&(rabbits[i].rabbit_rect), &mouse)){
                            rabbits[i].is_dead = true;
                            rabbits[i].cause_of_death_calc = 6;
                        }
                    }
                    for(int i=0; i<total_foxes;i++){                        
                        if(SDL_HasIntersection(&(foxes[i].fox_rect), &mouse)){
                            foxes[i].is_dead = true;
                            foxes[i].cause_of_death_calc = 3;
                        }
                    }
                    for(int i=0; i<total_grasses;i++){                        
                        if(SDL_HasIntersection(&(grasses[i].grass_rect), &mouse)){
                            grasses[i].grass_rect.h = 0;grasses[i].grass_rect.w = 0;
                        }
                    }
                }
            }

            // Rabbit - main activites
            for(int i=0;i<total_rabbits;i++){
                rabbits[i].spawn(renderer, current_time);
                if(!rabbits[i].dead_cooldown(current_time))
                    rabbits[i].render(renderer, visibility);
                if(!rabbits[i].is_dead)
                    rabbits[i].movement(current_time);
                rabbits[i].collision_detection_border();
                rabbits[i].essentials(renderer, lake.lake_rect, pond.pond_rect_1, current_time);
                rabbits[i].essentials(renderer, lake.lake_rect, pond.pond_rect_2, current_time);
                rabbits[i].find_mate();
                rabbits[i].pregnancy(current_time);
                rabbits[i].growth(current_time);
                rabbits[i].collision_detection_water(pond.pond_rect_1);
                rabbits[i].collision_detection_water(pond.pond_rect_2);
            }

            // Rabbit - search for mate
            for(int i=0;i<total_rabbits;++i){
                for(int j=0;j<total_rabbits;++j){
                    if(rabbits[i].flag_once_eat == 1 ){
                        if(rabbits[i].mate &&rabbits[j].mate && (!rabbits[i].is_dead) && (!rabbits[j].is_dead)){
                            if(SDL_HasIntersection(&(rabbits[i].rabbit_vision), &(rabbits[j].rabbit_rect)) && (!rabbits[j].is_kit) && (!rabbits[i].is_kit)){
                                if(rabbits[i].gender != rabbits[j].gender && (!rabbits[j].is_pregnant) && (!rabbits[i].is_pregnant)){
                                    rabbits[i].target_mate = rabbits[j].rabbit_rect;
                                    rabbits[i].found_partner = true;
                                    rabbits[i].flag_once_mate = 2;rabbits[j].flag_once_mate = 2;
                                }
                            }
                        }
                    }
                    if(rabbits[j].kit_born){
                        total_rabbits += 1;
                        rabbits[total_rabbits-1].location_x = rabbits[j].location_x + (16 - 8);
                        rabbits[total_rabbits-1].location_y = rabbits[j].location_y + (16 - 8);
                        rabbits[total_rabbits-1].timer_growth = SDL_GetTicks();
                        rabbits[total_rabbits-1].is_kit = true;
                        rabbits[total_rabbits-1].timer_growth = SDL_GetTicks();
                        int gene = random_number(0,1);
                        if(gene == 0){
                            rabbits[total_rabbits-1].traits[0] = rabbits[i].vision_radius;
                            rabbits[total_rabbits-1].traits[1] = rabbits[i].hunger_level_max;
                            rabbits[total_rabbits-1].traits[2] = rabbits[i].thirst_level_max;
                            rabbits[total_rabbits-1].traits[3] = rabbits[i].mate_level_max;
                        }
                        if(gene == 1){
                            rabbits[total_rabbits-1].traits[0] = rabbits[j].vision_radius;
                            rabbits[total_rabbits-1].traits[1] = rabbits[j].hunger_level_max;
                            rabbits[total_rabbits-1].traits[2] = rabbits[j].thirst_level_max;
                            rabbits[total_rabbits-1].traits[3] = rabbits[j].mate_level_max;
                        }
                        if(rabbits[i].info_generation > rabbits[j].info_generation)
                            rabbits[total_rabbits-1].info_generation = rabbits[i].info_generation;
                        else
                            rabbits[total_rabbits-1].info_generation = rabbits[j].info_generation;

                        // Increase traits in the next generation
                        rabbits[total_rabbits-1].info_generation += 1;
                        rabbits[total_rabbits-1].traits[0] += random_number(1, 5);
                        rabbits[total_rabbits-1].traits[1] += random_number(1, 5);
                        rabbits[total_rabbits-1].traits[2] += random_number(1, 5);
                        rabbits[total_rabbits-1].traits[3] += random_number(1, 5);

                        rabbits[total_rabbits-1].vision_radius = 70;
                        rabbits[total_rabbits-1].hunger_level_max = random_number(50, 70);
                        rabbits[total_rabbits-1].thirst = random_number(50, 70);
                        rabbits[total_rabbits-1].width = 20; 
                        rabbits[total_rabbits-1].height = 20;

                        rabbits[j].kit_born = false;

                    }
                }
            }
            // Grass - main activies
            for(int i=0;i<total_grasses;i++){
                grasses[i].render_grass(renderer);
                grasses[i].grass_growth(current_time);
            }

            // Rabbit - Find Grass-->
            for(int rabbit = 0;rabbit < total_rabbits ; rabbit++){
                for(int grass = 0; grass < total_grasses; grass++){
                    // Dead
                    if(current_time - rabbits[rabbit].timer_low_hunger > 60000){
                        if(rabbits[rabbit].timer_flag_hunger == 1){
                            rabbits[rabbit].is_dead = true;
                            rabbits[rabbit].cause_of_death_calc = 1;
                        }
                    }
                    // Find the lcoation grass from surrounding
                    if (grasses[grass].rabbit_eat_flag(rabbits[rabbit].get_rect_vision()) && rabbits[rabbit].hungry == true){
                        if(grasses[grass].current_state == 3){
                            rabbits[rabbit].target_found_grass = true;
                            rabbits[rabbit].flag_once_eat = 2;
                            // Find its way to the grass
                            if (rabbits[rabbit].target_found_grass == true){
                                if(grasses[grass].rabbit_eat_flag(rabbits[rabbit].get_rect_rabbit())){
                                    rabbits[rabbit].flag_once_eat += 1;
                                }
                                // Eat grass
                                if(rabbits[rabbit].flag_once_eat == 3){
                                    grasses[grass].grass_rect.h -= 1;
                                    grasses[grass].grass_rect.w -= 1;
                                    if(rabbits[rabbit].hunger_level < rabbits[rabbit].hunger_level_max){
                                        rabbits[rabbit].hunger_level += 0.1;
                                    }
                                    else{
                                            rabbits[rabbit].hungry = false;
                                    }
                                    // Remove eaten grass from the total_grass list
                                    if(grasses[grass].grass_rect.h < 20){
                                        rabbits[rabbit].hunger_level = rabbits[rabbit].hunger_level_max;
                                        rabbits[rabbit].info_grasses_consumed += 1;
                                        rabbits[rabbit].timer_flag_hunger = 0;

                                        
                                    }
                                }
                                rabbit[rabbits].eat(current_time, grasses[grass].grass_rect);
                            }
                        }
                    }
                    
                }

            }
            
            // Rabbit - Escape Fox 
            for(int rabbit = 0; rabbit <= total_rabbits; rabbit++){
                for(int fox = 0; fox <= total_foxes; fox++){
                    if(!rabbits[rabbit].is_dead && (!foxes[fox].is_dead)){
                        if(SDL_HasIntersection(&(foxes[fox].fox_rect), &(rabbits[rabbit].rabbit_vision)) && !(foxes[fox].rest)){
                            if (foxes[fox].fox_rect.x < rabbits[rabbit].rabbit_rect.x - 5){
                                rabbits[rabbit].location_x += 2;
                            }
                            if (foxes[fox].fox_rect.x > rabbits[rabbit].rabbit_rect.x - 5){
                                rabbits[rabbit].location_x -= 2;
                            }
                            if(foxes[fox].fox_rect.y < rabbits[rabbit].rabbit_rect.y - 5){
                                rabbits[rabbit].location_y += 2;
                            }
                            if(foxes[fox].fox_rect.y > rabbits[rabbit].rabbit_rect.y - 5){
                                rabbits[rabbit].location_y -= 2;
                            }
                        }
                    }
                }
            }


            // Fox - main activites
            for(int i=0;i<total_foxes;i++){
                foxes[i].spawn(renderer, current_time);
                if(!foxes[i].dead_cooldown(current_time))
                    foxes[i].render(renderer, visibility);
                if(!(foxes[i].is_dead))
                    foxes[i].movement(current_time);
                foxes[i].collision_detection_border();
                foxes[i].essentials(renderer, current_time);
                foxes[i].find_mate();
                foxes[i].pregnancy(current_time);
                foxes[i].growth(current_time);
                foxes[i].collision_detection_water(pond.pond_rect_1);
                foxes[i].collision_detection_water(pond.pond_rect_2);
            }
                

            // Fox - Eat rabbit
            for(int fox = 0; fox < total_foxes; fox++){
                for(int rabbit = 0; rabbit <= total_rabbits; rabbit++){
                    // Dead
                    if(current_time - foxes[fox].timer_low_hunger > 60000){
                        if(foxes[fox].timer_flag_hunger == 1){
                            foxes[fox].is_dead = true;
                            foxes[fox].cause_of_death_calc = 1;
                        }
                    }
                    if(!foxes[fox].is_dead){
                        if(SDL_HasIntersection(&(foxes[fox].fox_rect), &(rabbits[rabbit].rabbit_rect)) && (!foxes[fox].rest) && !rabbits[rabbit].is_dead){
                            rabbits[rabbit].is_dead = true;
                            rabbits[rabbit].cause_of_death_calc = 4;
                            foxes[fox].rest_level -= 50;
                            foxes[fox].hunger_level = foxes[fox].hunger_level_max;
                            foxes[fox].info_prey_consumed_temp += 0.5;
                            }
                    }
                    if(!(foxes[fox].mate || foxes[fox].rest) && (!rabbits[rabbit].is_dead)){
                        if(SDL_HasIntersection(&(foxes[fox].fox_vision), &(rabbits[rabbit].rabbit_rect))){
                            if (foxes[fox].fox_rect.x < rabbits[rabbit].rabbit_rect.x - 5){
                                foxes[fox].location_x += 1;
                            }
                            if (foxes[fox].fox_rect.x > rabbits[rabbit].rabbit_rect.x - 5){
                                foxes[fox].location_x -= 1;
                            }
                            if(foxes[fox].fox_rect.y < rabbits[rabbit].rabbit_rect.y - 5){
                                foxes[fox].location_y += 1;
                            }
                            if(foxes[fox].fox_rect.y > rabbits[rabbit].rabbit_rect.y - 5){
                                foxes[fox].location_y -= 1;
                            }
                        }
                    }
                }
            }

            // Fox - Search for mate
            for(int i=0;i<=total_foxes-1;++i){
                for(int j=0;j<total_foxes;++j){
                    if(foxes[i].flag_once_eat == 1){
                        if(foxes[i].mate && foxes[j].mate && (!foxes[i].is_dead) && (!foxes[j].is_dead)){
                            if(SDL_HasIntersection(&(foxes[i].fox_vision), &(foxes[j].fox_rect)) && (!foxes[j].is_kit) && (!foxes[i].is_kit)){
                                if(foxes[i].gender != foxes[j].gender && (!foxes[j].is_pregnant) && (!foxes[i].is_pregnant)){
                                    foxes[i].target_mate = foxes[j].fox_rect;
                                    foxes[i].found_partner = true;
                                    foxes[i].flag_once_mate = 2;foxes[j].flag_once_mate = 2;
                                }
                            }
                        }
                    }
                    if(foxes[j].kit_born){
                        total_foxes += 1;
                        foxes[total_foxes-1].location_x = foxes[j].location_x + (16 - 8);
                        foxes[total_foxes-1].location_y = foxes[j].location_y + (16 - 8);
                        foxes[total_foxes-1].timer_growth = SDL_GetTicks();
                        foxes[total_foxes-1].is_kit = true;
                        foxes[total_foxes-1].timer_growth = SDL_GetTicks();
                        int gene = random_number(0,1);
                        if(gene == 0){
                            foxes[total_foxes-1].traits[0] = foxes[i].vision_radius;
                            foxes[total_foxes-1].traits[1] = foxes[i].hunger_level_max;
                            foxes[total_foxes-1].traits[2] = foxes[i].rest_level_max;
                            foxes[total_foxes-1].traits[3] = foxes[i].mate_level_max;
                        }
                        if(gene == 1){
                            foxes[total_foxes-1].traits[0]= foxes[j].vision_radius;
                            foxes[total_foxes-1].traits[1] = foxes[j].hunger_level_max;
                            foxes[total_foxes-1].traits[2] = foxes[j].rest_level_max;
                            foxes[total_foxes-1].traits[3] = foxes[j].mate_level_max;
                        }

                        if(foxes[i].info_generation > foxes[j].info_generation)
                            foxes[total_foxes-1].info_generation = foxes[i].info_generation;
                        else
                            foxes[total_foxes-1].info_generation = foxes[j].info_generation;

                        // Increase traits in the next generation
                        foxes[total_foxes-1].info_generation += 1;
                        foxes[total_foxes-1].traits[0] += random_number(1, 5);
                        foxes[total_foxes-1].traits[1] += random_number(1, 5);
                        foxes[total_foxes-1].traits[2] += random_number(1, 5);
                        foxes[total_foxes-1].traits[3] += random_number(1, 5);
                        foxes[total_foxes-1].vision_radius = 70;
                        foxes[total_foxes-1].hunger_level_max = random_number(50, 70);
                        foxes[total_foxes-1].rest = random_number(50, 70);
                        foxes[total_foxes-1].width = 20; 
                        foxes[total_foxes-1].height = 20;

                        foxes[j].kit_born = false;

                    }
                }
            }


            SDL_RenderPresent(renderer);

            SDL_Delay(1000/30);

            if(total_grasses > 300 || total_rabbits > 100){
                std::cout<<"\n=========================\n\t\tLimit Reached";
                break;
            }
        }
        
    }


    // Clear the texture memory
    for(int i=0;i<total_rabbits;i++){
        SDL_DestroyTexture(rabbits[i].rabbit_texture);
        SDL_DestroyTexture(rabbits[i].info_hungry_texture);
        SDL_DestroyTexture(rabbits[i].info_thirst_texture);
    }
    for(int i=0;i<total_grasses;i++){
        SDL_DestroyTexture(grasses[i].grass_texture[0]);
        SDL_DestroyTexture(grasses[i].grass_texture[1]);
        SDL_DestroyTexture(grasses[i].grass_texture[2]);
        SDL_DestroyTexture(grasses[i].grass_texture[3]);
    }

    SDL_DestroyTexture(lake.lake_texture);
    SDL_DestroyTexture(pond.pond_texture_1);
    SDL_DestroyTexture(pond.pond_texture_2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    std::cout<<"\n\n\t\tPRESS ANY KEY TO EXIT";
    std::cout<<"\n\n\t\t=====================";

    getch();

    return 0;

}


