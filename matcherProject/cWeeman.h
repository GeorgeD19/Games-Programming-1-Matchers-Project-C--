/*
=================
cWeeman.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CWEEMAN_H
#define _CWEEMAN_H
#include "cSprite.h"

class cWeeman : public cSprite
{
private:
	//Can be changed depending on difficulty
	int mHealth;

public:
	cWeeman();
	cWeeman(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename);
	void update(float deltaTime);					// update the weeman's position based on the translation
};
#endif