#include "GameDataSave.h"
#include "cocos2d.h"
#include "Utility.h"
GameDataSave* GameDataSave::_Context = NULL;
GameDataSave* GameDataSave::getInstance() {
	if (_Context == NULL) {
		_Context = new GameDataSave;
	}
	return _Context;
}

void GameDataSave::purgeGameSave() {
	CC_SAFE_DELETE(_Context);
}

int GameDataSave::getLiveDays()
{
	return loadIntDataByKey("livedays");
}

void GameDataSave::setLiveDays(int val)
{
	saveIntDataByKey("livedays", val);
}

int GameDataSave::getNatureTime()
{
	return loadIntDataByKey("ndaytime");
}
void GameDataSave::setNatureTime(int val)
{
	saveIntDataByKey("ndaytime", val);
}

int GameDataSave::getNatureWheather()
{
	return loadIntDataByKey("nwhather");
}
void GameDataSave::setNatureWheather(int val)
{
	saveIntDataByKey("nwhather", val);
}
int GameDataSave::getNatureReason()
{
	return loadIntDataByKey("nreason");
}
void GameDataSave::setNatureReason(int val)
{
	saveIntDataByKey("nreason", val);
}

int GameDataSave::getNatureTemperature()
{
	return loadIntDataByKey("ntempe");
}
void GameDataSave::setNatureTemperature(int val)
{
	saveIntDataByKey("ntempe", val);
}