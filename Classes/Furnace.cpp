#include "Furnace.h"
#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;
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

void Furnace::action(int minute, int exminute)
{
	g_nature->setTemperature(g_nature->getTemperature() + 15);
	this->scheduleOnce(schedule_selector(Furnace::warmover), ACTION_BAR_TIME);
}

void Furnace::warmover(float dt)
{
	g_nature->setTemperature(g_nature->getTemperature() - 15);
}