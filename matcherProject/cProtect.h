/*
=================
cProtect.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CPROTECT_H
#define _CPROTECT_H
#include "cSprite.h"

class cProtect : public cSprite
{
public:
	cProtect();
	cProtect(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename); // Constructor
};
#endif