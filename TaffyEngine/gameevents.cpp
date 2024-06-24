//hold game events such as each level, pause menu, etc
#include "grid.cpp"
#include "gridobject.cpp"
//main loop

struct MainGameVar {
	u8 level = 1;
};

//headers for organization
internal void level1(float dt);
internal void level2(float dt);
internal void level3(float dt);
internal void level4(float dt);
internal void level5(float dt);
internal void level6(float dt);
internal void level7(float dt);
internal void level8(float dt);
internal void level9(float dt);
internal void level10(float dt);

MainGameVar mgVar;

PControls controls;

Grid grid;
Player bob;

internal void game(float dt) {
	clearScreen(0);
	switch (mgVar.level) {
	case 1: {
		level1(dt);
	} break;
	case 2: {
		level2(dt);
	} break;
	case 3: {
		level3(dt);
	} break;
	case 4: {
		level4(dt);
	} break;
	case 5: {
		level5(dt);
	} break;
	case 6: {
		level6(dt);
	} break;
	case 7: {
		level7(dt);
	} break;
	case 8: {
		level8(dt);
	} break;
	case 9: {
		level9(dt);
	} break;
	case 10: {
		level10(dt);
	} break;
	}
}



struct LevelVar {
	u8 sequence = 0;
	int tsY = 0;
	int tsRate = 0;
	int timeS = 0;
};
LevelVar lvar;

int camera_x = 0, camera_y = 0;
internal void level1(float dt) {

	switch (lvar.sequence) {
	case 0: {
		new (&grid) Grid(-480, 270, 50, "resources/testrender.png", 0, 0, 1, "resources/testrender.png");
		new (&bob) Player(&grid, 1, 1, "resources/bob.png", 0.75);
		lvar.timeS = GetTickCount();
		lvar.sequence = 1;
		} break;
	case 1: {
		renderRect(0, 0, 480, 270, 0);

		if (GetTickCount() - 1000 > lvar.timeS) {
			lvar.sequence = 2;
		}
	} break;
	case 2: {
		if (isdown(LEFT)) camera_x -= 300 * dt;
		if (isdown(RIGHT))camera_x += 300 * dt;
		if (isdown(UP)) camera_y += 300 * dt;
		if (isdown(DOWN))camera_y -= 300 * dt;

		bob.move(controls, dt);

		mainCam.setPos(camera_x, camera_y);

		updateAllObjects();
		renderAllObjects();

		if (pressed(ESC)) { running = false; }

		if (lvar.tsY < 540) {
			lvar.tsY += lvar.tsRate * dt;
			lvar.tsRate += 1080 * dt;
			renderRect(0, lvar.tsY, 480, 270, 0);
		}

	} break;
	}
}


internal void level2(float dt) {

}
internal void level3(float dt) {

}
internal void level4(float dt) {

}
internal void level5(float dt) {

}
internal void level6(float dt) {

}
internal void level7(float dt) {

}
internal void level8(float dt) {

}
internal void level9(float dt) {

}
internal void level10(float dt) {

}