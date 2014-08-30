/*
=================
cDetector.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CDETECTOR_H
#define _CDETECTOR_H
#include "cSprite.h"

class cDetector : public cSprite
{
private:
	eDirection mDirection;

public:
	cDetector();
	cDetector(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename); // Constructor
	void update(float deltaTime);					// update the detectors position based on the translation
	eDirection getDirection();      // Returns the current direction of the sprite
	void setDirection(eDirection theDirection); // Sets the current direction for the sprite.
};
#endif