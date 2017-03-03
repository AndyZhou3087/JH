#include "Bed.h"
#include "Nature.h"
#include "Hero.h"

extern Nature* g_nature;
extern Hero* g_hero;

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

void Bed::action(int minute, int exminute)
{
	g_nature->setTimeInterval(exminute / 5);
	g_hero->sleep(exminute / 60);
}