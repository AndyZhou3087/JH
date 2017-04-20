#include "GameDataSave.h"
#include "cocos2d.h"
#include "Utility.h"
#include "Hero.h"
#include "Nature.h"
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

int GameDataSave::getHeroOutinjury()
{
	return loadIntDataByKey("outinjury", Hero::MAXOutinjuryValue);
}
void GameDataSave::setHeroOutinjury(int val)
{
	saveIntDataByKey("outinjury", val);
}

int GameDataSave::getHeroInnerinjury()
{
	return loadIntDataByKey("innerinjury", Hero::MAXInnerinjuryValue);
}
void GameDataSave::setHeroInnerinjury(int val)
{
	saveIntDataByKey("innerinjury", val);
}

int GameDataSave::getHeroHunger()
{
	return loadIntDataByKey("hunger", Hero::MAXHungerValue);
}
void GameDataSave::setHeroHunger(int val)
{
	saveIntDataByKey("hunger", val);
}

int GameDataSave::getHeroSpirit()
{
	return loadIntDataByKey("spirit", Hero::MAXSpiritValue);
}
void GameDataSave::setHeroSpirit(int val)
{
	saveIntDataByKey("spirit", val);
}

int GameDataSave::getHeroLife()
{
	return loadIntDataByKey("life", -1);
}
void GameDataSave::setHeroLife(int val)
{
	saveIntDataByKey("life", val);
}

int GameDataSave::getNatureTime()
{
	return loadIntDataByKey("ndaytime");
}
void GameDataSave::setNatureTime(int val)
{
	saveIntDataByKey("ndaytime", val);
}

int GameDataSave::getNatureWeather()
{
	return loadIntDataByKey("nwhather", EWeather::Suny);
}
void GameDataSave::setNatureWeather(int val)
{
	saveIntDataByKey("nwhather", val);
}
int GameDataSave::getNatureReason()
{
	return loadIntDataByKey("nreason", EReason::Spring);
}
void GameDataSave::setNatureReason(int val)
{
	saveIntDataByKey("nreason", val);
}

int GameDataSave::getNatureTemperature()
{
	int r = tempeRange[getNatureReason()][0] + rand() % (tempeRange[getNatureReason()][1] - tempeRange[getNatureReason()][0] + 1);
	return loadIntDataByKey("ntempe", r);
}
void GameDataSave::setNatureTemperature(int val)
{
	saveIntDataByKey("ntempe", val);
}

std::string GameDataSave::getStorageData()
{
	return loadStringDataByKey("storage");
}
void GameDataSave::setStorageData(std::string valstr)
{
	saveStringDataByKey("storage", valstr);
}

int GameDataSave::getBuildLV(std::string buildname)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	return loadIntDataByKey(rname);
}
void GameDataSave::setBuildLV(std::string buildname, int val)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	saveIntDataByKey(rname, val);
}

std::string GameDataSave::getHeroAddr()
{
	return loadStringDataByKey("heroaddr", "m1-1");
}

void GameDataSave::setHeroAddr(std::string addr)
{
	saveStringDataByKey("heroaddr", addr);
}

std::string GameDataSave::getTempStorage(std::string addrname)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	return loadStringDataByKey(str);
}

void GameDataSave::setTempStorage(std::string addrname, std::string vstr)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	saveStringDataByKey(str, vstr);
}

std::string GameDataSave::getPackage()
{
	return loadStringDataByKey("packages");
}

void GameDataSave::setPackage(std::string vstr)
{
	saveStringDataByKey("packages", vstr);
}

std::string GameDataSave::getResData()
{
	return loadStringDataByKey("resdata");
}
void GameDataSave::setResData(std::string vstr)
{
	saveStringDataByKey("resdata", vstr);
}

int GameDataSave::getHeroId()
{
	return loadIntDataByKey("heroid", 1);
}
void GameDataSave::setHeroId(int id)
{
	saveIntDataByKey("heroid", id);
}

int GameDataSave::getHeroLV()
{
	return loadIntDataByKey("herolv", 0);
}
void GameDataSave::setHeroLV(int lv)
{
	saveIntDataByKey("herolv", lv);
}


int GameDataSave::getHeroExp()
{
	return loadIntDataByKey("heroexp", 0);
}
void GameDataSave::setHeroExp(int val)
{
	saveIntDataByKey("heroexp", val);
}

bool GameDataSave::getHeroIsOut()
{
	int isout = loadIntDataByKey("heroisout", 0);
	return isout == 0?false:true;
}
void GameDataSave::setHeroIsOut(bool val)
{
	saveIntDataByKey("heroisout", val==true?1:0);
}

std::string GameDataSave::getHeroProperData()
{
	return loadStringDataByKey("heroproper");
}
void GameDataSave::setHeroProperData(std::string strval)
{
	saveStringDataByKey("heroproper", strval);
}