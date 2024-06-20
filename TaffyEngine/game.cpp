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
global_var bool background = false;
//^do not mess with names, meant to be constants

#include "renderer.cpp"
#include "objects.cpp"

enum GameScreens {
	INTRO, MAINMENU, 
	OPTIONS,
	SOUND, CONTROLS,
	CREDITS, GAME, PAUSEMENU,
};

GameScreens currentScreen = INTRO;

struct PControls {
	Inputs square = KEY_1;
	Inputs triangle = KEY_2;
	Inputs circle = KEY_3;
	Inputs up = W;
	Inputs down = S;
	Inputs left = A;
	Inputs right = D;
	Inputs rotleft = Q;
	Inputs rotright = E;
	Inputs power = ENTER;
};


#define destroy(o, type) if (o.instance) { o.~type(); }
#define rebuild(o, type, construct) destroy(o, type) new (&o) construct;
#define buildonce(o, construct) if (!o.instance) new (&o) construct;

internal void gameinit() {
	clearEntireScreen(0x000000);
}

#include "events.cpp"

internal void gameloop(float dt) {
	//default background
	if (background) renderBG();

	switch (currentScreen) {
		case INTRO: {
			intro(dt);
		} break;
		case MAINMENU: {
			mainMenu(dt);
		}break;
		case GAME: {
			game(dt);
		} break;
	}

	
}