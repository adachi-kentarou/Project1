/**
* @file Item.h
*/
#ifndef LIFE_ITEM_INCLUDED
#define LIFE_ITEM_INCLUDED
#include "Charactor.h"

class LifeItem:public Char::Item {
public:
	
	std::function<void(LifeItem*)> updfnc = nullptr;
	void SetFunc(std::function<void(LifeItem*)> fnc) { updfnc = fnc; };
	static void UpdateFnc(LifeItem* n);
	
	void DeleteAnimation();
	void Update();

	virtual void GetItem(Char::Charactor* c);
};

#endif