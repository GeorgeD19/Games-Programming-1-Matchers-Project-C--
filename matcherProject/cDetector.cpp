/*
=================
cBalloon.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cDetector.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cDetector::cDetector(): cSprite() 			// Default constructor
{
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );
	mDirection = static_cast<eDirection>(rand() % 2);
}
cDetector::cDetector(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename)  // Constructor
{
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );
	mDirection = static_cast<eDirection>(rand() % 2);
}
/*
=================
- update the position etc of the sprite.
=================
*/
void cDetector::update(float deltaTime) 					// update the detectors position based on the translation
{
	D3DXVECTOR2 tempV2;
	D3DXVECTOR3 tempV3;
	tempV2 = cDetector::getSpritePos2D() + (cDetector::getTranslation())*deltaTime;
	tempV3 = D3DXVECTOR3(tempV2.x,tempV2.y,0.0f);
	cDetector::setSpritePos(tempV3);
	cSprite::update();
}
/*
=================
- Returns the current direction of the sprite.
=================
*/
eDirection cDetector::getDirection()       // Rturns the current direction of the sprite
{
	return cDetector::mDirection;
}
/*
=================
- Sets the current direction for the sprite.
=================
*/
void cDetector::setDirection(eDirection theDirection)  // Sets the current direction for the sprite.
{
	cDetector::mDirection = theDirection;
}
