#include "../../include/handlers/ObjectHandler.h"

std::vector<Object*>* ObjectHandler::render_objects[5] = { &FarBack_0, &Back_1, &Middle_2, &Front_3, &FarFront_4 };
std::vector<Object*>* ObjectHandler::render_objects[6] = { &FarBack_0, &Back_1, &Middle_2, &Front_3, &FarFront_4, &NoRender_5 };

void ObjectHandler::renderAllObjects() {
	for (auto it : render_objects) {
		for (auto obj : (*it)) {
			obj->render();
		}
	}
}
void ObjectHandler::destroyAllObjects() {
	for (auto it : objects) {
		for (auto obj : (*it)) {
			obj->~Object();
		}
		(it)->clear();
	}
}
void ObjectHandler::updateAllObjects() {
	for (auto it : objects) {
		for (auto obj : (*it)) {
			obj->update();
		}
	}
}

void ObjectHandler::addObject(Object* obj) {
	removeObject(obj);

	objects[obj->z_layer]->push_back(obj);
}
void ObjectHandler::removeObject(Object* obj) {
	for (auto it : objects) {
		auto key = std::find(it->begin(), it->end(), obj);
		if (key != it->end()) it->erase(key);
	}
}
