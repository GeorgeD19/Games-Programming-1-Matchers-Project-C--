/*
=================
cEnemy.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CENEMY_H
#define _CENEMY_H
#include "cSprite.h"

class cEnemy : public cSprite
{
private:
	eDirection mDirection;
	string sKey;	//Used to link up to which key is to be pressed
	
public:
	cEnemy();
	cEnemy(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename); // Constructor
	void update(float deltaTime);					// update the enemy position based on the translation
	eDirection getDirection();      // Returns the current direction of the sprite
	string getKey();
	void setKey(string cKey);
	void setDirection(eDirection theDirection); // Sets the current direction for the sprite.
	//void spawn(float delay, vector<cEnemy> enemy);
};
#endif