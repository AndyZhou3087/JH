#include "Forgingtable.h"

#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;

Forgingtable::Forgingtable()
{
	goodvaule = 100;
}


Forgingtable::~Forgingtable()
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

void Forgingtable::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute / (TIMESCALE* getActionBarTime()));
}