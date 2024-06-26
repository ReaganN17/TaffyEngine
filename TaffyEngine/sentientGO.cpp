enum movementSeq {
	SELECTION,
	MOVEINIT, MOVE,
	MOVEINTERUPTINIT, MOVEINTERUPT,
	BOUNCEINIT, BOUNCEMOVE,
	MOVEEND
};

union MovementByte {
	u8 byte = 0;

	struct {
		u8 sequence : 6;
		u8 direction : 2;
	};
};


//idk how else to name an object that moves around like a player
struct SentientGO : GridObject {
	SentientGO();
	~SentientGO();

	SentientGO(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, float scale, u8 id);
	SentientGO(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, u16 cx, u16 cy, u16 cw, u16 ch, float scale, u8 id);

	//moving purposes
	float pos;
	float acc;
	float rate = 0;
	float posborder;
	u8 distance;
	MovementByte mmb;

	//characteristics
	short health = 0;
	u8 state = 0;
	u8 power = 0;
	u8 range = 0;
	u8 speed = 0;

	virtual SentientGO& move(float dt);

	//moving functions
	virtual SentientGO& moveinit();

	virtual SentientGO& takeDamage(short base);
	virtual SentientGO& toPos(int x, int y);

	virtual bool checkValidVector(u8 dir, float mag);
	virtual bool checkValid(int x, int y);
};

#include "player.cpp"
#include "enemy.cpp"

//obligatory constructor
SentientGO::SentientGO() {}

//obligatory destructor
SentientGO::~SentientGO() { if (!ob.instance) return; }

//construct basic sprite
SentientGO::SentientGO(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, float scale, u8 id)
	: GridObject(grid, x, y, sprite, scale, z, id) {}

//construct from sprite sheet
SentientGO::SentientGO(Grid* grid, u16 x, u16 y, zLayer z, const char* sprite, u16 cx, u16 cy, u16 cw, u16 ch, float scale, u8 id)
	: GridObject(grid, x, y, sprite, cx, cy, cw, ch, scale, z, id) {}

SentientGO& SentientGO::move(float dt) {
	//4 directional movement
	switch (mmb.sequence) {
	case SELECTION: {

	} break;

	case MOVEINIT: {
		moveinit();
		mmb.sequence = MOVE;
	}

	case MOVE: {
		if (!checkValidVector(mmb.direction, 1)) { mmb.sequence = MOVEINTERUPTINIT; break; }

		grid->grid[xG + yG * grid->gw] = overlapped;

		//kinematics (sorta)
		pos += rate * dt + acc * dt * dt * 0.5f;
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

		if (rate <= 0) { mmb.sequence = MOVEEND; }
	} break;

	case MOVEINTERUPTINIT: {
		float b;

		switch (grid->getIDVector(mmb.direction, 1, xG, yG)) {
			case 1: b = 1; break;
			case 2: b = 0.65; break;
			case 3: b = 0.75; break;
			default: b = 1; break;
		}

		posborder = 0.5 * (2- b - (float)scale / 1000) + 0.5;

		mmb.sequence = MOVEINTERUPT;
	} break;

	case MOVEINTERUPT: {
		pos += rate * dt + acc * dt * dt * 0.5f;
		rate += acc * dt;

		setGridVector(mmb.direction, pos - 0.5);
		if (pos >= posborder) { mmb.sequence = BOUNCEINIT; setGridVector(mmb.direction, posborder - 0.5); break; }

		if (rate <= 0) { mmb.sequence = MOVEEND; }
	} break;

	case BOUNCEINIT: {
		float factor = 0.1;

		acc = 2 * (posborder - 0.5) / (factor * factor);
		rate = -2 * (posborder - 0.5) / (factor);

		pos = posborder;

		mmb.sequence = BOUNCEMOVE;

	}break;

	case BOUNCEMOVE: {
		pos += rate * dt + acc * dt * dt * 0.5f;
		rate += acc * dt;

		setGridVector(mmb.direction, pos - 0.5);

		if (rate >= 0) { mmb.sequence = MOVEEND; }
	} break;

	case MOVEEND: {
		speed = 0;
		rate = 0;
		acc = 0;
		range = 1;
		setGridVector(mmb.direction, 0);
		mmb.sequence = SELECTION;
	}break;
	}

	return *this;
}

SentientGO& SentientGO::moveinit() {
	distance = range;
	pos = 0.5;

	rate = 2 * ((float)speed / 10.f) * distance;
	acc = -2 * distance * ((float)speed / 10.f) * ((float)speed / 10.f);

	return*this;
}

SentientGO& SentientGO::takeDamage(short base) {
	health -= base;

	return *this;
}

SentientGO& SentientGO::toPos(int x, int y) {
	grid->grid[xG + yG * grid->gw] = overlapped;
	xG = x, yG = y;
	overlapped = grid->grid[xG + yG * grid->gw];
	grid->grid[xG + yG * grid->gw] = id;

	return *this;
}

bool SentientGO::checkValidVector(u8 dir, float mag) {
	switch (dir) {
		case MLEFT: return checkValid(xG - mag, yG);
		case MRIGHT: return checkValid(xG + mag, yG);
		case MUP: return checkValid(xG, yG - mag);
		case MDOWN: return checkValid(xG, yG + mag);
	}
}

bool SentientGO::checkValid(int x, int y) {
	if (grid->grid[x + y * grid->gw] > 0) return false;
	if (x < 0 || x >= grid->gw) return false;
	if (y < 0 || y >= grid->gh) return false;
	return true;
}
