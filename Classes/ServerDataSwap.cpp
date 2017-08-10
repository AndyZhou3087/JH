#include "ServerDataSwap.h"
#include "GlobalData.h"
#include "HttpUtil.h"
#include "json.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

#define HTTPURL "https://www.stormnet.cn/api/"

ServerDataSwap* ServerDataSwap::_serverDataSwap = NULL;

ServerDataDelegateProtocol* ServerDataSwap::m_pDelegateProtocol = NULL;

ServerDataSwap::ServerDataSwap()
{

}

ServerDataSwap::~ServerDataSwap()
{

}

ServerDataSwap* ServerDataSwap::getInstance()
{
	if (_serverDataSwap == NULL)
	{
		_serverDataSwap = new  ServerDataSwap();
	}

	return _serverDataSwap;
}

void ServerDataSwap::setDelegate(ServerDataDelegateProtocol *delegateProtocol)
{
	m_pDelegateProtocol = delegateProtocol;
}


void ServerDataSwap::postAllData()
{
	std::vector<std::string> vec_ids = GlobalData::getSaveListId();

	std::vector<std::string> vec_userid;
	for (unsigned int i = 0; i < vec_ids.size(); i++)
	{
		std::string saveuid = vec_ids[i];
		if (saveuid.length() > 0)
		{
			vec_userid.push_back(saveuid);
		}
	}

	int tag = 0;
	for (unsigned int i = 0; i < vec_userid.size(); i++)
	{
		if (i == vec_userid.size() - 1)
			tag = 1;
		postOneData(vec_userid[i], tag);
	}
}

void ServerDataSwap::postOneData(std::string userid, int tag)
{
	std::string postdata;

	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();

	writedoc.AddMember("playerid", rapidjson::Value(GlobalData::UUID().c_str(), allocator), allocator);
	writedoc.AddMember("localid", rapidjson::Value(userid.c_str(), allocator), allocator);
	writedoc.AddMember("nickname", rapidjson::Value(GlobalData::getMyNickName().c_str(), allocator), allocator);

	int herounlock = 1;
	std::string str = GameDataSave::getInstance()->getHeroUnlockData();
	std::vector<std::string> tmp;
	CommonFuncs::split(str, tmp, "-");

	for (unsigned int i = 1; i < tmp.size(); i++)
	{
		int val = atoi(tmp[i].c_str());
		herounlock += val << i;
	}

	writedoc.AddMember("hunlock", herounlock, allocator);
	int coin = GameDataSave::getInstance()->getGoldCount();
	int usecoin = GameDataSave::getInstance()->getUseGold();
	writedoc.AddMember("coin", coin, allocator);
	writedoc.AddMember("costcoin", usecoin, allocator);
	GameDataSave::getInstance()->setUserId(userid);

	int type = GameDataSave::getInstance()->getHeroId();
	int lv = GameDataSave::getInstance()->getHeroLV();
	int exp = GameDataSave::getInstance()->getHeroExp();
	for (int i = 0; i <= lv; i++)
	{
		exp += GlobalData::map_heroAtr[type].vec_exp[i];
	}
	writedoc.AddMember("type", type, allocator);
	writedoc.AddMember("exp", exp, allocator);
	int hungry = GameDataSave::getInstance()->getHeroHunger();
	int innerhurt = GameDataSave::getInstance()->getHeroInnerinjury();
	int outerhurt = GameDataSave::getInstance()->getHeroOutinjury();
	int life = GameDataSave::getInstance()->getHeroLife();
	int days = GameDataSave::getInstance()->getLiveDays();
	int sprite = GameDataSave::getInstance()->getHeroSpirit();
	int plotindex = GameDataSave::getInstance()->getPlotMissionIndex();
	int bpotindex = GameDataSave::getInstance()->getBranchPlotMissionIndex();
	int unlock = GameDataSave::getInstance()->getPlotUnlockChapter();

	writedoc.AddMember("hungry", hungry, allocator);
	writedoc.AddMember("innerhurt", innerhurt, allocator);
	writedoc.AddMember("outerhurt", outerhurt, allocator);
	writedoc.AddMember("life", life, allocator);
	writedoc.AddMember("days", days, allocator);
	writedoc.AddMember("mood", sprite, allocator);
	writedoc.AddMember("task", plotindex, allocator);
	writedoc.AddMember("btask", bpotindex, allocator);
	writedoc.AddMember("unlock", unlock, allocator);
	
	rapidjson::Document doc = ReadJsonFile("data/buildings.json");
	rapidjson::Value& allBuilds = doc["b"];
	for (unsigned int i = 0; i < allBuilds.Size(); i++)
	{
		rapidjson::Value& oneBuild = allBuilds[i];
		rapidjson::Value& oneitem = oneBuild["name"];
		std::string buildname = oneitem.GetString();
		int blv = GameDataSave::getInstance()->getBuildLV(buildname);
        std::string blvstr = StringUtils::format("%d", blv);
		writedoc.AddMember(rapidjson::Value(buildname.c_str(), allocator), rapidjson::Value(blvstr.c_str(), allocator), allocator);
	}
	
	rapidjson::Value dataArray(rapidjson::kArrayType);

	std::string strval[] = { GameDataSave::getInstance()->getStorageData(), GameDataSave::getInstance()->getPackage(), GameDataSave::getInstance()->getHeroProperData() };

	for (int m = 0; m < sizeof(strval) / sizeof(strval[0]); m++)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("flag", m + 1, allocator);

		std::vector<std::string> tmp;
		CommonFuncs::split(strval[m], tmp, ";");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			std::vector<std::string> tmp2;
			CommonFuncs::split(tmp[i], tmp2, "-");

			std::string strid = tmp2[0];
			if (strid.length() <= 0)
				continue;

			int type = atoi(tmp2[1].c_str());
			int count = atoi(tmp2[2].c_str());
			int lv = atoi(tmp2[4].c_str());
			int goodvalue = atoi(tmp2[6].c_str());

			int slv = 0;
			if (tmp2.size() >= 9)
			{
				slv = atoi(tmp2[7].c_str());
			}

			if (strid.length() > 0 && count > 0)
			{
				int val = 0;
				std::string jsonkey;

				if (atoi(strid.c_str()) != 0)
				{
					val = count;
					jsonkey = StringUtils::format("r%s", strid.c_str());
				}
				else
				{
					jsonkey = strid;
					if (type == WEAPON || type == PROTECT_EQU)
					{
						val = slv * 1000 + goodvalue;
					}
					else
					{
						val = lv;
					}
				}
                std::string valstr = StringUtils::format("%d", val);
				object.AddMember(rapidjson::Value(jsonkey.c_str(), allocator), rapidjson::Value(valstr.c_str(), allocator), allocator);
			}
		}
		dataArray.PushBack(object, allocator);
	}

	writedoc.AddMember("data", dataArray, allocator);
	postdata = JsonWriter(writedoc);

	std::string url;
	url.append(HTTPURL);
	url.append("wx_savealldata");
	std::string tagstr = StringUtils::format("%d", tag);
	HttpUtil::getInstance()->doData(url, httpPostOneDataCB, "", POST, postdata, tagstr);
}

void ServerDataSwap::getAllData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_getalldata?");
	url.append("playerid=");
	url.append(GlobalData::UUID());
	HttpUtil::getInstance()->doData(url, httpGetAllDataCB);
}

void ServerDataSwap::propadjust()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_propadjust?");
	url.append("playerid=");
	url.append(GlobalData::UUID());
	HttpUtil::getInstance()->doData(url, httpPropadJustDataCB);
	
}

void ServerDataSwap::modifyNickName(std::string nickname)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_updateusername?");
	url.append("playerid=");
	url.append(GlobalData::UUID());
	url.append("&name=");
	url.append(nickname);
	HttpUtil::getInstance()->doData(url, httpModifyNickNameCB);
}

void ServerDataSwap::vipSuccNotice(std::string gid)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_buynotify?");
	url.append("playerid=");
	url.append(GlobalData::UUID());
	url.append("&goodsid=");
	url.append(gid);
	HttpUtil::getInstance()->doData(url);
}

void ServerDataSwap::vipIsOn()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_takemonthlycard?");
	url.append("playerid=");
	url.append(GlobalData::UUID());
	HttpUtil::getInstance()->doData(url, httpVipIsOnCB);
}

void ServerDataSwap::updateFreeReviveCount(int count)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_usefreelife?");
	url.append("playerid=");
	url.append(GlobalData::UUID());
	HttpUtil::getInstance()->doData(url);
}

void ServerDataSwap::httpPostOneDataCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		if (tag.compare("1") == 0)
		{
			if (m_pDelegateProtocol != NULL)
			{
				m_pDelegateProtocol->onSuccess();
			}
		}
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			rapidjson::Value& myidv = doc["id"];
			std::string myidstr = myidv.GetString();
			GameDataSave::getInstance()->setMyID(myidstr);

			rapidjson::Value& mynamev = doc["nickname"];
			std::string mynamestr = mynamev.GetString();
			GameDataSave::getInstance()->setMyNickName(mynamestr);
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
}

void ServerDataSwap::httpGetAllDataCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			std::vector<std::string> vec_saveid;
			for (int i = 0; i < 4; i++)
			{
				vec_saveid.push_back("");
			}
			rapidjson::Value& myidv = doc["id"];
			std::string myidstr = myidv.GetString();
			GameDataSave::getInstance()->setMyID(myidstr);

			rapidjson::Value& mynamev = doc["nickname"];
			std::string mynamestr = mynamev.GetString();
			GameDataSave::getInstance()->setMyNickName(mynamestr);

			if (doc.HasMember("coin"))
			{
				rapidjson::Value& coindata = doc["coin"];
				GameDataSave::getInstance()->setGoldCount(atoi(coindata.GetString()));
			}
			if (doc.HasMember("costcoin"))
			{
				rapidjson::Value& coindata = doc["costcoin"];
				GameDataSave::getInstance()->setUseGold(atoi(coindata.GetString()));
			}
			rapidjson::Value& hunlockdata = doc["hunlock"];
			int hunlock = atoi(hunlockdata.GetString());

			for (int k = 0; k < 4; k++)
			{
				int val = hunlock & (1 << k);
				val = val >> k;
				GlobalData::setUnlockHero(k, val == 1 ? true : false);
			}

			rapidjson::Value& dataArray = doc["data"];

			if (doc.HasMember("data"))
            {
			for (unsigned int m = 0; m < dataArray.Size(); m++)
			{
				rapidjson::Value& item = dataArray[m];

				rapidjson::Value& v = item["localid"];
				std::string localuid = v.GetString();

				GameDataSave::getInstance()->setUserId(localuid);

				v = item["type"];
				int type = atoi(v.GetString());
				GameDataSave::getInstance()->setHeroId(type);

				vec_saveid[type-1] = localuid;

				v = item["exp"];
				int exp = atoi(v.GetString());
				int lv = 0;
				int size = GlobalData::map_heroAtr[type].vec_exp.size();
				for (int i = 0; i < size; i++)
				{
					if (exp > GlobalData::map_heroAtr[type].vec_exp[i])
					{
						lv = i;
						exp = exp - GlobalData::map_heroAtr[type].vec_exp[i];
					}
					else
					{
						break;
					}
				}
				if (lv >= size)
				{
					lv = size - 1;
					exp = GlobalData::map_heroAtr[type].vec_exp[lv];
				}

				GameDataSave::getInstance()->setHeroLV(lv); 
				GameDataSave::getInstance()->setHeroExp(exp);

				v = item["hungry"];
				int hungry = atoi(v.GetString());
				GameDataSave::getInstance()->setHeroHunger(hungry);

				v = item["innerhurt"];
				int innerhurt = atoi(v.GetString());
				GameDataSave::getInstance()->setHeroInnerinjury(innerhurt);

				v = item["outerhurt"];
				int outerhurt = atoi(v.GetString());
				GameDataSave::getInstance()->setHeroOutinjury(outerhurt);

				v = item["life"];
				int life = atoi(v.GetString());
				GameDataSave::getInstance()->setHeroLife(life);

				v = item["days"];
				int days = atoi(v.GetString());
				GameDataSave::getInstance()->setLiveDays(days);

				v = item["mood"];
				int spirit = atoi(v.GetString());
				GameDataSave::getInstance()->setHeroSpirit(spirit);

				v = item["task"];
				int task = atoi(v.GetString());
				GameDataSave::getInstance()->setPlotMissionIndex(task);

				std::string str;
				int pdatasize = GlobalData::vec_PlotMissionData.size();
				for (int i = 0; i < pdatasize; i++)
				{
					std::string tmpstr;
					if (i < task)
						tmpstr = "2-";
					else
						tmpstr = "0-";
					str.append(tmpstr);
				}
				GameDataSave::getInstance()->setPlotMissionStatus(str.substr(0, str.length() - 1));

				v = item["btask"];
				int btask = atoi(v.GetString());
				GameDataSave::getInstance()->setBranchPlotMissionIndex(btask);

				std::string bstr;
				int bpdatasize = GlobalData::vec_BranchPlotMissionData.size();
				for (int i = 0; i < bpdatasize; i++)
				{
					std::string tmpstr;
					if (i < btask)
						tmpstr = "2-";
					else
						tmpstr = "0-";
					bstr.append(tmpstr);
				}
				GameDataSave::getInstance()->setBranchPlotMissionStatus(bstr.substr(0, bstr.length() - 1));

				v = item["unlock"];
				int unlock = atoi(v.GetString());
				GameDataSave::getInstance()->setPlotUnlockChapter(unlock);

				rapidjson::Document doc = ReadJsonFile("data/buildings.json");
				rapidjson::Value& allBuilds = doc["b"];
				for (unsigned int i = 0; i < allBuilds.Size(); i++)
				{
					rapidjson::Value& oneBuild = allBuilds[i];
					rapidjson::Value& oneitem = oneBuild["name"];
					std::string buildname = oneitem.GetString();
					v = item[buildname.c_str()];
					int blv = atoi(v.GetString());
					GameDataSave::getInstance()->setBuildLV(buildname, blv);
				}

				v = item["holding"];
				
				for (unsigned int n = 0; n < v.Size(); n++)
				{
					rapidjson::Value& hv = v[n];
					rapidjson::Value& resv = hv["flag"];
					int flag = atoi(resv.GetString());
					std::string str;
					for (rapidjson::Value::ConstMemberIterator iter = hv.MemberBegin(); iter != hv.MemberEnd(); ++iter)
					{
						std::string strid = iter->name.GetString();

						if (strid.compare("flag") != 0)
						{
							int val = atoi(iter->value.GetString());
							int goodvalue = 100;
							int lv = 0;
							int tqu = 1;
							int slv = 0;
							int count = 0;
							if (strid.compare(0, 1, "r") == 0)
							{
								strid = strid.substr(1);
								count = val;
							}
							else
							{
								if (strid.compare(0, 1, "a") == 0 || strid.compare(0, 1, "e") == 0)
								{
									goodvalue = val % 1000;
									slv = goodvalue / 1000;
								}
								else
								{
									lv = val;
								}
								count = 1;
							}
							std::string tempstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", strid.c_str(), GlobalData::getResType(strid), count, GlobalData::getResExType(strid), lv, 0, goodvalue, slv, tqu);
							str.append(tempstr);
						}
					}
					if (str.length() > 1)
					{
						str = str.substr(0, str.length() - 1);
						if (flag == 1)
							GameDataSave::getInstance()->setStorageData(str);
						else if (flag == 2)
						{
							GameDataSave::getInstance()->setPackage(str);
						}
						else if (flag == 3)
						{
							const std::string prestr[] = { "a", "24", "25", "26", "w", "x", "e","7"};
							std::vector<std::string> tmp;
							CommonFuncs::split(str, tmp, ";");
							str.clear();
							for (int c = 0; c < 8; c++)
							{
								int len = prestr[c].length();
								bool isfind = false;
								for (unsigned int k = 0; k < tmp.size(); k++)
								{
									if (prestr[c].compare(0, len, tmp[k], 0, len) == 0)
									{
										isfind = true;
										str.append(tmp[k]);
										str.append(";");
										break;
									}
								}
								if (!isfind)
								{
									str.append("-0-0-0-0-0-100-0-1;");
								}
							}
							GameDataSave::getInstance()->setHeroProperData(str.substr(0, str.length() - 1));
						}
					}
				}

			}
            }

			GlobalData::setSaveListId(vec_saveid);
			if (vec_saveid.size() > 0)
				GlobalData::setUId(vec_saveid[0]);

			if (m_pDelegateProtocol != NULL)
			{
				m_pDelegateProtocol->onSuccess();
			}
		}
	}

	if (m_pDelegateProtocol != NULL)
	{
		m_pDelegateProtocol->onErr(-1);
	}
}

void ServerDataSwap::httpPropadJustDataCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			GameDataSave::getInstance()->setIsJustData(false);

			if (!doc.HasMember("data"))
				return;

			rapidjson::Value& dataArray = doc["data"];

			for (unsigned int m = 0; m < dataArray.Size(); m++)
			{
				rapidjson::Value& item = dataArray[m];

				rapidjson::Value& v = item["propid"];
				std::string strid = v.GetString();

				v = item["amount"];
				int val = atoi(v.GetString());

				if (strid.compare("coin") == 0)
				{
					int curcount = GameDataSave::getInstance()->getGoldCount();
					GameDataSave::getInstance()->setGoldCount(curcount + val);
					GlobalData::init();
					continue;
				}

				if (strid.compare("hunlock") == 0)
				{
					int hunlock = val;

					for (int k = 0; k < 4; k++)
					{
						int val = hunlock & (1 << k);
						val = val >> k;
						GlobalData::setUnlockHero(k, val == 1 ? true : false);
					}
					continue;
				}

				v = item["type"];
				int heroid = atoi(v.GetString());
				std::vector<std::string> vec_ids = GlobalData::getSaveListId();
				std::string uid = vec_ids[heroid - 1];
				if (uid.length() > 0)
				{
					GameDataSave::getInstance()->setUserId(uid);

					std::string strval = GameDataSave::getInstance()->getStorageData();
					std::vector<std::string> tmp;
					CommonFuncs::split(strval, tmp, ";");
					std::string retstr;
					bool isfind = false;
					for (unsigned int i = 0; i < tmp.size(); i++)
					{
						std::vector<std::string> tmp2;
						CommonFuncs::split(tmp[i], tmp2, "-");

						std::string id = tmp2[0];
						int type = atoi(tmp2[1].c_str());

						int count = atoi(tmp2[2].c_str());
						int extype = GlobalData::getResExType(id);
						int lv = atoi(tmp2[4].c_str());

						int exp = atoi(tmp2[5].c_str());
						int goodvalue = atoi(tmp2[6].c_str());
						int slv = 0;
						int tqu = 1;
						if (tmp2.size() >= 9)
						{
							slv = atoi(tmp2[7].c_str());
							tqu = atoi(tmp2[8].c_str());
						}
						if (strid.compare(id) == 0)
						{
							isfind = true;
							if (atoi(strid.c_str()) > 0)
								count += val;
							else
							{
								if (strid.compare(0, 1, "a") == 0 || strid.compare(0, 1, "e") == 0)
								{
									goodvalue = val % 1000;
									slv = goodvalue / 1000;
								}
								else
								{
									lv = val;
								}
								count = 1;
							}
						}
						std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", id.c_str(), type, count, extype, lv, exp, goodvalue, slv, tqu);
						retstr.append(idstr);
	
					}
					if (!isfind)
					{
						int count = 0;
						int goodvalue = 100;
						int lv = 0;
						int slv = 0;
						if (atoi(strid.c_str()) > 0)
							count = val;
						else
						{
							if (strid.compare(0, 1, "a") == 0 || strid.compare(0, 1, "e") == 0)
							{
								goodvalue = val % 1000;
								slv = goodvalue / 1000;
							}
							else
							{
								lv = val;
							}
							count = 1;
						}
						std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", strid.c_str(), GlobalData::getResType(strid), count, GlobalData::getResExType(strid), lv, 0, goodvalue, slv, 1);
						retstr.append(idstr);
					}

					if (retstr.length() > 0)
						GameDataSave::getInstance()->setStorageData(retstr.substr(0, retstr.length() - 1));
				}
			}

		}
	}
}




void ServerDataSwap::httpModifyNickNameCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			rapidjson::Value& reval = doc["ret"];
			int ret = reval.GetInt();
			if (ret == 0)
			{
				if (m_pDelegateProtocol != NULL)
					m_pDelegateProtocol->onSuccess();
			}
			else
			{
				if (m_pDelegateProtocol != NULL)
					m_pDelegateProtocol->onErr(ret);
			}
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
}

void ServerDataSwap::httpVipIsOnCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			for (rapidjson::Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
			{
				std::string strid = iter->name.GetString();

				if (strid.compare(0, 3, "vip") == 0)
				{
					int val = iter->value.GetInt();
					if (val > 0)
					{
						GlobalData::vec_buyVipIds.push_back(strid);
					}
				}
			}

			if (doc.HasMember("timegift"))
			{
				rapidjson::Value& retval = doc["timegift"];
				GlobalData::setTimeGiftLeftTime(retval.GetInt());
			}

			if (doc.HasMember("freelife"))
			{
				rapidjson::Value& retval = doc["freelife"];
				GlobalData::setFreeReviveCount(retval.GetInt());
			}

			if (m_pDelegateProtocol != NULL)
				m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
}

