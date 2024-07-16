//hold game events such as each level, pause menu, etc

//main loop

struct MainGameVar {
	u8 level = 1;
};

//headers for organization
internal void level1();
internal void level2();
internal void level3();
internal void level4();
internal void level5();
internal void level6();
internal void level7();
internal void level8();
internal void level9();
internal void level10();

MainGameVar mgVar;

PControls controls;

Grid grid;
Player bob;

internal void game() {
	clearScreen(0);
	switch (mgVar.level) {
	case 1: {
		level1();
	} break;
	case 2: {
		level2();
	} break;
	case 3: {
		level3();
	} break;
	case 4: {
		level4();
	} break;
	case 5: {
		level5();
	} break;
	case 6: {
		level6();
	} break;
	case 7: {
		level7();
	} break;
	case 8: {
		level8();
	} break;
	case 9: {
		level9();
	} break;
	case 10: {
		level10();
	} break;
	}
}

enum LevelEvents {
	INITOBJECTS, TRANSITION, LOAD, PLAYLOOP
};

struct LevelVar {
	u8 sequence = 0;
	int timeS = 0;
};
LevelVar lvar;

BasicEnemy rah;
PlayerUI huh;
Object test;

int camera_x = 0, camera_y = 0;
internal void level1() {

	switch (lvar.sequence) {
	case INITOBJECTS: {
		new (&grid) Grid(-480, 270, 50, "resources/testrender.png", 0, 0, 1, "resources/testrender.png");
		new (&bob) Player(&grid, 1, 1, &globalSpriteSheet);
		new (&rah) BasicEnemy(&grid, 5, 5, MIDDLE, 3);
		new (&huh) PlayerUI(&globalSpriteSheet);

		new (&test) Object(0, 0, 50, 50, (u32)0, FRONT);
		lvar.timeS = GetTickCount();
		lvar.sequence = LOAD;
		} break;
	case LOAD: {
		tsV.sequence = TRANSININIT;
		
		renderTransition(false, false);
		
		if (GetTickCount() - 1000 > lvar.timeS) {
			lvar.sequence = PLAYLOOP;
		}
	} break;
	case PLAYLOOP: {
		
		if (pressed(W)) { test.animateToPos(bob.x, bob.y, 2); }

		
		bob.control(controls);
		mainCam.setPos(bob.x, bob.y);

		updateAllObjects();
		huh.update(&bob);

		runEvents();
		renderAllObjects();
		
		draw_number(abs(test.x), 0, 50, 20);
		draw_number(abs(test.y), 0, -50, 20);

		renderTransition(false, false);

		/*
		for (int x = 0; x < 27; x++) {
			for (int y = 0; y < 21; y++) {
				draw_number(grid.getID(x, y), -20 + (10 * x), (-10 * y), 2);
			}
		}

		draw_number(bob.health, -200, 200, 20);
		*/
		
		

		if (pressed(ESC)) { running = false; }


		if (bob.health <= 0) {
			bob.destroyObject();
			tsV.sequence = TRANSOUTINIT;

			lvar.sequence = TRANSITION;
		}

	} break;
	case TRANSITION: {
		renderTransition(true, true);

		if (tsV.sequence == TRANSIDLE) {
			destroyAllObjects();
			lvar.sequence = INITOBJECTS;
		}
	} break;
	}
}


internal void level2() {

}
internal void level3() {

}
internal void level4() {

}
internal void level5() {

}
internal void level6() {

}
internal void level7() {

}
internal void level8() {

}
internal void level9() {

}
internal void level10() {

}