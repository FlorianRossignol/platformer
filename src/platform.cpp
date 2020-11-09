#include "platform.h"
#include "game.h"
#include <box2d/box2d.h>

Platform::Platform(Game& game) : game_(game)
{
    isTextureLoaded_ = false;
}

void Platform::Init()
{
    if (!isTextureLoaded_)
    {
        platform_texture_.loadFromFile("data/sprites/platform.png");
        isTextureLoaded_ = true;
    }
    sprite_.setTexture(platform_texture_);
    b2BodyDef bodyDef;
    bodyDef.position.Set(originPos.x, originPos.y);

    sprite_.setPosition(
        originPos.x * game_.pixeltoMeter,
        originPos.y * game_.pixeltoMeter);
    
    body_ = game_.CreateBody(bodyDef);
	
    const auto textureSize = platform_texture_.getSize();
    sprite_.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    sprite_.setScale(scale_, scale_);

    b2Vec2 boxSize;
    boxSize.Set(
        textureSize.x / game_.pixeltoMeter / 2.0f * scale_,
        textureSize.y / game_.pixeltoMeter / 2.0f * scale_
    );


    b2PolygonShape platformShape;
    platformShape.SetAsBox(boxSize.x, boxSize.y);
    b2FixtureDef platformFixtureDef;
    platformFixtureDef.userData = &platformUserData_;
    platformFixtureDef.shape = &platformShape;
    body_->CreateFixture(&platformFixtureDef);

	b2BodyDef platform2;
    platform2.position.Set(123, 13);
    platform2.fixedRotation = false;
    platform2.type = b2_staticBody;
    b2PolygonShape platform2Shape;
    platform2Shape.SetAsBox(boxSize.x,boxSize.y);

}

void Platform::Update(float dt)
{
}

void Platform::Render(sf::RenderWindow& window)
{
    window.draw(sprite_);
}