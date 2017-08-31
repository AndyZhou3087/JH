/********************************************************************
*服务器数据交互
*********************************************************************/
#ifndef __SERVERDATASWAP__
#define __SERVERDATASWAP__
#include "cocos2d.h"

class ServerDataDelegateProtocol;

class ServerDataSwap
{
public:
	ServerDataSwap();
	~ServerDataSwap();
	static ServerDataSwap* getInstance();

	void postOneData(std::string userid, int tag = 0);
	void postAllData();
	void getAllData();
	void propadjust();
	void modifyNickName(std::string nickname);
	void vipSuccNotice(std::string gid);
	void vipIsOn(int heroid);
	void updateFreeReviveCount();
	void getServerTime();
	void getRankData(std::string orderby);
	void getannouncement();
	void setDelegate(ServerDataDelegateProtocol *delegateProtocol);
private:
	static ServerDataSwap* _serverDataSwap;
	static ServerDataDelegateProtocol *m_pDelegateProtocol;//接口对象
private:
	static void httpPostOneDataCB(std::string retdata, int code, std::string tag);
	static void httpGetAllDataCB(std::string retdata, int code, std::string tag);
	static void httpPropadJustDataCB(std::string retdata, int code, std::string tag);
	static void httpModifyNickNameCB(std::string retdata, int code, std::string tag);
	static void httpVipIsOnCB(std::string retdata, int code, std::string tag);
	static void httpGetServerTimeCB(std::string retdata, int code, std::string tag);
	static void httpGetRankDataCB(std::string retdata, int code, std::string tag);
	static void httpGetAnnouncementCB(std::string retdata, int code, std::string tag);
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
