
#include "MoveBackground.cpp"

struct GameLobby : Event {
	Object title;
	Event* run_background = nullptr;

	GameLobby() {}

	void init() {
		new (&title) Object(0, 150, 400, 200, "Title", Z_BACK);

		run_background = (new MoveBackground());

		EventHandler::getInstance().scheduleEvent(run_background);
	}

	void loop() {
		
	}

	bool isFinished() {
		return false;
	}

	void end(bool interrupted) {
		run_background->cancel();
		run_background = nullptr;

		ObjectHandler::getInstance().removeObject(&title);
	}
};