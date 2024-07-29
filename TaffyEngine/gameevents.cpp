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

		eb.cond1 = false;
	}

	void loop() {
		if (!eb.cond1) {
			if (trans != nullptr && GetTickCount() - 1000 > loadtime) { load(); }

			if (level->eb.cond1 || level->eb.cond2) {
				trans = new Transition(&trans, true);
				trans->start();
				eb.cond1 = true;

			}
		}

		if (eb.cond1 && trans == nullptr && !eb.cond2) { loadtime = GetTickCount(); eb.cond2 = true; }

		if (eb.cond2 && GetTickCount() - 1000 > loadtime) {
			if (level->eb.cond1) { endNstart(); }
			if (level->eb.cond2) { endNtransfer(); }
		}
	}

	void load() {
		level->eb.running = true;
		trans->eb.running = true;
	}

	void endNtransfer() {
		level->end();
		*pointer = new MainScreen(pointer, 1);

		trans = new Transition(false);
		trans->start();

		BasicEvent::endNtransfer();

		(*pointer)->start();
	}

	void endNstart() {
		level->end();

		BasicEvent::endNstart();
	}
};