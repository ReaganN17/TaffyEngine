enum Direction {
	MRIGHT, MUP, MLEFT, MDOWN
};

struct sNode {
	bool bObstacle = false;
	bool bVisited = false;
	float fGlobalGoal;
	float fLocalGoal;
	u16 x;
	u16 y;
	vector<sNode*> vecNeighbours;
	sNode* parent;
};



struct Nodes {
	Nodes();
	~Nodes();
	Nodes(u16 w, u16 h);

	sNode* nodes = nullptr;
	u16 nMWidth = 0;
	u16 nMHeight = 0;
	bool goalUpdated;

	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;

	sNode*& reset();
	Nodes& createEmpty();
	Nodes& createFromGrid(u8* huh, u16 w, u16 h);

	sNode*& setGoal(u16 x, u16 y);
	sNode*& setStart(u16 x, u16 y);

	Nodes& createPath();
	void createDirections(list<u8>* directions);
	float heuristic(sNode * a, sNode * b);
};



//maf
float Nodes::heuristic(sNode* a, sNode* b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

//construction
Nodes::Nodes() {}

//construction with size
Nodes::Nodes(u16 w,  u16 h) : nMWidth(w), nMHeight(h) {}

//am i deconstructing this right?
Nodes::~Nodes() {
	delete[] nodes;
	nodes = nullptr;
	nodeStart = nullptr;
	nodeEnd = nullptr;
}

//create plain empty field
Nodes& Nodes::createEmpty() {
	delete[] nodes;

	nodes = new sNode[nMWidth * nMHeight];

	for (u16 x = 0; x < nMWidth; x++) {
		for (u16 y = 0; y < nMHeight; y++)
		{
			nodes[y * nMWidth + x].x = x; // ...because we give each node its own coordinates
			nodes[y * nMWidth + x].y = y;
			nodes[y * nMWidth + x].bObstacle = false;
			nodes[y * nMWidth + x].parent = nullptr;
			nodes[y * nMWidth + x].bVisited = false;
		}
	}

	for (u16 x = 0; x < nMWidth; x++) {
		for (u16 y = 0; y < nMHeight; y++)
		{
			if (y > 0)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMWidth + (x + 0)]);
			if (y < nMHeight - 1)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMWidth + (x + 0)]);
			if (x > 0)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMWidth + (x - 1)]);
			if (x < nMWidth - 1)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMWidth + (x + 1)]);
		}
	}

	return *this;
}

//create node grid from Grid class
Nodes& Nodes::createFromGrid(u8* huh, u16 w, u16 h) {
	delete[] nodes;

	nMWidth = w;
	nMHeight = h;

	if (sizeof(huh) != w * h) { createEmpty(); return *this; }

	nodes = new sNode[nMWidth * nMHeight];

	for (u16 x = 0; x < nMWidth; x++) {
		for (u16 y = 0; y < nMHeight; y++)
		{
			nodes[y * nMWidth + x].x = x;
			nodes[y * nMWidth + x].y = y;
			nodes[y * nMWidth + x].bObstacle = huh[y * nMWidth + x] == 1;
			nodes[y * nMWidth + x].parent = nullptr;
			nodes[y * nMWidth + x].bVisited = false;
		}
	}

	//4 directional connections (neighbours)
	for (u16 x = 0; x < nMWidth; x++) {
		for (u16 y = 0; y < nMHeight; y++)
		{
			if (y > 0)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMWidth + (x + 0)]);
			if (y < nMHeight - 1)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMWidth + (x + 0)]);
			if (x > 0)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMWidth + (x - 1)]);
			if (x < nMWidth - 1)
				nodes[y * nMWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMWidth + (x + 1)]);
		}
	}

	return *this;
}

//reset connections
//does not clear obstacles
sNode*& Nodes::reset() {
	for (u16 x = 0; x < nMWidth; x++) {
		for (u16 y = 0; y < nMHeight; y++) {
			nodes[y * nMWidth + x].bVisited = false;
			nodes[y * nMWidth + x].fGlobalGoal = INFINITY;
			nodes[y * nMWidth + x].fLocalGoal = INFINITY;
			nodes[y * nMWidth + x].parent = nullptr;
		}
	}
	return nodes;
}

//set goal
sNode*& Nodes::setGoal(u16 x, u16 y) {
	nodeEnd = &nodes[x + y * nMWidth];

	return nodeEnd;
}

//set start (i think its that simple)
sNode*& Nodes::setStart(u16 x, u16 y) {
	nodeStart = &nodes[x + y * nMWidth];

	return nodeStart;
}

//create path from start to goal
Nodes& Nodes::createPath() {
	reset();

	//start at the started node
	sNode* nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	//create list of explored but untested nodes
	list<sNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) //first path found is the path that is taken
	{
		// sort by lowest heurisitc
		listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

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

//produce direction from path
void Nodes::createDirections(list<u8>* directions) {
	sNode* curNode = nodeEnd;

	directions->clear();
	while (curNode->parent != nullptr) {
		if (curNode->parent->x > curNode->x) { directions->push_front(MLEFT); }
		if (curNode->parent->x < curNode->x) { directions->push_front(MRIGHT); }
		if (curNode->parent->y > curNode->y) { directions->push_front(MUP); }
		if (curNode->parent->y < curNode->y) { directions->push_front(MDOWN); }

		curNode = curNode->parent;
	}
}


