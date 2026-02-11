#include<math.h>
#include<SDL3/SDL.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define WIDTH 900
#define HEIGHT 600

#define RECT_L 4
#define SCALE 10



typedef struct {

    int vx, vy;

} Velocity;


typedef struct {

    int x, y;
    Velocity v;
    Uint32 color;

} Agent;


int get_rand_coord(int max) {

    int sec = max / SCALE;
    return rand() % sec * SCALE + SCALE / 2;

}


Uint32 get_color(SDL_Surface *psurface, int num_agents, int agent) {

    float r, g, b;
	
	float h = (float) agent / num_agents;
	float s = 1;
	float v = 0.6;
	
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
        default: r = 0, g = 0, b = 0;
	}
	
	return SDL_MapSurfaceRGB(psurface, r*255, g*255, b*255);

}


bool is_valid(Agent *a, Velocity v) {

    if (!a) {
        return true;
    }

    if (a->v.vx != -v.vx && a->v.vy != -v.vy) {
        if (a->x + SCALE * v.vx >= RECT_L / 2 &&
            a->x + SCALE * v.vx <= WIDTH - RECT_L / 2 &&
            a->y + SCALE * v.vy >= RECT_L / 2 &&
            a->y + SCALE * v.vy <= HEIGHT - RECT_L / 2
        ) {
            return true;

        } else {
            return false;
        }

    } else {
        return false;
    }

}


Velocity get_rand_vel(Agent *a) {

    Velocity ret;

    bool valid = false;
    while (!valid) {

        int choice = rand() % 4;
        switch (choice) {
            case 0:
                ret = (Velocity) {1, 0};
                break;

            case 1:
                ret = (Velocity) {-1, 0};
                break;

            case 2:
                ret = (Velocity) {0, 1};
                break;

            case 3:
                ret = (Velocity) {0, -1};
                break;

            default:
                fprintf(stderr, "%s\n", "Wrong random value");
                exit(-1);
        }

        valid = is_valid(a, ret);

    }

    return ret;

}


Agent get_rand_agent(SDL_Surface *psurface, int num_agents, int agent) {

    Agent a;

    a.x = get_rand_coord(WIDTH);
    a.y = get_rand_coord(HEIGHT);
    a.v = get_rand_vel(NULL);
    a.color = get_color(psurface, num_agents, agent);

    return a;

}


void create_agents(SDL_Surface *psurface, Agent *pagents, int num_agents) {

    for (int i = 0; i < num_agents; i++) {

        pagents[i] = get_rand_agent(psurface, num_agents, i);

    }

}


void update_agents(SDL_Surface *psurface, Agent *pagents, int num_agents) {

    for (int i = 0; i < num_agents; i++) {

        Agent *pa = &pagents[i];
        Velocity v = get_rand_vel(pa);
        pa->v = v;

        SDL_Rect r;
        for (int j = 0; j < SCALE; j++) {

            pa->x += v.vx;
            pa->y += v.vy;

            r = (SDL_Rect) {pa->x, pa->y, RECT_L, RECT_L};
            SDL_FillSurfaceRect(psurface, &r, pa->color);

        }

    }

}


int main(int argc, const char *argv[]) {

    int num_agents;

    if (argc == 1) {
        num_agents = 5;

    } else if (argc == 2) {
        num_agents = atoi(argv[1]);

    } else {
        printf("Usage: %s <num_of_agents>\n", argv[0]);
        return -1;
    }

    srand(time(NULL));

    SDL_Window *pwindow = SDL_CreateWindow("Random Walk", WIDTH, HEIGHT, 0);
    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

    Agent *pagents = calloc(num_agents, sizeof(Agent));
    create_agents(psurface, pagents, num_agents);

    int running = 1;
    while (running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }

        }

        update_agents(psurface, pagents, num_agents);

        SDL_UpdateWindowSurface(pwindow);
        SDL_Delay(20);
    }

    free(pagents);
    return 0;

}

