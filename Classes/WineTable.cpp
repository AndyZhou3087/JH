#include "WineTable.h"


WineTable::WineTable()
{
	goodvaule = 100;
}


WineTable::~WineTable()
{

}

void WineTable::build()
{

}


bool WineTable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/winetable.png"))
	{

		return true;
	}
	return false;
}

void WineTable::repair()
{

}