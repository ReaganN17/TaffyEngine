

enum State {
	IDLE, PATROL, LOCATE
};

struct BasicEnemy : SentientGO {

	BasicEnemy();
	~BasicEnemy();
	BasicEnemy(Grid* grid, u16 x, u16 y, zLayer z, u8 id);

	//virtual void control(float dt);

	BasicEnemy& move(u8 dir, float dt);

	list<u8> moves = {};

	//bool onScreen();

	BasicEnemy& producePath();
	BasicEnemy& update(float dt);
};



//obligatory constructor
BasicEnemy::BasicEnemy() {}

//obligatory destructor
BasicEnemy::~BasicEnemy() { if (!ob.instance) return; }

BasicEnemy::BasicEnemy(Grid* grid, u16 x, u16 y, zLayer z, u8 id) : SentientGO(grid, x, y, z, "resources/bob.png", 0.75, 3) {
	health = 250;
	state = IDLE;
	speed = 10;
	range = 1;
	power = 5;
}

BasicEnemy& BasicEnemy::move(u8 dir, float dt) {
	switch (mmb.sequence) {
		case SELECTION: {
			if (dir < 4) {
				mmb.direction = dir; 
				mmb.sequence = MOVEINIT;
			}
		} break;
		case MOVEINIT: {
			speed = 10;
			moveinit();
		}
		default: {
			SentientGO::move(dt);
		} break;
	}

	

	return*this;
}

BasicEnemy& BasicEnemy::update(float dt) {
	if (PathMap.goalUpdated) producePath();

	if (mmb.sequence == SELECTION && moves.empty() == 0) {
		move(moves.front(), dt);
		moves.pop_front();
	}
	else {
		move(4, dt);
	}


	return *this;
}

BasicEnemy& BasicEnemy::producePath() {
	PathMap.setStart(xG, yG);

	PathMap.createPath();
	PathMap.createDirections(&moves);

	return *this;
}







