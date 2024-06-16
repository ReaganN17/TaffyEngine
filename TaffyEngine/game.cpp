//rendering and gaming factors
//moved here so variables that are supposed to be interacted with are in one place

//mouse x and y position relative to coordinates
global_var POINT mouse;

//contains button inputs
global_var Input input = {};

#define isdown(b) input.buttons[b].down
#define pressed(b) (input.buttons[b].down && input.buttons[b].changed)
#define released(b) (!input.buttons[b].down && input.buttons[b].changed)

//contains Camera that most objects will follow
global_var Camera mainCam(0, 0, 1);

//runs the game, if set false will exit the window
global_var bool running = true;

//if backround doesnt exist, set to enable black bg;
global_var bool background = true;
//^do not mess with names, meant to be constants

#include "renderer.cpp"
#include "objects.cpp"

enum GameScreens {
	INTRO, MAINMENU, EXIT,
	OPTIONS,
	SOUND, CONTROLS,
	CREDITS,
	LEVELS, GAME, PAUSEMENU,
};

GameScreens currentScreen = INTRO;


#define destroy(o, type) if (o.instance) { o.~type(); }
#define rebuild(o, type, construct) destroy(o, type) new (&o) construct;
#define buildonce(o, construct) if (!o.instance) new (&o) construct;

float player_x = 0, player_y = 0;
float camera_x = 0, camera_y = 0, cameraspeed = 500, zoom = 1;

//Create Objects in order of depth, fartheset to closest if same Z level
//Object bob1(50, 50, 50, 50, "resources/bob.png", MIDDLE);
//Object bob2(-50, -50, 50, 50, "resources/bob.png", MIDDLE);
//Object bob3(50, -50, 50, 50, "resources/bob.png", MIDDLE);
//Object bob4(-50, 50, 50, 50, "resources/bob.png", MIDDLE);

//Button test(0, 0, 20, 20, "resources/please.png");


internal void gameinit() {
	clearScreen(0x000000);
}

#include "events.cpp"

internal void gameloop(float dt) {
	if (background) renderBG();

	switch (currentScreen) {
		case INTRO: {
			intro(dt);
		} break;
		case MAINMENU: {
			mainMenu(dt);
		}break;


		case GAME: {



			if (isdown(W)) { player_y += 300 * dt; }
			if (isdown(S)) { player_y -= 300 * dt; }

			if (isdown(A)) { player_x -= 300 * dt; }
			if (isdown(D)) { player_x += 300 * dt; }


			if (isdown(UP)) camera_y += 150 * dt;
			if (isdown(DOWN)) camera_y -= 150 * dt;

			if (pressed(Q)) { destroyAllObjects(); }
			//if (pressed(E)) { bob1.~Object(); }
			

			mainCam.setPos(camera_x, camera_y, zoom);

			//bob1.setPos(player_x, player_y);
			//renderCustomBG(dt, false);

			renderAllObjects();

			//draw_number(bob1.pos, 50, 50, 10);
			//draw_number(bob2.pos, -50, -50, 10);
			//draw_number(bob3.pos, 50, -50, 10);
			//draw_number(bob4.pos, -50, 50, 10);

			draw_number(dt * 100000, 0, 0, 10);
		} break;
	}

	if (pressed(ESC)) { running = false; }
}