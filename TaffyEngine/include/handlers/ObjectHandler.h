#pragma once

#include "objects/Object.h"
#include "../utils/Utils.h"


/*
ObjectHandler Class

Contains and manages all registered objects
*/
class ObjectHandler final {

	friend class SpriteHandler;
	friend class EngineBase;

private:
	static std::vector<Object*> FarBack_0;
	static std::vector<Object*> Back_1;
	static std::vector<Object*> Middle_2;
	static std::vector<Object*> Front_3;
	static std::vector<Object*> FarFront_4;
	static std::vector<Object*> NoRender_5;

	static std::vector<Object*>* render_objects[5];
	static std::vector<Object*>* objects[6];

	//Renders all renderable objects
	static void renderAllObjects();

	//Updates all objects
	static void updateAllObjects();

	ObjectHandler() = delete;
public:

	/**
	* Adds Object to Object List; placed by its Z value
	*
	* @param Object to add
	*/
	static void addObject(Object* obj);

	/**
	* Removes Object to Object List; placed by its Z value
	*
	* @param Object to add
	*/
	static void removeObject(Object* obj);

	//Destroys all objects and clears Object list
	static void destroyAllObjects();
};
