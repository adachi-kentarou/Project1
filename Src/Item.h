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

	void GetItem(Char::Charactor* c);
};

class TimeItem :public Char::Item {
public:
	std::function<void(TimeItem*)> updfnc = nullptr;
	void SetFunc(std::function<void(TimeItem*)> fnc) { updfnc = fnc; };
	static void UpdateFnc(TimeItem* n);

	void DeleteAnimation();
	void Update();

	void GetItem(Char::Charactor* c);
};
#endif