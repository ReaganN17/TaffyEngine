
enum State {
	IDLE, PATROL, LOCATE
};

struct PatrolArea {
	
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

	Grid*& scan(u8 r);
	Grid*& unscan(u8 r);
};



//obligatory constructor
BasicEnemy::BasicEnemy() {}

//obligatory destructor
BasicEnemy::~BasicEnemy() { if (!ob.instance) return; }

BasicEnemy::BasicEnemy(Grid* grid, u16 x, u16 y, zLayer z, u8 id) : SentientGO(grid, x, y, z, "resources/bob.png", 0.75, 3) {
	health = 250;
	state = IDLE;
	speed = 20;
	range = 1;
	power = 5;
}

BasicEnemy& BasicEnemy::move(u8 dir, float dt) {
	switch (mmb.sequence) {
		case SELECTION: {
			//moves if given direction is a valid direction
			if (dir < 4) {
				mmb.direction = dir; 
				mmb.sequence = MOVEINIT;
			}
		} break;

		default: {
			SentientGO::move(dt, 0.75);
		} break;
	}

	

	return*this;
}

BasicEnemy& BasicEnemy::update(float dt) {
	if ((mmb.sequence == MOVEEND || moves.empty() == 1)) {producePath();}

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
	grid->setStart(xG, yG);

	scan(2);
	grid->createPath();
	grid->createDirections(&moves);
	unscan(2);

	return *this;
}


Grid*& BasicEnemy::scan(u8 r) {
	for (int x = -r; x <= r; x++) {
		for (int y = -r; y <= r; y++) {
			if (grid->containsID(xG + x, yG + y, id)) { grid->nodes[xG + x + (yG + y) * grid->gw].bObstacle = true; }
		}
	}


	return grid;
}

Grid*& BasicEnemy::unscan(u8 r) {
	for (int x = -r; x <= r; x++) {
		for (int y = -r; y <= r; y++) {
			if (grid->containsID(xG + x, yG + y, id)) { grid->nodes[xG + x + (yG + y) * grid->gw].bObstacle = false; }
		}
	}

	return grid;
}





