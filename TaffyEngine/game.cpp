#include "camera.cpp"
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
Camera camera(0, 0, 1);

#define isdown(b) input->buttons[b].down
#define pressed(b) (input->buttons[b].down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].down && input->buttons[b].changed)

#define destroy(o) if (o.instance) { o.~Object(); }
#define rebuild(o, constructor) destroy(o); new (&o) constructor;

float player_x = 0, player_y = 0;
float camera_x = 0, camera_y = 0, cameraspeed = 500, zoom = 1;
long start;

//Create Objects in order of depth, fartheset to closest
Object rah(0, 0, 1920, 1080, "resources/please.png", CHARACTER, BACK);
Object bob(0, 0, "resources/bob.png", CHARACTER, MIDDLE);

internal void gameinit() {
	start = GetTickCount64();
	clearScreen(0x000000);
}

#include "events.cpp"

internal void gameloop(Input *input, float dt) {
	switch (currentScreen) {
		case INTRO: {
			intro(input, dt);
		} break;




		case GAME: {
			renderBG(camera);

			if (isdown(W)) { player_y = 500; }
			else if (isdown(S)) { player_y = -500; }
			else { player_y = 0; }

			if (isdown(A)) { player_x = -500; }
			else if (isdown(D)) { player_x = 500; }
			else { player_x = 0; }

			if (isdown(UP)) camera_y += cameraspeed * dt;
			if (isdown(DOWN)) camera_y -= cameraspeed * dt;

			if (isdown(LEFT)) camera_x -= cameraspeed * dt;
			if (isdown(RIGHT)) camera_x += cameraspeed * dt;

			if (isdown(Q)) zoom *= 1.1;
			if (isdown(E)) zoom /= 1.1;

			camera.setPos(camera_x, camera_y, zoom);

			bob.velocity(player_x, player_y, dt);

			renderAllObjects(camera);
		} break;
	}
}