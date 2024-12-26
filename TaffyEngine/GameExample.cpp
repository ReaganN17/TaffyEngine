#include "GameExample/LogoIntro.cpp"
#include "GameExample/GameLobby.cpp"


enum Screen {
	LOGO_INTRO,
	GAME_LOBBY
};

struct GameExample {

	Screen cur_screen = LOGO_INTRO;
	SpriteHandler& sprites = SpriteHandler::getInstance();

	void configureSprites() {
		sprites.addSprite("Taffy_Logo", new Sprite("Resources/taffyEngine.png"));
		sprites.addSprite("Moving_BG_0", new Sprite("Resources/Circles.png"));
		sprites.addSprite("Moving_BG_1", new Sprite("Resources/Triangles.png"));
		sprites.addSprite("Moving_BG_2", new Sprite("Resources/Squares.png"));
		sprites.addSprite("Title", new Sprite("Resources/MegaSpriteSheet.png", CropInfo(490, 680, 463, 281)));
	}

	void configureBindings() {

	}

	void init() {

		configureSprites();
		EventHandler::getInstance().scheduleEvent((new LogoIntro())->andThen({ new GameLobby()}));
	}

	void play() {

	}

	
};