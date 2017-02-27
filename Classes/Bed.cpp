#include "Bed.h"


Bed::Bed()
{
	goodvaule = 100;
}


Bed::~Bed()
{

}

void Bed::build()
{

}


bool Bed::init()
{
	Sprite* self = Sprite::createWithSpriteFrameName("ui/bed.png");
	this->addChild(self);

	return true;
}

void Bed::repair()
{

}