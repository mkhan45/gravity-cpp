#include <iostream>
#include <vector>
#include <math.h>
#include <tuple>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

using namespace std;

const float G = 1.0;
const int SCREEN_WIDTH = 800.0;
const int SCREEN_HEIGHT = 800.0;

float distance(float x1, float y1, float x2, float y2){
    float dx_sqr = pow(x2 - x1, 2);
    float dy_sqr = pow(y2 - y1, 2);
    return sqrt(dx_sqr + dy_sqr);
}

class Body{
    public:
        float radius;
        float mass;
        float x;
        float y;
        float v_x;
        float v_y;

    public:
        Body (float, float, float, float);
        float accel_mag(float);
        tuple<float, float> accel_vector(float x, float y);
};

Body::Body (float r, float m, float x1, float y1){
    radius = r;
    mass = m;
    x = x1;
    y = y1;
    v_x = 0.0;
    v_y = 0.0;
}

float Body::accel_mag(float radius){
    return (G * mass) / pow(radius, 2);
}

tuple<float, float> Body::accel_vector(float other_x, float other_y){
    float radius = distance(x, y, other_x, other_y);
    float mag = accel_mag(radius);
    float x_comp = (x - other_x)/radius * mag;
    float y_comp = (y - other_y)/radius * mag;
    return make_tuple(x_comp, y_comp);
}

int main(){
    vector<Body> bodies;

    //Body (radius, mass, x, y)
    bodies.push_back(Body (50.0, 800.0, 400.0, 400.0)); //big planet

    Body satellite = Body(5.0, 1.0, 750.0, 400.0);
    satellite.v_y = -1.5;

    bodies.push_back(satellite);


    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Renderer* renderer = NULL;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ); 
    else{
        window = SDL_CreateWindow("SDL Window",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
        if (window == NULL){
            printf("Error creating window: %s\n", SDL_GetError());
        }else{
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            SDL_Event e;

            while(true){
                for(int i = 0; i < bodies.size(); i++){

                    SDL_PollEvent(&e);
                    if(e.type == SDL_QUIT){
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 0;
                    }

                    float accel_x = 0.0;
                    float accel_y = 0.0;

                    Body current = bodies[i];

                    /* cout << "Body #" << i << " -> x: " << current.x << " y: " << current.y << "\n"; */

                    for(int j = 0; j < bodies.size(); j++){
                        Body other = bodies[j];
                        if(distance(current.x, current.y, other.x, other.y) >= current.radius + other.radius){
                            tuple<float, float> accel = other.accel_vector(current.x, current.y);
                            accel_x += get<0>(accel);
                            accel_y += get<1>(accel);
                        }
                    }

                    bodies[i].v_x += accel_x;
                    bodies[i].v_y += accel_y;

                    bodies[i].x += bodies[i].v_x + (accel_x/2);
                    bodies[i].y += bodies[i].v_y + (accel_y/2);

                    filledCircleRGBA(renderer, current.x, current.y, current.radius, 255, 255, 255, 255);
                }

                SDL_RenderPresent(renderer);
                SDL_UpdateWindowSurface(window);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                usleep(16666);
            }
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
