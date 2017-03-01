#ifndef _NATURE_H_
#define _NATURE_H_
#include "cocos2d.h"
USING_NS_CC;
typedef enum
{
	Spring = 0,
	Summer,
	Autumn,
	Winter
}EReason;

typedef enum
{
	Suny = 0,
	Cloudy,
	Rainy,
	Snowy
}EWeather;
typedef enum
{
	Day = 0,
	Night
}EDAYNIGTH;

class Nature:public Node
{
public:
	Nature();
	~Nature();
	CC_SYNTHESIZE(int, m_temperature, Temperature);
	CC_SYNTHESIZE(int, m_time, Time);//����һ��ʱ�䣬0.2s--��Ӧ1min
	CC_SYNTHESIZE(EReason, m_reason, Reason);
	CC_SYNTHESIZE(EWeather, m_weather, Weather);
	CC_SYNTHESIZE(EDAYNIGTH, m_daynight, DayOrNight);
	CC_SYNTHESIZE(int, m_pastdays, PastDays);

	void ChangeWeather();
	void ChangeReason();
	void ChangeDayNight();

	virtual bool init();

	CREATE_FUNC(Nature);

public:
	int static ReasonCDays;//�����л�����
private:
	int static systime();
	void updateData(float dt);
};

static std::string reasonname[] = { "��", "��", "��", "��" };
static std::string reasondesc1[] = { "��������", "��������", "��������", "������������������������" };
static std::string reasondesc[] = { "����������������", "������������", "��������������������", "������������" };

static std::string weathername[] = {"����", "����", "����", "��ѩ"};
static std::string weatherdesc1[] = { "������������", "������������", "������������", "��ѩ��ѩ��ѩ" };
static std::string weatherdesc[] = { "����������������", "��������������������", "��������������������", "��ѩ��ѩ��ѩ��ѩ��ѩ" };

static std::string liveDayDesc = {"��¼����ʱ�䣬�Ա��ƶ�������ļƻ�"};
static std::string timeDesc = { "06:00-18:00���� 18:00-06:00��ҹ" };

#endif

