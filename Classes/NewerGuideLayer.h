

#ifndef __NEWERGUIDELAYER__
#define __NEWERGUIDELAYER__

#include "cocos2d.h"
USING_NS_CC;

#define NEWERLAYERZOER 1999
class NewerGuideLayer : public Layer
{
public:
	NewerGuideLayer();
	~NewerGuideLayer();
	bool init(int type, std::vector<Node*> stencilNodes);
	static NewerGuideLayer* create(int step, std::vector<Node*> stencilNodes);
	static bool checkifNewerGuide(int index);
private:
	Layer* m_colorlayer;
	ClippingNode* m_clippingNode;
	int m_step;
	static bool isNewerGuide[65];
private:
	void showAnim(Vec2 pos);
	Vec2 starPos;

};
extern NewerGuideLayer* g_NewerGuideLayer;
#endif
