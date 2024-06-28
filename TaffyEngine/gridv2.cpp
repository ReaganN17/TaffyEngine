//Directions
enum Direction {
	MRIGHT, MUP, MLEFT, MDOWN
};

//Node struct
struct Node {
	bool bObstacle = false;
	bool bVisited = false;
	u8 occupied = 0;
	float fGlobalGoal;
	float fLocalGoal;
	u16 x;
	u16 y;
	vector<Node*> vecNeighbours;
	Node* parent;
};


//Grid Header

struct Grid : Object{
	Node* nodes;
	u16 gw = 0, gh = 0;
	int dx = 0, dy = 0;
	float grid_scale = 1;

	Node* nodeStart = nullptr;
	Node* nodeEnd = nullptr;

	Grid();
	~Grid();

	Grid(int x, int y, float pixelsPerSqr, const char* gridMap, int offsetX, int offsetY, float pixelsPerSqrD, const char* displayMap, zLayer z);

	Grid& PNGToGrid(const char* filename);
	Grid& createEmpty(int w, int h);

	Node*& setStart(u16 x, u16 y);
	Node*& setEnd(u16 x, u16 y);

	float heuristic(Node* a, Node* b);
	Node*& reset();
	Grid& createPath();
	void createDirections(list<u8>* dir);

	//temporary til better solution
	u8 getID(u16 x, u16 y);
	u8 getIDVector(u8 dir, float mag, u16 xG, u16 yG);
};

//End of Headers

//Obligatory constructor
Grid::Grid() {}

//Obligatory destructor
Grid::~Grid() { if (!ob.instance) return; }

//Actual Constructor
Grid::Grid(int x, int y, float pixelsPerSqr, const char* gridMap, int offsetX, int offsetY, float pixelsPerSqrD, const char* displayMap, zLayer  z = BACK) : Object(0, 0, displayMap, z) {
	PNGToGrid(gridMap);
	w = gw, h = gh;

	grid_scale = pixelsPerSqr, dx = x + grid_scale * 0.5, dy = y - grid_scale * 0.5;
	ob.layer = z;

	setScale(w * pixelsPerSqr / pixelsPerSqrD, h * pixelsPerSqr / pixelsPerSqrD);
	setPos(x + w * (0.5 + offsetX), y - h * (0.5 + offsetY));

	ob.cameraLinked = true;
}


//Translate PNG to grid
Grid& Grid::PNGToGrid(const char* file) {
	Image ref(file);

	createEmpty(ref.w, ref.h);

	for (u16 i = 0; i < gw * gh; i++) {
		nodes[i].bObstacle = (ref.data[ref.channels * (i)] == 0);
		
		//learn how to do this better then fix
		nodes[i].occupied = ((ref.data[ref.channels * (i)] == 0) ? 1 : 0);
	}

	return *this;
}


//Create Array of Nodes
Grid& Grid::createEmpty(int w, int h) {
	delete[] nodes;

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
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++)
		{
			if (y > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - 1) * gw + (x + 0)]);
			if (y < gh - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 1) * gw + (x + 0)]);
			if (x > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x - 1)]);
			if (x < gw - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x + 1)]);
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
Grid& Grid::createPath() {
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

	return *this;
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

//stupid stupid stupid
u8 Grid::getID(u16 x, u16 y) {
	return nodes[x + y * gw].occupied;
}

u8 Grid::getIDVector(u8 dir, float mag, u16 xG = 0, u16 yG = 0) {
	switch (dir) {
		case MLEFT: return getID(xG - mag, yG);
		case MRIGHT: return getID(xG + mag, yG);
		case MUP: return getID(xG, yG - mag);
		case MDOWN: return getID(xG, yG + mag);
	}
}



