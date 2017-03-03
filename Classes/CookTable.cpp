#include "CookTable.h"


CookTable::CookTable()
{
	goodvaule = 100;
}


CookTable::~CookTable()
{

}

void CookTable::build()
{

}


bool CookTable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/cooktable.png"))
	{

		return true;
	}
	return false;
}

void CookTable::action(int minute, int exminute)
{

}