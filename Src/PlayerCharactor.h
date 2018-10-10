/**
* @file PlayerCharactor.h
*/
#ifndef PLAYER_CHARACTOR_INCLUDED
#define PLAYER_CHARACTOR_INCLUDED
#include "Charactor.h"

class PlayerCharactor :public Char::Charactor {
public:
	~PlayerCharactor();

	std::function<void(PlayerCharactor*)> updfnc = nullptr;
	void SetFunc(std::function<void(PlayerCharactor*)> fnc) { updfnc = fnc; };
	static void UpdateFnc(PlayerCharactor* n);

	void Update();

	void Attack();

	void EnemyAction();

	void DamageAnimation();

};

#endif