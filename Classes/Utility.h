
#ifndef _H_UTILITY_H_
#define _H_UTILITY_H_

#include "cocos2d.h"
#include "DataBase64.h"
#include <string>

using namespace std;

USING_NS_CC;

#define encrypt_data 0

inline void saveStringDataByKey(string key, string sValue = ""){ 
	char buffer[32];
	sprintf(buffer, "%s", key.c_str());
#if encrypt_data
	string sKey = saveData(reinterpret_cast<const unsigned char*>(sValue.c_str()), sValue.length());
#else
	string sKey = sValue;
#endif
	UserDefault::getInstance()->setStringForKey(buffer, sKey);

	UserDefault::getInstance()->flush();
}

inline void saveLongDataByKey(string Key, long nVal = 0L){
	char buffer[32];
	sprintf(buffer, "%ld", nVal);
	saveStringDataByKey(Key, buffer);
}

inline void saveIntDataByKey(string key, int nVal){
	char buffer[32];
	sprintf(buffer, "%d", nVal);
	saveStringDataByKey(key, buffer);
}

inline int loadIntDataByKey(string key, int defaultValue = 0){
	char buffer[32];
	sprintf(buffer, "%s", key.c_str());

	string s = UserDefault::getInstance()->getStringForKey(buffer);
	if(s == "") {
		return defaultValue;
	}
#if encrypt_data
	string parseKey = parseData(s);
	return atoi(parseKey.c_str());	
#else
	return atoi(s.c_str());
#endif
}

inline std::string loadStringDataByKey(string key, std::string defaultValue = ""){
	char buffer[32];
	sprintf(buffer, "%s", key.c_str());

	string s = UserDefault::getInstance()->getStringForKey(buffer);
	if (s == "") {
		return defaultValue;
	}
#if encrypt_data
	string parseKey = parseData(s);
	return parseKey;
#else
	return s;
#endif
}
#endif // _H_UTILITY_H_
