#include "StorageRoom.h"
#include "Const.h"

StorageRoom::StorageRoom()
{
}


StorageRoom::~StorageRoom()
{

}

bool StorageRoom::init()
{
	if (Sprite::initWithSpriteFrameName("ui/storageroom.png"))
	{

		return true;
	}
	return false;
}