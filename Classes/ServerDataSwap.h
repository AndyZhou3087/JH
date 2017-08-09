/********************************************************************
*���������ݽ���
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
	void vipIsOn();
	void setDelegate(ServerDataDelegateProtocol *delegateProtocol);
private:
	static ServerDataSwap* _serverDataSwap;
	static ServerDataDelegateProtocol *m_pDelegateProtocol;//�ӿڶ���
private:
	static void httpPostOneDataCB(std::string retdata, int code, std::string tag);
	static void httpGetAllDataCB(std::string retdata, int code, std::string tag);
	static void httpPropadJustDataCB(std::string retdata, int code, std::string tag);
	static void httpModifyNickNameCB(std::string retdata, int code, std::string tag);
	static void httpVipIsOnCB(std::string retdata, int code, std::string tag);
};

class ServerDataDelegateProtocol
{
public:
	virtual ~ServerDataDelegateProtocol(){};

	/**
	* @brief ���ݳɹ�
	*/
	virtual void onSuccess() {};

	virtual void onErr(int errcode) {};
};

#endif
