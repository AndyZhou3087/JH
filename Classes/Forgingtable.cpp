#include "Forgingtable.h"


Forgingtable::Forgingtable()
{
	goodvaule = 100;
}


Forgingtable::~Forgingtable()
{

}

void Forgingtable::build()
{

}


bool Forgingtable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/forgingtable.png"))
	{

		return true;
	}
	return false;
}

void Forgingtable::action(float acsec, int exminute)
{

}