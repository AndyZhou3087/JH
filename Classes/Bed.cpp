#include "Bed.h"
#include "Nature.h"
#include "Hero.h"
#include "Const.h"

extern Nature* g_nature;
extern Hero* g_hero;

Bed::Bed()
{
}


Bed::~Bed()
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
	//更新时间刷新的间隔，这样实现消耗游戏
	g_nature->setTimeInterval(exminute / (TIMESCALE* getActionBarTime()));
	//
	g_hero->sleep(exminute / 60);
}