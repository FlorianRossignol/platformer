#include "boss.h"
#include "game.h"
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/box2d.h>


 void Boss::Init()
{
	b2BodyDef bossdef;
	bossdef.type = b2_kinematicBody;
	
	b2FixtureDef bossfixture;
	bossfixture.restitution = 5.0f;
	b2PolygonShape bosspoly;
	bosspoly.SetAsBox(40.0f, 40.0f);
	if (!Idle_texture_.loadFromFile("data/sprites/trump.png"))
	{
		std::cerr << "Could no load textur\n";
	}
	sprite_.setTexture(Idle_texture_);
	const auto textureSize = Idle_texture_.getSize();
	sprite_.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
	sprite_.setPosition(1280.0f, 50.0f);
	sprite_.setScale(0.1f, 0.1f);
}

void Boss::Render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void Boss::Update(float dt)
{
	
}
