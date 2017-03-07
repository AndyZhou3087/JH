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
	extime = exminute;
	g_nature->setTimeInterval(minute / (TIMESCALE* getActionBarTime()));

	this->scheduleOnce(schedule_selector(Furnace::warm), getActionBarTime());
}
void Furnace::warm(float dt)
{
	g_nature->setTemperature(g_nature->getTemperature() + 15);
	this->scheduleOnce(schedule_selector(Furnace::warm), extime / TIMESCALE);
	
}
void Furnace::warmover(float dt)
{
	g_nature->setTemperature(g_nature->getTemperature() - 15);
}