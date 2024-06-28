
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
	void scanGrid();
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
			//moves if given direction is a valid direction
			if (dir < 4) {
				mmb.direction = dir; 
				mmb.sequence = MOVEINIT;
			}
		} break;

		case MOVE: {
			if (!checkValidVector(mmb.direction, 1)) { mmb.sequence = MOVEINTERUPTINIT; break; }

			grid->nodes[xG + yG * grid->gw].occupied = overlapped;
			grid->nodes[xG + yG * grid->gw].bObstacle = false;

			//kinematics (sorta)
			pos += rate * dt + acc * dt * dt * 0.5f;
			rate += acc * dt;

			switch (mmb.direction) {
			case MLEFT: xG -= floor(pos); break;
			case MRIGHT: xG += floor(pos); break;
			case MUP: yG -= floor(pos); break;
			case MDOWN: yG += floor(pos); break;
			}

			overlapped = grid->nodes[xG + yG * grid->gw].occupied;
			grid->nodes[xG + yG * grid->gw].occupied = id;
			grid->nodes[xG + yG * grid->gw].bObstacle = true;


			pos = fmod(pos, 1);

			setGridVector(mmb.direction, pos - 0.5);

			if (rate <= 0) { mmb.sequence = MOVEEND; }
		} break;

		default: {
			SentientGO::move(dt, 0.75);
		} break;
	}

	

	return*this;
}

BasicEnemy& BasicEnemy::update(float dt) {
	if (mmb.sequence == MOVEEND || moves.empty() == 1) {producePath();}

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
	grid->createPath();
	grid->createDirections(&moves);

	return *this;
}

void BasicEnemy::scanGrid() {

}







