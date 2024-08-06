//Directions
enum Direction {
	MRIGHT, MUP, MLEFT, MDOWN
};

//Node struct
struct Node {
	bool bObstacle = false;
	bool bVisited = false;
	float fGlobalGoal;
	float fLocalGoal;
	u16 x;
	u16 y;
	vector<Node*> vecNeighbours;
	vector<Object*> occupants;
	Node* parent;
};

//Grid Header

struct Grid : Object{
	Node* nodes = nullptr;
	u16 gw = 0, gh = 0;
	float dx = 0, dy = 0;
	float grid_scale = 1;

	Node* nodeStart = nullptr;
	Node* nodeEnd = nullptr;

	Grid();
	~Grid();

	Grid(float x, float y, float pixelsPerSqr, const char* gridMap, float offsetX, float offsetY, float pixelsPerSqrD, const char* displayMap, Object* wall, zLayer z);

	Grid& PNGToGrid(const char* filename, Object* wall);
	Grid& createEmpty(int w, int h);
	Grid& createNeighbors(u8 range);

	Node*& setStart(u16 x, u16 y);
	Node*& setEnd(u16 x, u16 y);

	float heuristic(Node* a, Node* b);
	Node*& reset();
	bool createPath();
	void createDirections(list<u8>* dir);

	Node*& addObj(u16 x, u16 y, Object* guh);
	Node*& removeObj(u16 x, u16 y, Object* guh);

	Object* getTObj(u16 x, u16 y);
	Object* getTObjVec(u8 dir, float mag, u16 xG, u16 yG);

	Object* getIDObj(u16 x, u16 y, u8 id);
	Object* getIDObjVec(u8 dir, float mag, u8 id, u16 xG, u16 yG);

	bool checkID(u16 x, u16 y, u8 id);
	bool checkIDVec(u8 dir, float mag, u8 id, u16 xG, u16 yG);

	bool checkValid(int x, int y, initializer_list<u8> ids);
	bool checkValidVec(u8 dir, float mag, initializer_list<u8> ids, u16 xG, u16 yG);

	float getHighWidthObst(u16 x, u16 y, initializer_list<u8> ids);
	float getHighWidthObstVec(u8 dir, float mag, initializer_list<u8> ids, u16 xG, u16 yG);
};

//End of Headers

#include "gridobject.cpp"

//Obligatory constructor
Grid::Grid() {}

//Obligatory destructor
Grid::~Grid() { if (!ob.instance) return; }

//Actual Constructor
Grid::Grid(float x, float y, float pixelsPerSqr, const char* gridMap, float offsetX, float offsetY, float pixelsPerSqrD, const char* displayMap, Object* wall, zLayer z = BACK) : Object(0, 0, displayMap, z) {
	PNGToGrid(gridMap, wall);
	w = gw, h = gh;

	grid_scale = pixelsPerSqr, dx = x + grid_scale * 0.5, dy = y - grid_scale * 0.5;
	ob.layer = z;

	setScale(w * pixelsPerSqr / pixelsPerSqrD, h * pixelsPerSqr / pixelsPerSqrD);
	setPos(x + w * (0.5 + offsetX), y - h * (0.5 + offsetY));

	ob.cameraLinked = true;
}


//Translate PNG to grid
Grid& Grid::PNGToGrid(const char* file, Object* wall) {
	Image ref(file);

	createEmpty(ref.w, ref.h);

	for (u16 i = 0; i < gw * gh; i++) {
		
		//learn how to do this better then fix
		if (ref.data[ref.channels * (i)] == 0) {
			nodes[i].bObstacle = true;
			nodes[i].occupants.push_back(wall);
		}
	}

	return *this;
}


//Create Array of Nodes
Grid& Grid::createEmpty(int w, int h) {
	if (nodes != nullptr) delete[] nodes;

	gw = w;
	gh = h;

	nodes = new Node[gw * gh];

	//initlize nodes
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++)
		{
			nodes[y * gw + x].x = x;
			nodes[y * gw + x].y = y;
			nodes[y * gw + x].bObstacle = false;
			nodes[y * gw + x].parent = nullptr;
			nodes[y * gw + x].bVisited = false;
		}
	}

	//create neighbors
	createNeighbors(1);

	return *this;
}

Grid& Grid::createNeighbors(u8 range) {
	for (int i = 1; i <= range; i++) {
		for (u16 x = 0; x < gw; x++) {
			for (u16 y = 0; y < gh; y++)
			{
				if (y > (i - 1))
					nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - i) * gw + (x + 0)]);
				if (y < gh - i)
					nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + i) * gw + (x + 0)]);
				if (x > (i - 1))
					nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x - i)]);
				if (x < gw - i)
					nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x + i)]);
			}
		}
	}

	

	return *this;
}

//set start node
Node*& Grid::setStart(u16 x, u16 y) {
	x = min(gw - 1, x);
	y = min(gh - 1, y);

	nodeStart = &nodes[x + y * gw];

	return nodeStart;
}

//set goal node
Node*& Grid::setEnd(u16 x, u16 y) {
	x = min(gw - 1, x);
	y = min(gh - 1, y);

	nodeEnd = &nodes[x + y * gw];

	return nodeEnd;
}


//calculate heuristic (for path planning)
float Grid::heuristic(Node* a, Node* b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

//Reset Connections
Node*& Grid::reset() {
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++) {
			nodes[y * gw + x].bVisited = false;
			nodes[y * gw + x].fGlobalGoal = INFINITY;
			nodes[y * gw + x].fLocalGoal = INFINITY;
			nodes[y * gw + x].parent = nullptr;
		}
	}
	return nodes;
}

//Create Path
bool Grid::createPath() {
	reset();

	//start at the started node
	Node* nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	//create list of explored but untested nodes
	list<Node*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) //first path found is the path that is taken
	{
		// sort by lowest heurisitc
		listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		// clear notTestNodes of any visited nodes
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		//if non left in the list break
		if (listNotTestedNodes.empty())
			break;

		//check node as visited (because we are visiting them)
		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true;

		//go through each of the nodes neighbors (i didnt wanna do diagnol)
		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			//obstacle check
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			//calculate distance (and if lower distance to current distance)
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + heuristic(nodeCurrent, nodeNeighbour);

			// update distance if it was lower
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// pou16 the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.

				//yea what that guy said ^ (credit to javidx9 on Youtube for helping me learn and code A*)
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}

	return nodeEnd->parent != nullptr;
}


//Produce Directions
void Grid::createDirections(list<u8>* directions) {
	Node* curNode = nodeEnd;

	directions->clear();
	while (curNode->parent != nullptr) {
		if (curNode->parent->x > curNode->x) { directions->push_front(MLEFT); }
		if (curNode->parent->x < curNode->x) { directions->push_front(MRIGHT); }
		if (curNode->parent->y > curNode->y) { directions->push_front(MUP); }
		if (curNode->parent->y < curNode->y) { directions->push_front(MDOWN); }

		curNode = curNode->parent;
	}
}


Node*& Grid::removeObj(u16 x, u16 y, Object* guh) {
	x = min(x, gw - 1), y = min(y, gh - 1);

	auto it = find(nodes[x + y * gw].occupants.begin(), nodes[x + y * gw].occupants.end(), guh);

	if (it != nodes[x + y * gw].occupants.end()) {
		nodes[x + y * gw].occupants.erase(it);
	}

	return nodes;
}

Node*& Grid::addObj(u16 x, u16 y, Object*guh) {
	x = min(x, gw - 1), y = min(y, gh - 1);

	nodes[x + y * gw].occupants.push_back(guh);

	return nodes;

}

//returns the top most Object
Object* Grid::getTObj(u16 x, u16 y) {
	x = min(x, gw - 1), y = min(y, gh - 1);

	if (nodes[x + y * gw].occupants.empty() == 1) return nullptr;

	return nodes[x + y * gw].occupants.back();
}

Object* Grid::getTObjVec(u8 dir, float mag, u16 xG = 0, u16 yG = 0) {
	switch (dir) {
		case MLEFT: return getTObj(xG - mag, yG);
		case MRIGHT: return getTObj(xG + mag, yG);
		case MUP: return getTObj(xG, yG - mag);
		case MDOWN: return getTObj(xG, yG + mag);
	}

	return nullptr;
}

Object* Grid::getIDObj(u16 x, u16 y, u8 id) {
	x = min(x, gw - 1), y = min(y, gh - 1);

	GridObject* guh;

	for (auto it : nodes[x + y * gw].occupants) {
		guh = (GridObject*)it;
		if (guh->id == id) { return it; }
	}
	
	return nullptr;
}

Object* Grid::getIDObjVec(u8 dir, float mag, u8 id, u16 xG, u16 yG) {
	switch (dir) {
	case MLEFT: return getIDObj(xG - mag, yG, id);
	case MRIGHT: return getIDObj(xG + mag, yG, id);
	case MUP: return getIDObj(xG, yG - mag, id);
	case MDOWN: return getIDObj(xG, yG + mag, id);
	}

	return nullptr;
}


bool Grid::checkID(u16 x, u16 y, u8 id) {
	x = min(x, gw - 1), y = min(y, gh - 1);

	vector<Object*> guh(nodes[x + y * gw].occupants);

	if (id == 0) { return guh.size() == 0; }

	for (auto i : guh) {
		GridObject* h = (GridObject*)(i);
		if (h->id == id) { return true; }
	}

	return false;
}

bool Grid::checkIDVec(u8 dir, float mag, u8 id, u16 xG = 0, u16 yG = 0) {
	switch (dir) {
	case MLEFT: return checkID(xG - mag, yG, id);
	case MRIGHT: return checkID(xG + mag, yG, id);
	case MUP: return checkID(xG, yG - mag, id);
	case MDOWN: return checkID(xG, yG + mag, id);
	}

	return false;
}

bool Grid::checkValid(int x, int y, initializer_list<u8> ids) {
	if (x < 0 || y < 0 || x >= gw || y >= gh) { return false; }

	GridObject* guh;
	for (auto i : nodes[x + y * gw].occupants) {
		guh = (GridObject*)i;

		if (vecContainsBool(guh->id, ids)) return false;
	}

	return true;
}

bool Grid::checkValidVec(u8 dir, float mag, initializer_list<u8> ids, u16 xG, u16 yG) {
	switch (dir) {
	case MLEFT: return checkValid(xG - mag, yG, ids);
	case MRIGHT: return checkValid(xG + mag, yG, ids);
	case MUP: return checkValid(xG, yG - mag, ids);
	case MDOWN: return checkValid(xG, yG + mag, ids);
	}

	return false;
}

float Grid::getHighWidthObst(u16 x, u16 y, initializer_list<u8> ids) {
	x = min(x, gw - 1), y = min(y, gh - 1);
	float width = 0;
	GridObject* pointah;

	for (auto it : nodes[x + y * gw].occupants) {
		pointah = (GridObject*)it;

		if (vecContainsBool(pointah->id, ids)) width = max(width, (float) pointah->scale / 1000.f);
	}

	return width;
}

float Grid::getHighWidthObstVec(u8 dir, float mag, initializer_list<u8> ids, u16 xG, u16 yG) {
	switch (dir) {
	case MLEFT: return getHighWidthObst(xG - mag, yG, ids);
	case MRIGHT: return getHighWidthObst(xG + mag, yG, ids);
	case MUP: return getHighWidthObst(xG, yG - mag, ids);
	case MDOWN: return getHighWidthObst(xG, yG + mag, ids);
	}

	return 0;
}