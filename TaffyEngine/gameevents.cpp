//main game event structure

#include "levels.cpp"

struct GameEvent : BasicEvent {
	GameEvent() {}
	~GameEvent() {}
	GameEvent(BasicEvent** pointer) : BasicEvent(pointer) {}

	BasicEvent* level;
	BasicEvent* trans;

	long loadtime = 0;

	void init() {
		trans = new Transition(&trans, false);
		
		switch (curLevel) {
		case 1: level = new Level1Event(&level);  break;
		}

		loadtime = GetTickCount();

		level->start();
		level->eb.running = false;

		trans->start();
		trans->eb.running = false;
	}

	void loop() {
		if (trans != nullptr && GetTickCount() - 1000 > loadtime) { load(); }


	}

	void load() {
		level->eb.running = true;
		trans->eb.running = true;
	}

	void endNtransfer() {
		*pointer = new MainScreen(pointer);

		BasicEvent::endNtransfer();

	}
};