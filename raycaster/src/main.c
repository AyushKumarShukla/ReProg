#include<SDL2/SDL.h>
#include<SDL2/SDL_keycode.h>
#include<SDL2/SDL_events.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include<SDL_stdinc.h>
//Screen parameters
#define WIDTH 900
#define HEIGHT 900

//Color definitions
//              AARRGGBB
#define CRED 	0x00ff0000
#define CBLACK 	0x00000000
#define CWHITE	0x00ffffff
#define CCYAN	0x00395555
#define CRED   	0x00ff0000
#define CGREEN 	0x0000ff00
#define CYELLOW 0x00ffff00
//Map parameters
#define MAP_HEIGHT 10
#define MAP_WIDTH 10
#define WALL 1
#define CELL_WIDTH (WIDTH/MAP_WIDTH)
#define CELL_HEIGHT (HEIGHT/MAP_HEIGHT)

//Player parameters
#define PLAYER_HEIGHT 20
#define PLAYER_WIDTH 20
#define ARROW_HEAD_DIMENSION 5
#define INITIAL_PLAYER_DIRECTION 45 //degrees
#define FOV_ANGLE 60.0 //degrees
#define PLAYER_ROTATION_SPEED 2
#define PLAYER_STRIDE 10
#define INITIAL_X 0
#define INITIAL_Y 0
//Geometric parameters
#define DEFAULT_LINE_LENGTH 1
#define RAY_SPEED 1 //pixels
#define RAY_DENSITY (0.21)
#define VERTICAL_SCALE HEIGHT * 100
//Helper macros
#define DEG2RAD M_PI/180
#define RAY_WITHIN_BOUNDS raypoint.pos.x >=0 && raypoint.pos.x <= WIDTH && raypoint.pos.y >=0 && raypoint.pos.y <=HEIGHT
int map[MAP_HEIGHT][MAP_WIDTH] = {

	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

};

typedef struct{
	double value;
}Angle;

typedef struct{
	double x;
	double y;
}Point;

typedef struct{
	Point pos;
	double w;
	double h;
	Angle dir;
}Player;

typedef struct{
	Point pos;
	int cell_x;
	int cell_y;
}RayPoint;

Player player = {{INITIAL_X,INITIAL_Y }, PLAYER_WIDTH, PLAYER_HEIGHT, {INITIAL_PLAYER_DIRECTION}};


void DrawMapGrid(SDL_Surface *surface){
	
	double cell_width = WIDTH/MAP_WIDTH;
	double cell_height = HEIGHT/MAP_HEIGHT;

	for(int i=0;i<MAP_HEIGHT;i++)
		for(int j=0;j<MAP_WIDTH;j++){
			SDL_Rect cell = {j*cell_width,i*cell_height,cell_width,cell_height};
			SDL_FillRect(surface,&cell,map[i][j] == 0 ? CBLACK:CCYAN);
		}
}

int DrawPlayer(SDL_Surface *surface){
	SDL_Rect player_rect = {player.pos.x, player.pos.y, player.w, player.h};
	return SDL_FillRect(surface, &player_rect, CRED);
}

void DrawArrowHead(SDL_Surface *surface, Point start, Angle dir, Uint32 color){
	double x2,y2;
	x2 = start.x + DEFAULT_LINE_LENGTH * cos(dir.value * DEG2RAD);
	y2 = start.y + DEFAULT_LINE_LENGTH * sin(dir.value * DEG2RAD);
	SDL_Rect arrow_head = {x2, y2, ARROW_HEAD_DIMENSION, ARROW_HEAD_DIMENSION};
	SDL_FillRect(surface, &arrow_head, color);
}

double GetDistance(Point player_pos, Point wall_pos){
	return(sqrt(pow((wall_pos.x - player_pos.x),2) + pow((wall_pos.y - player_pos.y),2)));
}

void DrawPlayerFOV(SDL_Surface *surface, SDL_Surface *psurface){
	Angle left_fov= {player.dir.value - (FOV_ANGLE/2.0)};
	Angle right_fov = {player.dir.value + (FOV_ANGLE/2.0)};
	DrawArrowHead(surface, player.pos, left_fov, CGREEN);
	DrawArrowHead(surface, player.pos, right_fov, CGREEN);
	
	double vline_stride = WIDTH/(FOV_ANGLE * (1/RAY_DENSITY));
	double vline_x;
	Angle angle;
	//cast rays					//controls scan speed
	for(angle=left_fov, (vline_x=0);angle.value<=right_fov.value && (vline_x <= WIDTH);(angle.value += RAY_DENSITY), (vline_x += vline_stride)){
	RayPoint raypoint = {{player.pos.x, player.pos.y}, 0, 0};
	while(map[raypoint.cell_y][raypoint.cell_x] != WALL && RAY_WITHIN_BOUNDS){
		raypoint.pos.x = raypoint.pos.x + RAY_SPEED * cos(angle.value * DEG2RAD);
		raypoint.pos.y = raypoint.pos.y + RAY_SPEED * sin(angle.value * DEG2RAD);

		raypoint.cell_x = (int)raypoint.pos.x/CELL_WIDTH;
		raypoint.cell_y = (int)raypoint.pos.y/CELL_HEIGHT;
		DrawArrowHead(surface, raypoint.pos, angle, CWHITE);
		//printf("Raypoint cell_x:%d, cell_y:%d, pos:(%f,%f)\n",raypoint.cell_x,raypoint.cell_y,raypoint.pos.x,raypoint.pos.y);
	}
	//printf("Wall Detected : Cell [%d,%d], RayPos: (%f,%f)\n",raypoint.cell_y,raypoint.cell_x,raypoint.pos.x,raypoint.pos.y);

	//we can now compute the distance between the player and this raypoint at the wall and draw vertical lines according to the distance to see the 3D like projection
		double ray_distance = GetDistance(player.pos, raypoint.pos);
		//printf("wall distance: %.2f\n",ray_distance);

		//perform projection on projwin
		double wall_height = VERTICAL_SCALE/ray_distance;
		double vline_y = (HEIGHT/2.0) - (wall_height/2.0);
		SDL_Rect vline = {vline_x, vline_y, 1, wall_height};
		SDL_FillRect(psurface, &vline, CYELLOW);
	}
}

bool app_running = true;
int main(int argc, char* argv[]){
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window *mainwin 
		= SDL_CreateWindow("RayTracer",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_MOUSE_CAPTURE);
	
	SDL_Window *projwin
		= SDL_CreateWindow("Psudo3D Projection",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_MOUSE_CAPTURE);
	
	SDL_Surface *mainwin_surface
		= SDL_GetWindowSurface(mainwin);
	
	SDL_Surface *projwin_surface
		= SDL_GetWindowSurface(projwin);

	SDL_Event event;
	while(app_running){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					app_running=false;
					break;
				case SDL_MOUSEMOTION:
					if(event.motion.xrel > 0)
						player.dir.value += PLAYER_ROTATION_SPEED % 360;
					if(event.motion.xrel < 0)
						player.dir.value -= PLAYER_ROTATION_SPEED % 360;
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_x){
						exit(1);
					}
					if(event.key.keysym.sym == SDLK_LEFT)
						player.dir.value -= PLAYER_ROTATION_SPEED % 360;
					if(event.key.keysym.sym == SDLK_RIGHT)
						player.dir.value += PLAYER_ROTATION_SPEED % 360;
					if(event.key.keysym.sym == SDLK_w){
					//move in the direction player.dir.value
					double new_x,new_y;
					new_x = player.pos.x + PLAYER_STRIDE * cos(player.dir.value * DEG2RAD);
					new_y = player.pos.y + PLAYER_STRIDE * sin(player.dir.value * DEG2RAD);
					player.pos.x = new_x;
					player.pos.y = new_y;
					}
					
					if(event.key.keysym.sym == SDLK_s){
				player.pos.x = player.pos.x - PLAYER_STRIDE * cos(player.dir.value * DEG2RAD);
				player.pos.y = player.pos.y - PLAYER_STRIDE * sin(player.dir.value * DEG2RAD);
					}
					if(event.key.keysym.sym == SDLK_a){
			player.pos.x = player.pos.x + PLAYER_STRIDE * cos( 90 - player.dir.value * DEG2RAD);
			player.pos.y = player.pos.y - PLAYER_STRIDE * sin( 90 - player.dir.value * DEG2RAD);
					}
					if(event.key.keysym.sym == SDLK_d){
			player.pos.x = player.pos.x - PLAYER_STRIDE * cos( 90 - player.dir.value * DEG2RAD);
			player.pos.y = player.pos.y + PLAYER_STRIDE * sin( 90 - player.dir.value * DEG2RAD);
					}
					break;

			}
		}

		// ---------------------------------------------------
		//Flush the surface buffer
		SDL_FillRect(mainwin_surface,NULL,CBLACK);
		SDL_FillRect(projwin_surface, NULL, CBLACK);
		
		//Draw the actual map from the map 2D array
		DrawMapGrid(mainwin_surface);
	
		//Draw player : Body and Direction Head
		{
			if(DrawPlayer(mainwin_surface) < 0){
				printf("Failed to draw player\n");
				printf("%s",SDL_GetError());
			}
			DrawArrowHead(mainwin_surface, player.pos, player.dir, CGREEN);
			DrawPlayerFOV(mainwin_surface,projwin_surface);
		}
		
		//cast rays from player to detect distance to nearby walls
	
		SDL_UpdateWindowSurface(mainwin);
		SDL_UpdateWindowSurface(projwin);
		SDL_Delay(10);	
	}
		
	return 0;
}
