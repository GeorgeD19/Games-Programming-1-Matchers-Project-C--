/*
=================
cWeeman.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cWeeman.h"
/*
=================
Creates a weeman object
=================
*/
cWeeman::cWeeman(): cSprite() {}
cWeeman::cWeeman(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename) {}

/*
=================
- update the position etc of the sprite.
=================
*/
void cWeeman::update(float deltaTime) 					// update the detectors position based on the translation
{
	D3DXVECTOR2 tempV2;
	D3DXVECTOR3 tempV3;
	tempV2 = cWeeman::getSpritePos2D() + (cWeeman::getTranslation())*deltaTime;
	tempV3 = D3DXVECTOR3(tempV2.x,tempV2.y,0.0f);
	cWeeman::setSpritePos(tempV3);
	cSprite::update();
}