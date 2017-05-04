#include "GameDataSave.h"
#include "cocos2d.h"
#include "Utility.h"
#include "Hero.h"
#include "Nature.h"
#include "GlobalData.h"
GameDataSave* GameDataSave::_Context = NULL;
std::string GameDataSave::userid = "";
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
	return loadIntDataByKey(addUIDString("livedays"));
}

void GameDataSave::setLiveDays(int val)
{
	saveIntDataByKey(addUIDString("livedays"), val);
}

int GameDataSave::getHeroOutinjury()
{
	return loadIntDataByKey(addUIDString("outinjury"), Hero::MAXOutinjuryValue);
}
void GameDataSave::setHeroOutinjury(int val)
{
	saveIntDataByKey(addUIDString("outinjury"), val);
}

int GameDataSave::getHeroInnerinjury()
{
	return loadIntDataByKey(addUIDString("innerinjury"), Hero::MAXInnerinjuryValue);
}
void GameDataSave::setHeroInnerinjury(int val)
{
	saveIntDataByKey(addUIDString("innerinjury"), val);
}

int GameDataSave::getHeroHunger()
{
	return loadIntDataByKey(addUIDString("hunger"), Hero::MAXHungerValue);
}
void GameDataSave::setHeroHunger(int val)
{
	saveIntDataByKey(addUIDString("hunger"), val);
}

int GameDataSave::getHeroSpirit()
{
	return loadIntDataByKey(addUIDString("spirit"), Hero::MAXSpiritValue);
}
void GameDataSave::setHeroSpirit(int val)
{
	saveIntDataByKey(addUIDString("spirit"), val);
}

int GameDataSave::getHeroLife()
{
	return loadIntDataByKey(addUIDString("life"), -1);
}
void GameDataSave::setHeroLife(int val)
{
	saveIntDataByKey(addUIDString("life"), val);
}

int GameDataSave::getNatureTime()
{
	return loadIntDataByKey(addUIDString("ndaytime"));
}
void GameDataSave::setNatureTime(int val)
{
	saveIntDataByKey(addUIDString("ndaytime"), val);
}

int GameDataSave::getNatureWeather()
{
	return loadIntDataByKey(addUIDString("nwhather"), EWeather::Suny);
}
void GameDataSave::setNatureWeather(int val)
{
	saveIntDataByKey(addUIDString("nwhather"), val);
}
int GameDataSave::getNatureReason()
{
	return loadIntDataByKey(addUIDString("nreason"), EReason::Spring);
}
void GameDataSave::setNatureReason(int val)
{
	saveIntDataByKey(addUIDString("nreason"), val);
}

int GameDataSave::getNatureTemperature()
{
	int r = tempeRange[getNatureReason()][0] + rand() % (tempeRange[getNatureReason()][1] - tempeRange[getNatureReason()][0] + 1);
	return loadIntDataByKey(addUIDString("ntempe"), r);
}
void GameDataSave::setNatureTemperature(int val)
{
	saveIntDataByKey(addUIDString("ntempe"), val);
}

std::string GameDataSave::getStorageData()
{
	return loadStringDataByKey(addUIDString("storage"), GlobalData::getDefaultStorage(getHeroId()));
}
void GameDataSave::setStorageData(std::string valstr)
{
	saveStringDataByKey(addUIDString("storage"), valstr);
}

int GameDataSave::getBuildLV(std::string buildname)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	return loadIntDataByKey(addUIDString(rname));
}
void GameDataSave::setBuildLV(std::string buildname, int val)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	saveIntDataByKey(addUIDString(rname), val);
}

std::string GameDataSave::getHeroAddr()
{
	return loadStringDataByKey(addUIDString("heroaddr"), "m1-1");
}

void GameDataSave::setHeroAddr(std::string addr)
{
	saveStringDataByKey(addUIDString("heroaddr"), addr);
}

std::string GameDataSave::getTempStorage(std::string addrname)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	return loadStringDataByKey(addUIDString(str));
}

void GameDataSave::setTempStorage(std::string addrname, std::string vstr)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	saveStringDataByKey(addUIDString(str), vstr);
}

std::string GameDataSave::getPackage()
{
	return loadStringDataByKey(addUIDString("packages"));
}

void GameDataSave::setPackage(std::string vstr)
{
	saveStringDataByKey(addUIDString("packages"), vstr);
}

std::string GameDataSave::getResData()
{
	return loadStringDataByKey(addUIDString("resdata"));
}
void GameDataSave::setResData(std::string vstr)
{
	saveStringDataByKey(addUIDString("resdata"), vstr);
}

int GameDataSave::getHeroId()
{
	return loadIntDataByKey(addUIDString("heroid"), 1);
}
void GameDataSave::setHeroId(int id)
{
	saveIntDataByKey(addUIDString("heroid"), id);
}

int GameDataSave::getHeroLV()
{
	return loadIntDataByKey(addUIDString("herolv"), 0);
}
void GameDataSave::setHeroLV(int lv)
{
	saveIntDataByKey(addUIDString("herolv"), lv);
}


int GameDataSave::getHeroExp()
{
	return loadIntDataByKey(addUIDString("heroexp"), 0);
}
void GameDataSave::setHeroExp(int val)
{
	saveIntDataByKey(addUIDString("heroexp"), val);
}

bool GameDataSave::getHeroIsOut()
{
	int isout = loadIntDataByKey(addUIDString("heroisout"), 0);
	return isout == 0?false:true;
}
void GameDataSave::setHeroIsOut(bool val)
{
	saveIntDataByKey(addUIDString("heroisout"), val == true ? 1 : 0);
}

std::string GameDataSave::getHeroProperData()
{
	return loadStringDataByKey(addUIDString("heroproper"));
}
void GameDataSave::setHeroProperData(std::string strval)
{
	saveStringDataByKey(addUIDString("heroproper"), strval);
}

std::string GameDataSave::getHeroUnlockData()
{
	return loadStringDataByKey(addUIDString("herounlock"), "1-0-0-0");
}
void GameDataSave::setHeroUnlockData(std::string strval)
{
	saveStringDataByKey(addUIDString("herounlock"), strval);
}

std::string GameDataSave::getUserId()
{
	return loadStringDataByKey("uid");
}

void GameDataSave::setUserId(std::string val)
{
	userid = val;
	saveStringDataByKey("uid", val);
}

std::string GameDataSave::addUIDString(std::string val)
{
	return userid + val;
}

void GameDataSave::setPlotMissionIndex(int val)
{
	saveIntDataByKey(addUIDString("plot"), val);
}

int GameDataSave::getPlotMissionIndex()
{
	return loadIntDataByKey(addUIDString("plot"), 0);
}

void GameDataSave::setPlotMissionStatus(std::string strval)
{
	saveStringDataByKey(addUIDString("plotstatus"), strval);
}

std::string GameDataSave::getPlotMissionStatus()
{
	return loadStringDataByKey(addUIDString("plotstatus"));
}


