#pragma once
#include "FighterPawn.h"

struct Node
{
	ATTACK_TYPE Attack;
	Node* Low;
	Node* Medium;
	Node* Heavy;
};

class ComboTree
{

};

