//SDL includes
#include<SDL2/SDL.h>

//libc includes
#include<stdbool.h>
#include<time.h>
//color definitions
//             0xAARRGGBB
#define CBLACK 0x00000000
#define CWHITE 0x00FFFFFF

//env config
#define WIDTH 900
#define HEIGHT 900
#define NUMPOINTS 1000
bool app_running=true;

typedef struct{
	float x;
	float y;
}Point;

typedef struct{
	int cnt;
	Point points[NUMPOINTS];
}Point_Set;

float getRandomX(){
	return (((float)rand()/(float)RAND_MAX) * WIDTH);
}

float getRandomY(){
	return (((float)rand()/(float)RAND_MAX) * HEIGHT);
}

Point_Set scatter_points(int len){
	Point_Set ps;
	ps.cnt = 0;
	for(int i=0;i<len;i++){
		ps.points[ps.cnt].x = getRandomX();
		ps.points[ps.cnt].y = getRandomY();
		ps.cnt++;
	}
	return ps;
}

int main(int arc, char* argv[]){
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(0));
	SDL_Window *mainwin 
		= SDL_CreateWindow("Plane",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);

	SDL_Renderer *ren = SDL_CreateRenderer(mainwin,0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	int len = 4;
	Point_Set ps = scatter_points(len);
	
	SDL_Event event;
	while(app_running){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					app_running=false;
					break;
			}
		}
		
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren,0xFF,0xFF,0xFF,0xFF);
		for(int i=0;i<ps.cnt;i++){
			SDL_RenderDrawPoint(ren, (int)ps.points[i].x , (int)ps.points[i].y);
		}


		SDL_SetRenderDrawColor(ren,0x00,0x00,0x00,0xFF);
		SDL_RenderPresent(ren);
		SDL_Delay(10);
	}
	SDL_DestroyWindow(mainwin);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
	return 0;
}
