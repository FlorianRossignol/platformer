#pragma once
#include "player_character.h"
#include "boss.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "box2d/b2_world.h"
#include "SFML/Audio/Music.hpp"
#include "user_data.h"
#include "contactlistener.h"
#include "platform.h"


class Game
{
public:
    Game();
    void Init();
    void Loop();
    b2Body* CreateBody(const b2BodyDef& bodyDef);
    void BeginContact(UserDataType userData, UserDataType userData1);
    void EndContact(UserDataType userData, UserDataType userData1);
    const float pixeltoMeter = 100.0f;
private:
    const float fixedTimestep_ = 0.02f;
    float FixedTimer_ = 0.0f;
	
    sf::Music music_;
    sf::RenderWindow window_;
    sf::Texture espace_;
    sf::Sprite sprite_;
    b2World world_{b2Vec2(0.0f, 9.81f)};
    PlayerCharacter player_;
    Boss boss_;

    MyContactListener contactListener_;
    Platform platform_;
};
