#ifndef _HERO_H_
#define _HERO_H_
#include "cocos2d.h"
USING_NS_CC;

class Hero:public Node
{
public:
	Hero();
	~Hero();
	virtual bool init();

	CREATE_FUNC(Hero);

	CC_SYNTHESIZE(int, m_pic, HeadImg);
	CC_SYNTHESIZE(int, m_innerinjury, InnerinjuryValue);
	CC_SYNTHESIZE(int, m_outinjury, OutinjuryValue);
	CC_SYNTHESIZE(int, m_hunger, HungerValue);
	CC_SYNTHESIZE(float, m_spirit, SpiritValue);
	CC_SYNTHESIZE(int, m_life, LifeValue);
	CC_SYNTHESIZE(int, m_maxlife, MaxLifeValue);
	void sleep(int hour);
	void drinking();
public:
	static int MAXInnerinjuryValue;
	static int MAXOutinjuryValue;
	static int MAXHungerValue;
	static int MAXSpiritValue;
private:
	void updateData(float dt);
	void sleepbystep(float dt);
	void drinkbystep(float dt);
private:
	int sleephour;
};
static std::string innerInjurydesc1[] = { "��������", "�����ͨ", "������", "��Ϣ����", "��������", "�������", "��������" };
static std::string innerInjurydesc = {"��������ʱ�ή������ֵ���ޣ�Ҫʱ��ע������״̬��ʹ��ҩ����������ˡ�"};

static std::string outInjurydesc1[] = { "�����", "��ǿ��׳", "��Σ����", "�������", "Ƥ������", "��������", "��������" };
static std::string outInjurydesc = { "��������ʱ�ή������ֵ���ޣ�Ҫʱ��ע������״̬��ʹ��ҩ����������ˡ�" };

static std::string hungerdesc1[] = { "�ĸ�����", "���㷹��", "�����ˮ", "������ʳ", "������", "�̼��ܶ�", "��������" };
static std::string hungerdesc = { "���ȼ���ʱ������������Ե����ģ������͹��������������Ч�����ʵ���ʳ�ɱ�������״̬��" };

static std::string spiritInjurydesc1[] = { "�����緢", "�����", "����ʮ��", "��ͷɥ��", "û�����", "ή�Ҳ���", "���޿���" };
static std::string spiritInjurydesc = { "����״̬����ʱ�ή����������Ч�����ָ�Ч�ʣ��������õľ���״̬����Ҫ���Ⱦƻ��ܶ��˻�ָ�����״̬��" };

static std::string lifedesc = { "����ֵ������ʱ��ɫ������ͨ��˯�߿ɻָ�������" };
#endif

