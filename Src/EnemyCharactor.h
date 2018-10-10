/**
* @file EnemyCharactor.h
*/
#ifndef ENEMY_CHARACTOR_INCLUDED
#define ENEMY_CHARACTOR_INCLUDED
#include "Charactor.h"

class EnemyCharactor:public Char::Charactor {
public:
	
	std::function<void(EnemyCharactor*)> updfnc = nullptr;
	void SetFunc(std::function<void(EnemyCharactor*)> fnc) { updfnc = fnc; };
	static void UpdateFnc(EnemyCharactor* n);
	void DamageAnimation();
	void DeleteAnimation();
	void Update();
	void Action();

	void Attack();
	
};

#endif