//header
union EventByte{
	unsigned char byte = 0;

	struct {
		u8 space : 4;
		bool restart : 1;
		bool running : 1;
		bool instance : 1;
		bool interupt : 1;
	};
};

struct BasicEvent {
	EventByte eb;
	
	BasicEvent();
	~BasicEvent();


	virtual void init();
	virtual void loop();
	virtual void end();
	virtual void interrupt();
};

//event handler
vector<BasicEvent*> events;

void runEvents() {
	for (auto i : events) {
		(i)->loop();
	}
}

void endEvents() {
	for (auto i : events) {
		(i)->interrupt();
	}
}

void pauseEvents() {
	for (auto i : events) {
		(i)->eb.running = false;
	}
}

void unpauseEvents() {
	for (auto i : events) {
		(i)->eb.running = true;
	}
}

//cpp file

BasicEvent::BasicEvent() {};
BasicEvent::~BasicEvent() {};

void BasicEvent::init() {
	if (!eb.instance) {
		events.push_back(&*this);
		eb.instance = true;
		eb.running = true;
		eb.interupt = false;
	}
	else if (eb.restart) {
		interrupt();
		events.push_back(&*this);
		eb.running = true;
		eb.instance = true;
		eb.interupt = false;
	}
}

void BasicEvent::loop() {
	if (!eb.running) return;
	if (eb.interupt) { interrupt(); return; }
}

void BasicEvent::end() {
	events.erase(find(events.begin(), events.end(), &*this));
	eb.instance = false;
	eb.running = false;
	
}

void BasicEvent::interrupt() {
	
	end();
}

