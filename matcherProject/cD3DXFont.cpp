/*
=================
cD3DXFont.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cD3DXFont.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cD3DXFont::cD3DXFont()
{
	mFont = NULL;
}
cD3DXFont::cD3DXFont(LPDIRECT3DDEVICE9 theDevice, HINSTANCE hInstance, LPCSTR theFontname, WORD fontID) // Constructor
{
	if(FAILED(createCustomFont( hInstance, theFontname, fontID)))
	{
		OutputDebugString("Error: Cannot create Custom Font!");
	}
	else 
		if(FAILED(createDXFont( theDevice, theFontname )))
		{
			OutputDebugString("Error: Cannot create DX Font!");
		}

}
cD3DXFont::cD3DXFont(LPDIRECT3DDEVICE9 theDevice, HINSTANCE hInstance, LPCSTR theFontname, WORD fontID, INT fontSize) // Constructor
{
	if(FAILED(createCustomFont( hInstance, theFontname, fontID)))
	{
		OutputDebugString("Error: Cannot create Custom Font!");
	}
	else 
		if(FAILED(createDXFont( theDevice, theFontname, fontSize )))
		{
			OutputDebugString("Error: Cannot create DX Font!");
		}

}
/*
=================
- Destructor.
=================
*/
cD3DXFont::~cD3DXFont()					// Destructor
{
	if(mFont != NULL)
		mFont->Release();
	RemoveFontMemResourceEx(mHFontHdle);
}
/*
=================
- Use windows to find the custom font.
=================
*/
HRSRC cD3DXFont::locateFontResource(HINSTANCE hInstance, LPCSTR fontName, WORD fontID)	// Use windows to find the custom font
{
	return mCustFont = FindResource(hInstance, MAKEINTRESOURCE(fontID), RT_FONT);
}
/*
=================
- Use windows to create a custom font in memory.
=================
*/
HRESULT cD3DXFont::createCustomFont(HINSTANCE hInstance, LPCSTR fontName, WORD fontID)	// Use windows to create a custom font in memory;
{
	 if (locateFontResource(hInstance, fontName, fontID)) 
     {
          HGLOBAL mem = LoadResource(hInstance, mCustFont);
          void *data = LockResource(mem);
          size_t len = SizeofResource(hInstance, mCustFont);

          DWORD nFonts;
          mHFontHdle = AddFontMemResourceEx(data, len, NULL, &nFonts);

          if(mHFontHdle == 0)
		  {
			  OutputDebugString("Error: Font add fails");
			  return S_FALSE;
		  }
     }
	 return S_OK;
}
/*
=================
- Create the DX Font based on the custom font and font size specified
=================
*/
HRESULT cD3DXFont::createDXFont(LPDIRECT3DDEVICE9 theDevice, LPCSTR fontName, INT fontSize)	// Create the DX Font based on the custom font
{
	mFontDesc.Height = fontSize;
	mFontDesc.Width = 0;
	mFontDesc.Weight = FW_NORMAL;
	mFontDesc.MipLevels = 0;
	mFontDesc.Italic = false;
	mFontDesc.CharSet = DEFAULT_CHARSET;
	//mFontDesc.OutputPrecision = OUT_TT_PRECIS;
	mFontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	mFontDesc.Quality = CLIP_DEFAULT_PRECIS;
	mFontDesc.PitchAndFamily = DEFAULT_PITCH;
	//mFontDesc.FaceName = "JI Balloon Caps";
	strcpy_s(mFontDesc.FaceName, 32, fontName);

	if (FAILED(D3DXCreateFontIndirect(theDevice, &mFontDesc, &mFont)))
	{
		 OutputDebugString("Error: D3DXFont failed");
		 return 0;
	}
	return S_OK;
}
/*
=================
- Create the DX Font based on the custom font.
=================
*/
HRESULT cD3DXFont::createDXFont(LPDIRECT3DDEVICE9 theDevice, LPCSTR fontName)	// Create the DX Font based on the custom font
{
	mFontDesc.Height = 48;
	mFontDesc.Width = 0;
	mFontDesc.Weight = FW_NORMAL;
	mFontDesc.MipLevels = 0;
	mFontDesc.Italic = false;
	mFontDesc.CharSet = DEFAULT_CHARSET;
	//mFontDesc.OutputPrecision = OUT_TT_PRECIS;
	mFontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	mFontDesc.Quality = CLIP_DEFAULT_PRECIS;
	mFontDesc.PitchAndFamily = DEFAULT_PITCH;
	//mFontDesc.FaceName = "JI Balloon Caps";
	strcpy_s(mFontDesc.FaceName, 32, fontName);

	if (FAILED(D3DXCreateFontIndirect(theDevice, &mFontDesc, &mFont)))
	{
		 OutputDebugString("Error: D3DXFont failed");
		 return 0;
	}
	return S_OK;
}
/*
=================
- Draw the text onto the screen.
=================
*/
void cD3DXFont::printText(LPCSTR string, RECT pos, DWORD color)				// Draw the text onto the screen.
{
	pos.bottom += 40;
	mFont->DrawText(NULL, string,  -1, &pos, DT_CENTER, color);
}

