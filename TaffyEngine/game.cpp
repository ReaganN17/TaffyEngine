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
global_var Camera mainCam(0, 0, 1.5);

//runs the game, if set false will exit the window
global_var bool running = true;

//if backround doesnt exist, set to enable black bg;
global_var bool background = false;
//other bools
global_var bool render = true;
global_var bool update = true;
global_var bool runevents = true;
//^do not mess with names, meant to be constants

//basic elements
#include "renderer.cpp"
#include "events.cpp"
#include "objects.cpp"

//game elements
global_var u8 levelUnlocked = 1;
global_var u8 curLevel = 0;

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

PControls controls;


#include "gridv2.cpp"
#include "gridobject.cpp"
#include "playerUI.cpp"

internal void gameinit() {
	clearEntireScreen(0x000000);
}

Image globalSpriteSheet("resources/MegaSpriteSheet.png");

#include "intro.cpp"
#include "levels.cpp"
#include "gameevents.cpp"


BasicEvent* screen = nullptr;

internal void gameloop() {
	//default background
	if (background) renderBG();

	switch (currentScreen) {
	case INTRO: {
		if (screen == nullptr) { screen = new IntroEvent(&screen); screen->start(); }
	} break;
	case MAINMENU: {
		if (screen == nullptr) { screen = new MainScreen(&screen); screen->start(); }
	}break;
	case GAME: {
		if (screen == nullptr) { screen = new GameEvent(&screen); screen->start(); }
	} break;
	}


	if (runevents) { runEvents(); }
	if (update) {updateAllObjects();}
	if (render) { renderAllObjects(); }
	
	//draw_number(events.size(), 0, 0, 20);
	
}