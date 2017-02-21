#include "HomeLayer.h"

HomeLayer::HomeLayer()
{
}


HomeLayer::~HomeLayer()
{
}

bool HomeLayer::init()
{
	Node* csbnode = CSLoader::createNode("homeLayer.csb");
	this->addChild(csbnode);

	return true;
}
