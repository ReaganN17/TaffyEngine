enum movementSeq {
	SELECTION, MOVEINIT, MOVE, MOVEINTERUPTINIT, MOVEINTERUPT, BOUNCEINIT, BOUNCEMOVE, MOVEEND
};

union MovementByte {
	u8 byte = 0;

	struct {
		u8 sequence : 6;
		u8 direction : 2;
	};
};

struct Player : GridObject {

	Player();
	~Player();
	Player(Grid* grid, u16 x, u16 y, const char* file, float scale, zLayer z);

	//moving purposes
	float pos;
	float acc;
	float rate;
	float posborder;
	u8 distance;
	MovementByte mmb;

	//player characteristics
	short health = 250;
	u8 state = 0;
	u8 power = 5;
	u8 range = 3;
	float speed = 5; //time between moves is 1/speed seconds;

	Player& move(PControls inputs, float dt);

	Player& toPos(int x, int y);
	bool checkValidVector(u8 dir, float mag);
	Player& toGridVector(u8 dir, u8 mag);
	Player& gridPushBack(u8 dir, u8 mag);

	bool checkValid(int x, int y);

	void render();
};

//obligatory constructor
Player::Player() {}

//obligatory destructor
Player::~Player() { if (!ob.instance) return; }

//actual constructor
Player::Player(Grid* grid, u16 x, u16 y, const char* file, float scale, zLayer z = MIDDLE) : GridObject(grid, x, y, file, scale, z, 2) {}

Player& Player::move(PControls inputs, float dt) {
	
	//4 directional movement
	switch (mmb.sequence) {
		case SELECTION: {
			if (pressed(inputs.left)) { mmb.direction = MLEFT; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.right)) { mmb.direction = MRIGHT; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.up)) { mmb.direction = MUP; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.down)) { mmb.direction = MDOWN; mmb.sequence = MOVEINIT; }
		} break;

		case MOVEINIT: {
			distance = range;
			pos = 0.5;

			rate = 2 * speed * distance;
			acc = -2 * distance * speed * speed;
			mmb.sequence = MOVE;
		}

		case MOVE: {
			grid->grid[xG + yG * grid->gw] = overlapped;
	
			//kinematics (sorta)
			pos +=  rate * dt + acc * dt * dt * 0.5f;
			rate += acc * dt;
		
			switch (mmb.direction) {
				case MLEFT: xG -= floor(pos); break;
				case MRIGHT: xG += floor(pos); break;
				case MUP: yG -= floor(pos); break;
				case MDOWN: yG += floor(pos); break;
			}
	
			overlapped = grid->grid[xG + yG * grid->gw];
			grid->grid[xG + yG * grid->gw] = id;


			pos = fmod(pos, 1);

			setGridVector(mmb.direction, pos - 0.5);

			if (!checkValidVector(mmb.direction, 1)) { mmb.sequence = MOVEINTERUPTINIT; }

			if (rate <= 0) { mmb.sequence = MOVEEND; setGridVector(mmb.direction, 0); }
		} break;

		case MOVEINTERUPTINIT: {
			float b;

			switch(grid->getIDVector(mmb.direction, 1, xG, yG)) {
				case 1: b = 1; break;
				default: b = -2; break;
			}
			
			posborder = 0.5 * (b - (float)scale/1000) + 0.5;

			mmb.sequence = MOVEINTERUPT;
		} break;

		case MOVEINTERUPT: {
			pos += rate * dt + acc * dt * dt * 0.5f;
			rate += acc * dt;

			setGridVector(mmb.direction, pos - 0.5);

			if (pos => posborder) { mmb.sequence = BOUNCEINIT; setGridVector(mmb.direction, posborder - 0.5);}

			if (rate <= 0) { mmb.sequence = MOVEEND; setGridVector(mmb.direction, 0); }
		} break;

		case BOUNCEINIT: {
			float factor = 0.1;
			
			acc = 2 * (posborder - 0.5) / (factor * factor);
			rate = -2 * (posborder - 0.5) / (factor);

			pos = posborder;

			mmb.sequence = BOUNCEMOVE;

		}break;

		case BOUNCEMOVE: {
			pos +=  rate * dt + acc * dt * dt * 0.5f;
			rate += acc * dt;

			setGridVector(mmb.direction, pos - 0.5);

			if (rate >= 0) { mmb.sequence = MOVEEND; setGridVector(mmb.direction, 0); }
		} break;

		case MOVEEND: {
			mmb.sequence = SELECTION;
		}break;
	}

	return *this;
}


Player& Player::toPos(int x, int y) {
	grid->grid[xG + yG * grid->gw] = overlapped;
	xG = x, yG = y;
	overlapped = grid->grid[xG + yG * grid->gw];
	grid->grid[xG + yG * grid->gw] = id;
	return *this;
}

bool Player::checkValidVector(u8 dir, float mag) {
	switch (dir) {
		case MLEFT: return checkValid(xG - mag, yG);
		case MRIGHT: return checkValid(xG + mag, yG);
		case MUP: return checkValid(xG, yG - mag);
		case MDOWN: return checkValid(xG, yG + mag);
	}
}

bool Player::checkValid(int x, int y) {
	if (grid->grid[x + y * grid->gw] > 0) return false;
	if (x < 0 || x >= grid->gw) return false;
	if (y < 0 || y >= grid->gh) return false;
	return true;
}

void Player::render() {
	//Image arrow("resources/bob.png");

	//renderImageV2(&arrow, this->x + 50, this->y, 50, 50);

	Object::render();
}