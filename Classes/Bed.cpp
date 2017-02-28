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
	if (Sprite::initWithSpriteFrameName("ui/bed.png"))
	{

		return true;
	}
	return false;
}

void Bed::repair()
{

}