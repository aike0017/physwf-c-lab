#ifndef _DW_LOADING_SCENE_H
#define _DW_LOADING_SCENE_H

#include "cocos2d.h"

USING_NS_CC;

class DWLoading : public CCLayer
{
public:
	virtual bool init();
	static DWLoading* create();
private:
	DWLoading();
	~DWLoading();
private:

};
#endif