#include "BedUILayer.h"
#include "json.h"
#include "Bed.h"

BedUILayer::BedUILayer()
{

}


BedUILayer::~BedUILayer()
{
}

bool BedUILayer::init()
{
	Node* csbnode = CSLoader::createNode("bedUiLayer.csb");
	this->addChild(csbnode);

	return true;
}