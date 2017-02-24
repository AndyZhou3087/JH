#ifndef _GAMEDATA_SAVE_H_
#define _GAMEDATA_SAVE_H_

class GameDataSave
{
public:
	static GameDataSave* getInstance();
	static void purgeGameSave();
	int getLiveDays();
	void setLiveDays(int val);

	int getNatureTime();
	void setNatureTime(int val);

	int getNatureWheather();
	void setNatureWheather(int val);

	int getNatureReason();
	void setNatureReason(int val);

	int getNatureTemperature();
	void setNatureTemperature(int val);

	int getHeroOutinjury();
	void setHeroOutinjury(int val);

	int getHeroInnerinjury();
	void setHeroInnerinjury(int val);

	int getHeroHunger();
	void setHeroHunger(int val);

	int getHeroSpirit();
	void setHeroSpirit(int val);

	int getHeroLife();
	void setHeroLife(int val);

	int getHeroMaxLife();
	void setHeroMaxLife(int val);

private:
	static GameDataSave* _Context;

};
#endif

