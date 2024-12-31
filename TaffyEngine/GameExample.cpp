#include "GameExample/LogoIntro.cpp"
#include "GameExample/GameLobby.cpp"


enum Screen {
	LOGO_INTRO,
	GAME_LOBBY
};

struct GameExample {

	Screen cur_screen = LOGO_INTRO;

	void configureSprites() {
		SpriteHandler::addSprite("Taffy_Logo", new Sprite("Resources/taffyEngine.png"));
		SpriteHandler::addSprite("Moving_BG_0", new Sprite("Resources/Circles.png"));
		SpriteHandler::addSprite("Moving_BG_1", new Sprite("Resources/Triangles.png"));
		SpriteHandler::addSprite("Moving_BG_2", new Sprite("Resources/Squares.png"));
		SpriteHandler::addSprite("Title", new Sprite("Resources/MegaSpriteSheet.png", CropInfo(490, 680, 463, 281)));

		SpriteHandler::addSprite("LevelSelect_Button", new Sprite("Resources/MegaSpriteSheet.png", CropInfo(277, 560, 174, 49)));
		SpriteHandler::addSprite("Options_Button", new Sprite("Resources/MegaSpriteSheet.png", CropInfo(452, 560, 174, 49)));
		SpriteHandler::addSprite("Credits_Button", new Sprite("Resources/MegaSpriteSheet.png", CropInfo(277, 611, 174, 49)));
		SpriteHandler::addSprite("Controls_Button", new Sprite("Resources/MegaSpriteSheet.png", CropInfo(452, 611, 174, 49)));
	}

	void configureBindings() {

	}

	void init() {

		configureSprites();
		EventHandler::scheduleEvent((new LogoIntro())->andThen({ new GameLobby()}));
	}

	void play() {

	}

	
};