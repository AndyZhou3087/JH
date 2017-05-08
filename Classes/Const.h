
#ifndef _H_CONST_H_
#define _H_CONST_H_
#include "cocos2d.h"
#define TIMESCALE 5.0f//时间比例尺
#define ACTION_BAR_TIME 4.0f//建筑物中进度条秒数
#define HERO_MOVE_SPEED 1.5f//地图中移动速度，px/s

static std::string acname[] = { "战斗", "采集", "砍伐", "挖掘" };
static std::string acdesc[] = {"","你徒手采集，废了好大力才获得了", "你徒手劈树，手掌都快劈兰了才获得了", "你徒手采矿，双手都磨破了才扣下来一块"};
static std::string acdesc1[] = {"", "你使用镰刀收割，毫不费力的获得", "你使用斧子劈树，毫不费力的获得", "你挥舞十字镐采矿，毫不费力的获得一块" };

static std::string herofightdesc[] = { "%s猛地跑上前去，赤手空拳对%s使出一掌" };
static std::string herofightdesc1[] = { "只见%s挥舞着%s犹如黑虎下山，以迅猛之势击中%s，连连后退数步方稳住身形。", "%s高举手中的%s,三两步便靠近%s，一招将其打翻在地。", "%s手持%s将%s逼到角落，大喝一声“受死吧！”", "%s猛的挥出%s，气势凛冽，声如破竹，直奔%s面门而来。", "%s手中的%s轻轻一挑，拨开%s的武器，并反手一打，正中对方胸口。" };
static std::string herofightdesc2[] = {"%s哇哇吐出一堆鲜血。受到了%d点伤害。", "%s被震的向后退了几步，差点倒在地上，受到了%d点伤害。", "%s似乎受到了重击，已经摇摇欲坠，受到了%d点伤害。", "%s单膝跪地，粗气连连，似乎耗费了很多力气，受到了%d点伤害。"};

static std::string bossfight[] = {"%s雷鸣闪电样迅速击中%s，硬生生的接住这一掌，受到%d点伤害。"};
static std::string bossfight1[] = { "%s雷鸣闪电样迅速击中%s，幸亏有%s挡了一下，虽然没有大碍，但还是受到%d点伤害。" };
#endif // _H_CONST_H_
