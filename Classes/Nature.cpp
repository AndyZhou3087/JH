#include "Nature.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"

int Nature::ReasonCDays = 30;

Nature::Nature()
{
	m_timeinterval = NORMAL_TIMEINTERVAL;
	m_daynight = Night;
	m_ismakewarm = false;
	m_isshowinsect = false;
}

Nature::~Nature()
{

}

bool Nature::init()
{
	this->schedule(schedule_selector(Nature::updateData), NORMAL_TIMEINTERVAL * 1.0f / TIMESCALE);
	return true;
}


void Nature::ChangeWeather()
{
	int r = GlobalData::createRandomNum(100);
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		if (r < WeatherWeight[m_reason][i])
		{
			break;
		}
	}
	EWeather w = (EWeather)i;
	if (w != m_weather)
	{
		int c = w - m_weather;
		setWeather(w);
		setTemperature(m_temperature - c * 5);
		g_uiScroll->addEventText(CommonFuncs::gbk2utf(weatherEventText[w].c_str()));
	}


}

void Nature::ChangeReason()
{
	int yudays = m_pastdays % (4 * ReasonCDays);
	EReason r = Spring;
	if (yudays < ReasonCDays)
	{
		r = Spring;
	}
	else if (yudays < ReasonCDays * 2)
	{
		r = Summer;
	}
	else if (yudays < ReasonCDays * 3)
	{
		r = Autumn;
	}
	else
	{
		r = Winter;
	}
	if (r != m_reason)
	{
		setReason(r);
		int maxr = tempeRange[m_reason][1] - tempeRange[m_reason][0] + 1;
		int  t = tempeRange[m_reason][0] + GlobalData::createRandomNum(maxr);
		if (m_ismakewarm)
			t += 15;
		setTemperature(t);

		g_uiScroll->addEventText(CommonFuncs::gbk2utf(reasonEventText[r].c_str()));
	}
}


void Nature::makewarm(int extime)
{
	setIsMaKeWarm(true);
	setTemperature(g_nature->getTemperature() + 15);
	this->scheduleOnce(schedule_selector(Nature::makewarmover), extime / TIMESCALE);
	std::string str = StringUtils::format("%d-%d", m_pastdays * 24 * 60 + (int)m_time, extime);
	GlobalData::setMakeWarmConfig(str);
}

void Nature::makewarmover(float dt)
{
	//取暖时间到
	setIsMaKeWarm(false);
	setTemperature(g_nature->getTemperature() - 15);
	GlobalData::setMakeWarmConfig("");
}


void Nature::ChangeDayNight()
{
	if (m_time < 60 * 6 - 1 || m_time >= 18 * 60)
	{
		if (m_daynight == Day)
		{
			setDayOrNight(Night);
			setTemperature(m_temperature - 5);
			g_uiScroll->addEventText(CommonFuncs::gbk2utf(dayEventText[1].c_str()));
		}

	}
	else
	{
		if (m_daynight == Night)
		{
			setDayOrNight(Day);
			setTemperature(m_temperature + 5);
			g_uiScroll->addEventText(CommonFuncs::gbk2utf(dayEventText[0].c_str()));
		}
	}
}

void Nature::updateData(float dt)
{
	if (GlobalData::g_gameStatus != GAMESTART)
		return;

	m_time += getTimeInterval();
	if (m_time >= 1440.0f)
	{
		m_pastdays++;
		m_time = 0.0f;
		changeWeatherCount = 0;
		changeWeatherRandow = GlobalData::createRandomNum(24) + 1;
		ChangeReason();
	}
	//产生随机数
	int inttime = (int)m_time;
	if (changeWeatherRandow <= 0)
		changeWeatherRandow = GlobalData::createRandomNum(23) + 1;
	if (changeWeatherRandow == inttime / 60)
	{
		changeWeatherCount++;
		if (changeWeatherCount <= 1)//每天变化一次
		{
			ChangeWeather();
		}
	}

	ChangeDayNight();
}