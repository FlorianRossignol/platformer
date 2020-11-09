#include "game.h"
#include "boss.h"
#include "platform.h"
#include <iostream>



#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Audio.hpp"

Game::Game() : player_(*this), platform_(*this), contactListener_(*this)
{

}

void Game::Init()
{
    world_.SetContactListener(&contactListener_);
    window_.create(sf::VideoMode(1280, 720), "SAE Platformer");
    window_.setVerticalSyncEnabled(true);
    if (!music_.openFromFile("data/sprites/Scattle - Knock Knock.ogg"))
    {
        std::cerr << "Could not load sound!\n";
    }
    music_.play();
    if (!espace_.loadFromFile("data/sprites/espace.jpg"))
    {
        std::cerr << "Could not load texture!\n";
    }
    sprite_.setTexture(espace_);
    sf::Vector2f targetSize(1280, 720);
    sprite_.setOrigin(targetSize.x /sprite_.getGlobalBounds().width,
        targetSize.y / sprite_.getGlobalBounds().height);
    player_.Init();
    platform_.Init();
    boss_.Init();
}

void Game::Loop()
{
    sf::Clock gameClock_;
    while(window_.isOpen())
    {
        auto dt = gameClock_.restart();
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window_.close();
                return;
            }

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window_.setView(sf::View(visibleArea));
            }
        }

        FixedTimer_ += dt.asSeconds();
        while (FixedTimer_ > fixedTimestep_)
        {
            world_.Step(fixedTimestep_, 8, 3);
            FixedTimer_ -= fixedTimestep_;
        }

       
        player_.Update(dt.asSeconds());
       /* auto view = window_.getView();
        view.setCenter(player_.GetPosition());
        window_.setView(view);*/
    	
        window_.clear();
        const auto windowsSize = window_.getSize();
        sprite_.setPosition(windowsSize.x / 2.0f, windowsSize.y / 2.0f);
        window_.draw(sprite_);
     
        player_.Render(window_);
        platform_.Render(window_);
        boss_.Render(window_);

    	window_.display();
    }
}

b2Body* Game::CreateBody(const b2BodyDef& bodyDef)
{
    return world_.CreateBody(&bodyDef);
}

void Game::BeginContact(UserDataType userData, UserDataType userData1)
{
    if ((userData == UserDataType::PLATFORM && userData1 == UserDataType::PLAYER_FOOT) ||
        (userData1 == UserDataType::PLATFORM && userData == UserDataType::PLAYER_FOOT)
        )
    {
        player_.BeginContactGround();
    }
}

void Game::EndContact(UserDataType userData, UserDataType userData1)
{
    if ((userData == UserDataType::PLATFORM && userData1 == UserDataType::PLAYER_FOOT) ||
        (userData1 == UserDataType::PLATFORM && userData == UserDataType::PLAYER_FOOT))
    {
        player_.EndContactGround();
    }
}
