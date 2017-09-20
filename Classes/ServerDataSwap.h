/********************************************************************
*服务器数据交互
*********************************************************************/
#ifndef __SERVERDATASWAP__
#define __SERVERDATASWAP__
#include "cocos2d.h"

class ServerDataDelegateProtocol;

class ServerDataSwap: public cocos2d::Ref
{
public:

	static ServerDataSwap* init(ServerDataDelegateProtocol* delegateProtocol = NULL);

	void postOneData(std::string userid, int tag = 0);
	void postAllData();
	void getAllData();
	void propadjust();
	void modifyNickName(std::string nickname);
	void vipSuccNotice(std::string gid);
	void vipIsOn(int heroid);
	void isGetVip(std::vector<std::string> vipids);
	void updateFreeReviveCount();
	void getServerTime();
	void getRankData(std::string orderby);
	void getannouncement();
	void setDelegate(ServerDataDelegateProtocol *delegateProtocol);
	static bool isGetingData();

	void createFaciton(std::string name, int lvlimit, int sexlimit, std::string desc);
	void getFactionList();
	void requestFaction(int factionid);
	void getFactionMembers(int factionid);

	void joinFaction(int factionid, int requesterId, int requestertype);
	void kickFaction(int factionid, int requesterId, int requestertype);
	void promotionFaction(int factionid, int requesterId, int requestertype, int position);
	void leaveFaction(int actiontype, int factionid, int herotype);
	void contributionFaction(int factionid, int contribution, int herotype);

	void cancelFaction(int factionid);
	void refuseFaction(int factionid, int requesterId, int requestertype);
private:
	ServerDataDelegateProtocol *m_pDelegateProtocol;//接口对象
	static bool isdoing;
	ServerDataSwap();
	~ServerDataSwap();
	ServerDataSwap(ServerDataDelegateProtocol *pDelegateProtocol);
private:
	void httpPostOneDataCB(std::string retdata, int code, std::string tag);
	void httpGetAllDataCB(std::string retdata, int code, std::string tag);
	void httpPropadJustDataCB(std::string retdata, int code, std::string tag);
	void httpModifyNickNameCB(std::string retdata, int code, std::string tag);
	void httpVipIsOnCB(std::string retdata, int code, std::string tag);
	void httpGetServerTimeCB(std::string retdata, int code, std::string tag);
	void httpGetRankDataCB(std::string retdata, int code, std::string tag);
	void httpGetAnnouncementCB(std::string retdata, int code, std::string tag);
	void httpIsGetVipCB(std::string retdata, int code, std::string tag);
	void httpBlankCB(std::string retdata, int code, std::string tag);
	void httpCreateFactionCB(std::string retdata, int code, std::string tag);
	void httpGetFactionListCB(std::string retdata, int code, std::string tag);
	void httpRequestFactionListCB(std::string retdata, int code, std::string tag);
	void httpGetFactionMemberCB(std::string retdata, int code, std::string tag);
	void httpJionFactionCB(std::string retdata, int code, std::string tag);
	void httpKickFactionCB(std::string retdata, int code, std::string tag);

	void httpPromotionFactionCB(std::string retdata, int code, std::string tag);
	void httpLeaveFactionCB(std::string retdata, int code, std::string tag);

	void httpContributionFactionCB(std::string retdata, int code, std::string tag);

	void httpCancelFactionCB(std::string retdata, int code, std::string tag);

	void httpRefuseFactionCB(std::string retdata, int code, std::string tag);
};

class ServerDataDelegateProtocol
{
public:
	virtual ~ServerDataDelegateProtocol(){};

	/**
	* @brief 数据成功
	*/
	virtual void onSuccess() {};

	virtual void onErr(int errcode) {};
};

#endif
