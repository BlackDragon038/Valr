#pragma once
#include "FighterPawn.h"

struct Node
{
	ATTACK_TYPE Attack;
	Node* Low = nullptr;
	Node* Medium = nullptr;
	Node* Heavy = nullptr;
};

class ComboTree
{
public: 
	Node* node;
	void AddNode(Node* n);
	ATTACK_TYPE Search(Node n);

};

