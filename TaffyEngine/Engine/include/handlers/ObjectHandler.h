#pragma once

#include "SpriteHandler.h"
#include "objects/Object.h"

struct ObjectHandler {

	friend SpriteHandler;
private:
	static ObjectHandler instance;

	ObjectHandler() {}

	std::vector<Object*> FarBack_0;
	std::vector<Object*> Back_1;
	std::vector<Object*> Middle_2;
	std::vector<Object*> Front_3;
	std::vector<Object*> FarFront_4; 
	std::vector<Object*> NoRender_5;

	std::vector<Object*>* render_objects[5] = {&FarBack_0, &Back_1, &Middle_2, &Front_3, &FarFront_4};
	std::vector<Object*>* objects[6] = { &FarBack_0, &Back_1, &Middle_2, &Front_3, &FarFront_4, &NoRender_5 };

public:
	static ObjectHandler& getInstance();

	void renderAllObjects();
	void destroyAllObjects();
	void updateAllObjects();

	void addObject(Object* obj, u8 z);
	void removeObject(Object* obj);
};
