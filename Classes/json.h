#ifndef JSON_JSON_H_INCLUDED
#define JSON_JSON_H_INCLUDED

#include "cocos2d.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

std::string JsonWriter(rapidjson::Document& doc);

bool JsonReader(std::string& strData, rapidjson::Document& doc);
 
rapidjson::Document ReadJsonFile(const std::string& name);

void WriteJsonFile(const std::string& name, rapidjson::Document& doc);

#endif // JSON_JSON_H_INCLUDED
