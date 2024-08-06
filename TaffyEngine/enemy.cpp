
enum State {
	IDLE, PATROL, LOCATE
};

struct PatrolArea {
	
};

struct BasicEnemy : GridObject {

	BasicEnemy();
	~BasicEnemy();
	BasicEnemy(Grid* grid, u16 x, u16 y, zLayer z, u8 id);

	//virtual void control(float dt);

	BasicEnemy& move(u8 dir);

	list<u8> moves = {};

	//bool onScreen();

	BasicEnemy& producePath();
	BasicEnemy& update();

};



//obligatory constructor
BasicEnemy::BasicEnemy() {}

//obligatory destructor
BasicEnemy::~BasicEnemy() { if (!ob.instance) return; }

BasicEnemy::BasicEnemy(Grid* grid, u16 x, u16 y, zLayer z, u8 id) : GridObject(grid, x, y, z, "resources/bob.png", 0.75, 3, CHARA) {
	health = 250;
	state = IDLE;
	speed = 10;
	range = 1;
	power = 5;
}

BasicEnemy& BasicEnemy::move(u8 dir) {

	

	

	return*this;
}

BasicEnemy& BasicEnemy::update() {


	return *this;
}

BasicEnemy& BasicEnemy::producePath() {
	grid->setStart(xG, yG);

	grid->createPath();
	grid->createDirections(&moves);

	return *this;
}







