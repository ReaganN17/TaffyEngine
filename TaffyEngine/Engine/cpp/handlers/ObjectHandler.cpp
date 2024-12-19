#include "../../include/TaffyEngine.h"

ObjectHandler ObjectHandler::instance;

ObjectHandler& ObjectHandler::getInstance() {
	return instance;
}

void ObjectHandler::renderAllObjects() {
	for (auto it : objects) {
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

void ObjectHandler::addObject(Object* obj, u8 z) {
	removeObject(obj);

	objects[z]->push_back(obj);
}
void ObjectHandler::removeObject(Object* obj) {
	for (auto it : objects) {
		auto key = std::find(it->begin(), it->end(), obj);
		if (key != it->end()) it->erase(key);
	}
}
