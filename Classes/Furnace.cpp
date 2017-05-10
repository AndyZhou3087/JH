#include "Furnace.h"
#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;

Furnace::Furnace()
{
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
	//生火
	extime = exminute;
	g_nature->setTimeInterval(minute / (TIMESCALE* getActionBarTime()));

	this->scheduleOnce(schedule_selector(Furnace::warm), getActionBarTime());
}
void Furnace::warm(float dt)
{
	//温度+15°
	g_nature->setTemperature(g_nature->getTemperature() + 15);
	this->scheduleOnce(schedule_selector(Furnace::warm), extime / TIMESCALE);
	
}
void Furnace::warmover(float dt)
{
	//取暖时间到
	g_nature->setTemperature(g_nature->getTemperature() - 15);
}