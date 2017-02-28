#include "WineMaker.h"


WineMaker::WineMaker()
{
	goodvaule = 100;
}


WineMaker::~WineMaker()
{

}

void WineMaker::build()
{

}


bool WineMaker::init()
{
	if (Sprite::initWithSpriteFrameName("ui/winemake.png"))
	{

		return true;
	}
	return false;
}

void WineMaker::repair()
{

}