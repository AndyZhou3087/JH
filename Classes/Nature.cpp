#include "Nature.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"

int Nature::ReasonCDays = 90;

int WeatherWeight[][4] = { { 40, 62, 100, 100 }, { 40, 62, 100, 100 }, { 40, 62, 100, 100 }, { 20, 40, 60, 100 } };
int tempeRange[][2] = { { 10, 20 }, { 15, 25 }, { 5, 15 }, {-20, 0} };
Nature::Nature()
{
	m_timeinterval = NORMAL_TIMEINTERVAL;
	m_pastdays = 0;
	m_time = 0;
	m_reason = EReason::Spring;
	m_weather = EWeather::Suny;
	m_daynight = EDAYNIGTH::Night;
	srand(systime());
	int r = tempeRange[m_reason][0] + rand() % (tempeRange[m_reason][1] - tempeRange[m_reason][0] + 1);
	m_temperature = r;
}

Nature::~Nature()
{

}

bool Nature::init()
{
	this->schedule(schedule_selector(Nature::updateData), 0.2f);
	return true;
}

int Nature::systime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	return timep;
}
void Nature::ChangeWeather()
{
	srand(systime());
	int r = rand() % 100;
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
		setTemperature(m_temperature + c * 5);
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
		srand(systime());
		int  t = tempeRange[m_reason][0] + rand() % (tempeRange[m_reason][1] - tempeRange[m_reason][0] + 1);
		setTemperature(t);
		g_uiScroll->addEventText(CommonFuncs::gbk2utf(reasonEventText[r].c_str()));
	}

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
			g_uiScroll->addEventText(CommonFuncs::gbk2utf(reasonEventText[0].c_str()));
		}
	}
}

void Nature::updateData(float dt)
{
	m_time += NORMAL_TIMEINTERVAL;
	if (m_time >= 1440.0f)
	{
		m_time = 0.0f;
		m_pastdays++;

		ChangeReason();
		ChangeWeather();
	}
	ChangeDayNight();
}