// these items are more related to the game itself 
// it helps me to make the tools i need if i make the game alongside

#include "gameevents.cpp"


//intro sequence
struct IntroVar {
	short alpha = 0;
	u8 sequence = 0;
};
IntroVar introVar;
internal void intro(float dt) {

	switch (introVar.sequence) {
	case 0: {
		introVar.alpha += 200 * dt;
		renderStaticBG("resources/taffyEngine.png", clamp(0, introVar.alpha, 255) , 0);
		if (introVar.alpha >= 300 || pressed(ENTER)) introVar.sequence = 2;
	} break;
	case 2: {
		introVar.alpha -= 200 * dt;
		renderStaticBG("resources/taffyEngine.png", clamp(0, introVar.alpha, 255), 0);
		
		if (introVar.alpha <= -50) introVar.sequence = 3;
	} break;
	case 3: {
		introVar.sequence = 0;
		introVar.alpha = 0;
		currentScreen = MAINMENU;
	}

	}
}

//my moving background
float customBGx = 0, customBGy = 0, change = 0;
internal void renderCustomBG(float dt, bool stop, u8 shade = 0, bool reset = false) {
	if (reset) {
		customBGx = 0, customBGy = 0, change = 0;
	}
	else {
		if (!stop) {
			customBGx -= 960 * dt * 0.2;
			customBGy -= 540 * dt * 0.2;
			change += 1 * dt;
		}
		if (change >= 3) change -= 3;
		if (customBGx <= -960) customBGx += 960;
		if (customBGy <= -540) customBGy += 540;
		switch ((int)change % 3) {
			case 0: {renderMovingBG("resources/triangles.png", customBGx + 480, customBGy + 270, 2, shade);} break;
			case 1: {renderMovingBG("resources/circles.png", customBGx + 480, customBGy + 270, 2, shade);} break;
			case 2: {renderMovingBG("resources/squares.png", customBGx + 480, customBGy + 270, 2, shade);} break;
		}
	}
}


//events are named so its easier to keep track of them
enum MainMenuEvents {
	MMINIT, MMLOOP, EXITINIT, EXITLOOP, LEVELINIT, LEVELLOOP, TRANSITIONOUT, TRANSITIONIN
};

//main menu event
struct MainMenuVar {
	u8 sequence = MMINIT;
	u8 levelUnlocked = 1;
	int tsY = 700;
	int tsRate = -1400;
};

MainMenuVar mmVar;

Button LevelSelect;
Button Options;
Button Controls;
Button Credits;
UIObject Title;

UIObject Exit;
Button ExitYea;
Button ExitNah;

Button Level1;
Button Level2;
Button Level3;
Button Level4;
Button Level5;
Button Level6;
Button Level7;
Button Level8;
Button Level9;
Button Level10;

Button Back;

#define LevelButton(b, n) \
if (b.bb.released) {\
		mgVar.level = n;\
		mmVar.sequence = TRANSITIONOUT;\
}

internal void mainMenu(float dt) {
	switch (mmVar.sequence) {
	case MMINIT: {
		renderCustomBG(dt, true, 0, true);
		new (&LevelSelect) Button(-160, -50, 250, 70, "resources/levelselect.png");
		new (&Options) Button(160, -50, 250, 70, "resources/options.png");
		new (&Credits) Button(160, -150, 250, 70, "resources/credits.png");
		new (&Controls) Button(-160, -150, 250, 70, "resources/controls.png");
		new (&Title) UIObject(0, 120, 350, 200, "resources/title2.png");
		mmVar.sequence = MMLOOP;
	}break;
	case MMLOOP: {
		renderCustomBG(dt, false);
		updateAllObjects();
		renderAllObjects();
		

		if (LevelSelect.bb.released) { 
			destroyAllObjects();
			mmVar.sequence = LEVELINIT; 
		}

		if (pressed(ESC)) {
			mmVar.sequence = EXITINIT;
		}
	}break;
	case EXITINIT: {
		LevelSelect.bb.disable = true;
		Options.bb.disable = true;
		Credits.bb.disable = true;
		Controls.bb.disable = true;

		updateAllObjects();

		LevelSelect.setShade(50);
		Options.setShade(50);
		Credits.setShade(50);
		Controls.setShade(50);
		Title.setShade(50);

		new (&Exit) UIObject(0, 0, 250, 150, "resources/exit game.png", FRONT);
		new (&ExitYea) Button(90, -50, 30, 15, "resources/exit yea.png", FARFRONT);
		new (&ExitNah) Button(-90, -50, 30, 15, "resources/exit nah.png", FARFRONT);
		
		mmVar.sequence = EXITLOOP;
		
	} break;
	case EXITLOOP: {
		renderCustomBG(dt, true, 50);
		renderZObjects(&Middle_2);

		updateZObjects(&Front_3);
		updateZObjects(&FarFront_4);
		renderZObjects(&Front_3);
		renderZObjects(&FarFront_4);
		

		if (ExitYea.bb.pressed) {
			running = false;
		}

		if (pressed(ESC) || ExitNah.bb.pressed) {
			ExitYea.~Button();
			ExitNah.~Button();
			Exit.~UIObject();
			

			LevelSelect.bb.disable = false;
			Options.bb.disable = false;
			Credits.bb.disable = false;
			Controls.bb.disable = false;

			Title.setShade(0);

			mmVar.sequence = MMLOOP;
		}

		
	} break;
	case LEVELINIT: {
		//create buttons yea
		new (&Level1) Button(-320, 0, 75, 75, "resources/numbers.png", 0, 0, 50, 50);
		new (&Level2) Button(-160, 0, 75, 75, "resources/numbers.png", 50, 0, 50, 50);
		new (&Level3) Button(0, 0, 75, 75, "resources/numbers.png", 100, 0, 50, 50);
		new (&Level4) Button(160, 0, 75, 75, "resources/numbers.png", 150, 0, 50, 50);
		new (&Level5) Button(320, 0, 75, 75, "resources/numbers.png", 200, 0, 50, 50);
		new (&Level6) Button(-320, -150, 75, 75, "resources/numbers.png", 0, 50, 50, 50);
		new (&Level7) Button(-160, -150, 75, 75, "resources/numbers.png", 50, 50, 50, 50);
		new (&Level8) Button(0, -150, 75, 75, "resources/numbers.png", 100, 50, 50, 50);
		new (&Level9) Button(160, -150, 75, 75, "resources/numbers.png", 150, 50, 50, 50);
		new (&Level10) Button(320, -150, 75, 75, "resources/numbers.png", 200, 50, 50, 50);

		new (&Back) Button(-360, 225, 100, 50, "resources/back.png");

		//reuse button as UI im really lazy
		new (&LevelSelect) UIObject(0, 150, 350, 100, "resources/levelselect.png");

		//goes down the line depending on the max level unlocked and disables the buttons above the max level unlocked
		//i think its a unique way of using switch and case system
		switch (mmVar.levelUnlocked) {
		case 1: Level2.bb.disable = true; Level2.changeCrop("resources/numbers.png", 50, 100, 50, 50);
		case 2: Level3.bb.disable = true; Level3.changeCrop("resources/numbers.png", 100, 100, 50, 50);
		case 3: Level4.bb.disable = true; Level4.changeCrop("resources/numbers.png", 150, 100, 50, 50);
		case 4: Level5.bb.disable = true; Level5.changeCrop("resources/numbers.png", 200, 100, 50, 50);
		case 5: Level6.bb.disable = true; Level6.changeCrop("resources/numbers.png", 0, 150, 50, 50);
		case 6: Level7.bb.disable = true; Level7.changeCrop("resources/numbers.png", 50, 150, 50, 50);
		case 7: Level8.bb.disable = true; Level8.changeCrop("resources/numbers.png", 100, 150, 50, 50);
		case 8: Level9.bb.disable = true; Level9.changeCrop("resources/numbers.png", 150, 150, 50, 50);
		case 9: Level10.bb.disable = true; Level10.changeCrop("resources/numbers.png", 200, 150, 50, 50);
		case 10: break;
		}
		
		mmVar.sequence = LEVELLOOP;

	} break;
	case LEVELLOOP: {
		
		updateAllObjects();
		renderAllObjects();

		LevelButton(Level1, 1);
		LevelButton(Level2, 2);
		LevelButton(Level3, 3);
		LevelButton(Level4, 4);
		LevelButton(Level5, 5);
		LevelButton(Level6, 6);
		LevelButton(Level7, 7);
		LevelButton(Level8, 8);
		LevelButton(Level9, 9);
		LevelButton(Level10, 10);

		if (Back.bb.released || pressed(ESC)) {
			destroyAllObjects();
			mmVar.sequence = MMINIT;
		}

	} break;
	case TRANSITIONOUT: {
		mmVar.tsY += mmVar.tsRate * dt;
		mmVar.tsRate += 1080 * dt;

		renderCustomBG(dt, false);
		renderAllObjects();
		renderRect(0, max(mmVar.tsY, 0), 480, 270, 0);
		
		if (mmVar.tsY <= 0) {
			destroyAllObjects();
			currentScreen = GAME;
		}
	} break;
	}
	
}