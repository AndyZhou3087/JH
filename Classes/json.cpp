
#include "json.h"
std::string JsonWriter(rapidjson::Document& doc)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	return buffer.GetString();
}

//
bool JsonReader(std::string& strData, rapidjson::Document& doc)
{
	bool result = true;

	doc.Parse<0>(strData.c_str());
	if (doc.HasParseError())  //¥Ú”°Ω‚Œˆ¥ÌŒÛ
	{
		//log("JsonReader %s\n", doc.GetParseError());
		result = false;
	}
	return result;
}


rapidjson::Document ReadJsonFile(const std::string& name)
{
	std::string file = name;
	rapidjson::Document doc;

	FILE* fp = fopen(file.c_str(), "rt");

	if(fp != NULL) 
	{
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char tmp[1024*10] = {0};

		fread(tmp, 1, len, fp);
		fclose(fp);

		std::string data = tmp;
		JsonReader(data, doc);
	}

	return doc;
}

void WriteJsonFile(const std::string& name, rapidjson::Document& doc)
{
	std::string file = name;

	FILE* fp = fopen(file.c_str(), "wt");
	if(fp != NULL)
	{	
		std::string data = JsonWriter(doc);
		fwrite(data.c_str(), 1, data.size(), fp);
		fclose(fp);
	}
}