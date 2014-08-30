/*
=================
cProtect.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cProtect.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cProtect::cProtect(): cSprite() {}			// Default constructor
cProtect::cProtect(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename){}  // Constructor