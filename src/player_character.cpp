#define _USE_MATH_DEFINES
#include "player_character.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>
#include <cmath>
#include "game.h"
#include <box2d/box2d.h>


	PlayerCharacter::PlayerCharacter(Game& game) : game_(game)
	{
		
	}



void PlayerCharacter::Init()
{
	if (!Idle_texture_.loadFromFile("data/sprites/player.png"))
	{
		std::cerr << "Could not load hero idle texture!\n";
	}
	for (int i = 0; i < 2; i++)
	{
		std::ostringstream oss;
		oss << "data/sprites/walk/walk" << (i + 1) << ".png";
		walkTexture_[i].loadFromFile(oss.str());
	}
	jumpTexture_.loadFromFile("data/sprites/jump.png");
	UpdateSpriteTexture(Idle_texture_);
	sprite_.setTexture(Idle_texture_);
	sprite_.setScale(playerScale_,playerScale_);

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(1.0f, 1.0f);
	body_ = game_.CreateBody(bodyDef);

	const auto textureSize = Idle_texture_.getSize() - borderMagin_ * 2u;
	b2Vec2 boxsize;
	boxsize.Set(
		textureSize.x / game_.pixeltoMeter / 2.0f * playerScale_,
		textureSize.y / game_.pixeltoMeter / 2.0f * playerScale_);
	b2PolygonShape playerBox;
	playerBox.SetAsBox(boxsize.x, boxsize.y);
	
	b2FixtureDef playerFixureDef;
	playerFixureDef.shape = &playerBox;
	playerFixureDef.density = 1.0f;
	playerFixureDef.friction = 0.0f;
	body_->CreateFixture(&playerFixureDef);

	b2PolygonShape playerfootBox;
	b2Vec2 footCenter;
	footCenter.Set(0.0f, boxsize.y);
	playerfootBox.SetAsBox(boxsize.x, 0.2f, footCenter, 0.0f);

	b2FixtureDef playerFootFixtureDef;
	playerFootFixtureDef.isSensor = true;
	playerFootFixtureDef.shape = &playerfootBox;
	body_->CreateFixture(&playerFootFixtureDef);
}

void PlayerCharacter::Update(float dt)
{
	float dx = 0.0f;
	float dy = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		dx -= 1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		dx += 1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		dy -= 1.0f;
	}
	auto velocity = body_->GetLinearVelocity();
	velocity.Set(dx * speed_, dy);
	body_->SetLinearVelocity(velocity);

	switch (state_)
	{
	case State::IDLE:
	{

		if (footcontact_ == 0)
		{
			ChangeState(State::JUMP);
		}
		if (std::abs(dx) > deadzone_)
		{
			ChangeState(State::WALK);
		}
		break;
	}

	case State::WALK:
	{

		if (footcontact_ == 0)
		{
			ChangeState(State::JUMP);
		}

		if (facingRight_ && dx < 0.0f)
		{
			Flip();
		}

		if (!facingRight_ && dx > 0.0f)
		{
			Flip();
		}

		walkAnimTimer_ += dt;
		if (walkAnimTimer_ > walkAnimPeriod_)
		{
			walkAnimIndex_++;
			if (walkAnimIndex_ >= walkTexture_.size())

			{
				walkAnimIndex_ = 0;
			}

			UpdateSpriteTexture(walkTexture_[walkAnimIndex_]);
			walkAnimTimer_ -= walkAnimPeriod_;
		}

		if (std::abs(dx) < deadzone_)
		{
			ChangeState(State::IDLE);
		}
		break;
	}
	case State::JUMP:
	{
		if (footcontact_ > 0)
		{
			ChangeState(State::IDLE);
		}
		break;
	}
	default:;
	}
	const auto& b2Position = body_->GetPosition();
	sprite_.setPosition(sf::Vector2f(b2Position.x * game_.pixeltoMeter, b2Position.y * game_.pixeltoMeter));
	const auto b2rotation = body_->GetAngle();
	sprite_.setRotation(b2rotation / M_PI * 180.0f);
}

void PlayerCharacter::Render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

sf::Vector2<float> PlayerCharacter::GetPosition() const
{
	return sprite_.getPosition();
}

void PlayerCharacter::BeginContactGround()
{
	footcontact_++;
}

void PlayerCharacter::EndContactGround()
{
	footcontact_--;
}


void PlayerCharacter::ChangeState(State state)
{
	switch (state)
	{
	case State::IDLE:
	{
		UpdateSpriteTexture(Idle_texture_);
		break;
	}
	case State::WALK:
	{
		UpdateSpriteTexture(walkTexture_[0]);
		break;
	}
	case State::JUMP:
	{
		UpdateSpriteTexture(jumpTexture_);
		break;
	}
	default: break;
	}
	state_ = state;
}

	void PlayerCharacter::UpdateSpriteTexture(const sf::Texture& texture)
	{
		sprite_.setTexture(texture);
		const auto textureSize = texture.getSize();
		sprite_.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
	}

	void PlayerCharacter::Flip()
	{
		facingRight_ = !facingRight_;
		auto scale = sprite_.getScale();
		scale.x = -scale.x;
		sprite_.setScale(scale);
	}
