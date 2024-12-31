
#include "MoveBackground.cpp"
#include "MyButton.cpp"
#include "MoveObject.cpp"

struct GameLobby : Event {
	Object title;
	Event* run_background = nullptr;

	MyButton Level_Select;
	MyButton Controls;
	MyButton Options;
	MyButton Credits;

	Camera myCamera;

	GameLobby() {}

	void init() {
		new (&title) Object(0, 150, 300, 150, "Title", Z_BACK);

		new(&Level_Select) MyButton(ScreenButton(Object(-200, -50, 300, 75, "LevelSelect_Button", Z_BACK), HELD));
		new(&Controls) MyButton(ScreenButton(Object(200, -50, 300, 75, "Controls_Button", Z_BACK), HELD));
		new(&Credits) MyButton(ScreenButton(Object(200, -150, 300, 75, "Credits_Button", Z_BACK), HELD));
		new(&Options) MyButton(ScreenButton(Object(-200, -150, 300, 75, "Options_Button", Z_BACK), HELD));

		title.connectCamera(myCamera);
		Controls.connectCamera(myCamera);
		Credits.connectCamera(myCamera);
		Options.connectCamera(myCamera);
		Level_Select.connectCamera(myCamera);
		
		run_background = (new MoveBackground());

		EventHandler::scheduleEvent(run_background);

		Controls::addTrigger(new Trigger(
			[&]() {return Level_Select.is_down(); },
			ON_TRUE,
			[&]() {return new InstantEvent([&]() {myCamera.setPos(1080, 0); }, {}); }));
	}

	void loop() {
		
	}

	bool isFinished() {
		return false;
	}

	void end(bool interrupted) {
		run_background->cancel();
		run_background = nullptr;

		ObjectHandler::removeObject(&title);
	}
};