enum MoveType {
	Linear, Quadratic,
};


struct ObjectMove : BasicEvent {
	Object* obj;
	float xrate, yrate, xacc, yacc, xpos, ypos;
	int xgoal, ygoal;

	ObjectMove() {}
	~ObjectMove() {}

	void init(Object* obj, int x, int y, float time) {
		if (eb.instance && !eb.restart) { return; }
		if (eb.instance && eb.restart) { interrupt(); }

		events.push_back(&*this);
		eb.instance = true;
		eb.running = true;
		eb.interupt = false;

		this->obj = obj;
		xpos = obj->x, ypos = obj->y;
		xrate = (x - obj->x) * 1.f / time;
		yrate = (y - obj->y) * 1.f / time;
		xacc = 0;
		yacc = 0;
		xgoal = x, ygoal = y;

	}

	void loop() {
		if (!eb.running) return;
		if (eb.interupt) { interrupt(); return; }

		xpos += xrate * dt + 0.5 * xacc * dt * dt;
		ypos += yrate * dt + 0.5 * yacc * dt * dt;

		xrate += xacc * dt, yrate += yacc * dt;
		
		obj->setPos(xpos , ypos);

		bool reachedXGoal = (obj->x >= xgoal && xrate >= 0) || (obj->x <= xgoal && xrate <= 0);
		bool reachedYGoal = (obj->y >= ygoal && yrate >= 0) || (obj->y <= ygoal && yrate <= 0);

		if (reachedXGoal && reachedYGoal) {
			end();
		}
	}

};