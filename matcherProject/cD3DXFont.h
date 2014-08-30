/*
=================
cD3DXFont.h
- Header file for class definition - SPECIFICATION
- Header file for the Font class
=================
*/
#ifndef _CD3DXFONT_H
#define _CD3DXFONT_H
#include "GameConstants.h"
#include "GameResources.h"

class cD3DXFont
{
private:
	LPD3DXFONT mFont;
	D3DXFONT_DESC mFontDesc;
	HRSRC mCustFont;
	HANDLE mHFontHdle;

public:
	cD3DXFont();
	cD3DXFont(LPDIRECT3DDEVICE9 theDevice, HINSTANCE hInstance, LPCSTR theFontname, WORD fontID); // Constructor
	~cD3DXFont();					// Destructor
	cD3DXFont::cD3DXFont(LPDIRECT3DDEVICE9 theDevice, HINSTANCE hInstance, LPCSTR theFontname, WORD fontID, INT fontSize); // Constructor with fontsize
	HRSRC locateFontResource(HINSTANCE hInstance, LPCSTR fontName, WORD fontID);	// Use windows to find the custom font
	HRESULT createCustomFont(HINSTANCE hInstance, LPCSTR fontName, WORD fontID);	// Use windows to create a custom font in memory;
	HRESULT createDXFont(LPDIRECT3DDEVICE9 theDevice, LPCSTR fontName, INT fontSize);	// Create the DX Font based on the custom font overloaded to include font size
	HRESULT createDXFont(LPDIRECT3DDEVICE9 theDevice, LPCSTR fontName);	// Create the DX Font based on the custom font
	void printText(LPCSTR string, RECT pos, DWORD color);				// Draw the text onto the screen.
};
#endif