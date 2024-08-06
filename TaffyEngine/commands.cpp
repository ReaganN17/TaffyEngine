struct GOMove : BasicEvent {
	GOMove() {}
	~GOMove() {}

	GridObject* obj;
	float pos, rate, acc, posborder, kickback;
	u8 dir;

	bool interrupted, bounce;

	GOMove(BasicEvent** pointer, GridObject* go, float kickback) : BasicEvent(pointer) { 
		obj = go; 
		this->kickback = kickback;
	}

	void init() {
		dir = obj->inst.direction;
		
		pos = 0.5;
		rate = 2 * (obj->speed / 10.f) * obj->range;
		acc = -2 * obj->range * (obj->speed / 10.f) * (obj->speed / 10.f);
		interrupted = false;
	}


	void loop() {
		if (!interrupted && !obj->checkValidVector(dir, 1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})) {
			interrupted = true;

			float b = obj->grid->getHighWidthObstVec(dir, 1, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, obj->xG, obj->yG);

			posborder = 0.5 * (2 - b - (float)obj->scale / 1000.f) + 0.5;
		}
		else if (interrupted && obj->checkValidVector(dir, 1, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 })) {
			interrupted = false;
		}

		//kinematics (sorta)
		pos += rate * dt + acc * dt * dt * 0.5f;
		rate += acc * dt;

		if (!interrupted) {
			switch (dir) {
			case MLEFT:
				if (obj->xG - floor(pos) != obj->xG) { obj->removeObj(); obj->xG -= floor(pos); obj->addObjCur(); }
				else { obj->xG -= floor(pos); }
				break;
			case MRIGHT:
				if (obj->xG + floor(pos) != obj->xG) { obj->removeObj(); obj->xG += floor(pos); obj->addObjCur(); }
				else { obj->xG += floor(pos); }
				break;
			case MUP:
				if (obj->yG - floor(pos) != obj->yG) { obj->removeObj(); obj->yG -= floor(pos); obj->addObjCur(); }
				else { obj->yG -= floor(pos); }
				break;
			case MDOWN:
				if (obj->yG + floor(pos) != obj->yG) { obj->removeObj(); obj->yG += floor(pos); obj->addObjCur(); }
				else { obj->yG += floor(pos); }
				break;
			}

			pos = fmod(pos, 1);
		}

		if (interrupted && pos >= posborder && !bounce) {
			float factor = kickback;

			acc = 2 * (posborder - 0.5) / (factor * factor);
			rate = -2 * (posborder - 0.5) / (factor);

			pos = posborder;

			bounce = true;
		}

		obj->setGridVector(dir, pos - 0.5);

		if (!bounce && rate <= 0) { end(); }
		if (bounce && rate >= 0) { end(); }
	}

	void end() {
		obj->setGridVector(dir, 0);

		BasicEvent::end();
	}
	
};

