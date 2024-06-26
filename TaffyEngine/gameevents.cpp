//hold game events such as each level, pause menu, etc

#include "nodes.cpp"

Nodes PathMap;

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
BasicEnemy rah;

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
		new (&bob) Player(&grid, 1, 1);
		new (&rah) BasicEnemy(&grid, 4, 4, MIDDLE, 3);
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
		bob.control(controls, dt);


		mainCam.setPos(camera_x, camera_y);

		updateAllObjects(dt);
		renderAllObjects();

		/*
		draw_number(grid.grid[1 + 1 * grid.gw], -10, 0, 2);
		draw_number(grid.grid[2 + 1 * grid.gw], 0, 0, 2);
		draw_number(grid.grid[3 + 1 * grid.gw], 10, 0, 2);
		draw_number(grid.grid[4 + 1 * grid.gw], 20, 0, 2);
		draw_number(grid.grid[5 + 1 * grid.gw], 30, 0, 2);
		draw_number(grid.grid[1 + 2 * grid.gw], -10, -10, 2);
		draw_number(grid.grid[2 + 2 * grid.gw], 0, -10, 2);
		draw_number(grid.grid[3 + 2 * grid.gw], 10, -10, 2);
		draw_number(grid.grid[4 + 2 * grid.gw], 20, -10, 2);
		draw_number(grid.grid[5 + 2 * grid.gw], 30, -10, 2);
		draw_number(grid.grid[1 + 3 * grid.gw], -10, -20, 2);
		draw_number(grid.grid[2 + 3 * grid.gw], 0, -20, 2);
		draw_number(grid.grid[3 + 3 * grid.gw], 10, -20, 2);
		draw_number(grid.grid[4 + 3 * grid.gw], 20, -20, 2);
		draw_number(grid.grid[5 + 3 * grid.gw], 30, -20, 2);
		draw_number(grid.grid[1 + 4 * grid.gw], -10, -30, 2);
		draw_number(grid.grid[2 + 4 * grid.gw], 0, -30, 2);
		draw_number(grid.grid[3 + 4 * grid.gw], 10, -30, 2);
		draw_number(grid.grid[4 + 4 * grid.gw], 20, -30, 2);
		draw_number(grid.grid[5 + 4 * grid.gw], 30, -30, 2);
		draw_number(grid.grid[1 + 5 * grid.gw], -10, -40, 2);
		draw_number(grid.grid[2 + 5 * grid.gw], 0, -40, 2);
		draw_number(grid.grid[3 + 5 * grid.gw], 10, -40, 2);
		draw_number(grid.grid[4 + 5 * grid.gw], 20, -40, 2);
		draw_number(grid.grid[5 + 5 * grid.gw], 30, -40, 2);
		*/

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