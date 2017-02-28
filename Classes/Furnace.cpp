#include "Furnace.h"


Furnace::Furnace()
{
	goodvaule = 100;
}


Furnace::~Furnace()
{

}

void Furnace::build()
{

}


bool Furnace::init()
{
	if (Sprite::initWithSpriteFrameName("ui/furnace.png"))
	{

		return true;
	}
	return false;
}

void Furnace::repair()
{

}