#include "WineTable.h"
#include "Hero.h"

extern Hero* g_hero;
WineTable::WineTable()
{
	goodvaule = 100;
}


WineTable::~WineTable()
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

void WineTable::action(int minute, int exminute)
{
	g_hero->drinking();
}