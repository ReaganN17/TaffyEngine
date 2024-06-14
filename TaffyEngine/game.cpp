//rendering and gaming factors
//moved here so variables that are supposed to be interacted with are in one place

//mouse x and y position relative to coordinates
global_var POINT mouse;

//contains button inputs
global_var Input input = {};

//contains Camera that most objects will follow
global_var Camera mainCam(0, 0, 1);

//runs the game, if set false will exit the window
global_var bool running = true;

//^do not mess with names, meant to be constants

#include "renderer.cpp"
#include "objects.cpp"
#include "uibuttons.cpp"

enum GameScreens {
	INTRO, MAINMENU, EXIT,
	OPTIONS,
	SOUND, CONTROLS,
	CREDITS,
	LEVELS, GAME, PAUSEMENU,
};

GameScreens currentScreen = INTRO;


#define isdown(b) input.buttons[b].down
#define pressed(b) (input.buttons[b].down && input.buttons[b].changed)
#define released(b) (!input.buttons[b].down && input.buttons[b].changed)

#define destroy(o) if (o.instance) { o.~Object(); }
#define rebuild(o, construct) destroy(o) new (&o) construct;

//#define rebuild(o, constructor)  if(!o.instance) {new (&o) constructor;}

float player_x = 0, player_y = 0;
float camera_x = 0, camera_y = 0, cameraspeed = 500, zoom = 1;
long start;

//Create Objects in order of depth, fartheset to closest if same Z level
Object bob1(50, 50, 50, 50, "resources/bob.png", MIDDLE);
Object bob2(-50, -50, 50, 50, "resources/bob.png", MIDDLE);
Object bob3(50, -50, 50, 50, "resources/bob.png", MIDDLE);
Object bob4(-50, 50, 50, 50, "resources/bob.png", MIDDLE);

Button button(0, 0, 40, 40, "resources/please.png", BACK);

internal void gameinit() {
	start = GetTickCount64();
	clearScreen(0x000000);
}

#include "events.cpp"

internal void gameloop(float dt) {

	switch (currentScreen) {
		case INTRO: {
			intro(dt);
		} break;


		case GAME: {
			
			if (isdown(W)) { player_y = 500; }
			else if (isdown(S)) { player_y = -500; }
			else { player_y = 0; }

			if (isdown(A)) { player_x = -500; }
			else if (isdown(D)) { player_x = 500; }
			else { player_x = 0; }

			if (isdown(UP)) camera_y += cameraspeed * dt;
			if (isdown(DOWN)) camera_y -= cameraspeed * dt;

			if (pressed(LEFT)) { destroy(bob2); }
			if (pressed(RIGHT)) { rebuild(bob2, Object(50, 50, 50, 50, "resources/bob.png", MIDDLE)); }

			if (pressed(Q)) { destroy(bob1); }
			if (pressed(E)) { rebuild(bob1, Object(50, 50, 50, 50, "resources/bob.png", MIDDLE)); }

			

			mainCam.setPos(camera_x, camera_y, 1);
			bob1.velocity(player_x, player_y, dt);

			renderCustomBG(dt, false);

			draw_number(Z2.size(), 0, 0, 20);

			//bob1.render();
			//bob2.render();
			//bob3.render();
			//bob4.render();
			renderAllObjects();
		} break;
	}

	if (pressed(ESC)) { running = false; }
}