/*
=================
main.cpp
Main entry point for the Card application
=================
*/

#include "GameConstants.h"
#include "GameResources.h"
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cDetector.h"
#include "cWeeman.h"
#include "cEnemy.h"
#include "cSprite.h"
#include "cExplosion.h"
#include "cXAudio.h"
#include "cD3DXFont.h"
#include "CXBOXController.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <string>

using namespace std;

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();

// Get a reference to the detector class
vector<cDetector*> aDetector;
vector<cDetector*>::iterator d_iter;
vector<cDetector*>::iterator d_index;

// Get a refernce to the weeman class
vector<cWeeman*> aWeeman;
vector<cWeeman*>::iterator w_iter;
vector<cWeeman*>::iterator w_index;

// Get a refernce to the enemy class
vector<cEnemy*> aEnemy;
vector<cEnemy*>::iterator e_iter;
vector<cEnemy*>::iterator e_index;

//For the background sound track
bool bIsPlaying = false;

/*
==================================================================
* iGuihandle deals with changing between the menus/game
* 0 - Main Menu
* 1 - Main Game
* 2 - Win Screen
* 3 - Lose Screen
* 4 - How to play
* 5 - Options
* 6 - Exit's Game
==================================================================
*/
int iGuihandle = 0;
/*
==================================================================
* iDifficulty deals with the difficulty level of the game 
* 0 - Easy
* 1 - Medium
* 2 - Hard
==================================================================
*/
int iDifficulty = 0;
/*
==================================================================
* 0 - Keyboard
* 1 - Xbox Controller
==================================================================
*/
int iController = 0;
bool bPressed = false;
bool bVibrating = false;
float fCurrentVibrateTime = 5.0f;
float fVibrateTime = 0.3f;
float fCurrentPressTime = 5.0f;
float fPressInterval = 0.1f;
/*
==================================================================
* Spawn Variables
* _delay is altered based on difficulty to increase the rate at which objects appear
* _spawnTimer keeps track of the current time so that the objects appear appropriately (is reset on game replay)
==================================================================
*/
float _previousTime = 0.0f;
float _delay = 3.0f;
float _spawnTimer = 0.0f;
int _enemySpawnedIndex = 0;
float _detectorSpeed = 35.0f;
float _enemySpeed = 50.0f;

RECT clientBounds;
TCHAR szTempOutput[30];

/*
==================================================================
* In Game Interface Variables
==================================================================
*/
//Main Menu
	string sMainMenu_TitleStr = "Matchers";
	string sMainMenu_PlayStr = "Play";
	string sMainMenu_HowToPlayStr = "How to play";
	string sMainMenu_OptionsStr = "Options";
	string sMainMenu_ExitStr = "Exit";
	string sShared_BackStr = "Back";
	string _hitDetectorInfo;
	//How to play
	string sHowToPlayMenu_EnemyDescriptionStr = "The enemy are coming in from the edges of the screen,\n to defeat them make sure they are inside the detector\n before attempting to hit them.";
	string sHowToPlayMenu_DetectorDescriptionStr = "The Detectors will float between the weeman and\n the edges of the screen waiting for an enemy to fly under,\n when an enemy is under the detector\n press the key indicated on the enemy's body.";
	string sHowToPlayMenu_WeemanDescriptionStr = "The weeman needs protected against the enemy\n and if he loses all his health, its game over. \n Fend of the amount of enemy indicated at the top right.";
	//Options
	string sOptionsMenu_DefaultStr = "Default";
	string sOptionsMenu_CurrentDifficultyStr = "Difficulty: ";
	string sOptionsMenu_DifficultyStr[] = {"Easy","Medium","Hard"};
	string sOptionsMenu_ControllerSelectionStr = "Controller: ";
	string sOptionsMenu_KeyboardStr = "Keyboard";
	string sOptionsMenu_ControllerStr = "Controller";
	//Lose/Win
	string sWinLoseMenu_LoseStr = "You Lose";
	string sWinLoseMenu_WinStr = "You Win!";
	string sWinLoseMenu_TryAgainStr = "Try Again?";

bool gHit = false;
int gDetectorsHit = 0;
int gDetectorMissed = 0;
char gDetectorsHitStr[50];
char gDetectorMissedStr[50];
//Can be altered for the different difficulties
int iMaxDetectorHits = 15;
//Max amount of times the weeman can be hit
int iMaxWeemanHits = 5;
int hitDex;

//Main Menu Text
char gTitleStr[15];
char gPlayStr[10];
char gOptionsStr[10];
char gHowToPlayStr[20];
char gExitStr[10];

//Shared
char gBackStr[10];

//How to play
char gHowToPlayTitleStr[20];
char gEnemyDescription[300];
char gDetectorDescription[300];
char gWeemanDescription[300];

//Lose screen
char gloseTitleStr[20];
char gtryAgainStr[100];

//Win screen
char gwonTitleStr[20];
char gwinPlayAgainStr[100];

//Options Screen
char gEasyStr[10];
char gMediumStr[10];
char gHardStr[10];
// Notify the player of what difficulty setting is currently active
char gDifficultyStr[50];
char gDifficultySelectedStr[50];
// Notify the player of what controller they should be using
char gCurrentControllerStr[100];
char gCurrentControllerSelectedStr[50];
// Controller types
char gControllerKeyboard[50];
char gControllerXboxController[50];

//Position arrays
RECT mainMenuPos[5];
int mainMenuYPos[5];
RECT howToPlayPos[5];
RECT optionsPos[9];
RECT backPos;
RECT retryPos;
RECT titlePos;

D3DXVECTOR3 expPos;
list<cExplosion*> gFound;

// XBOX controller
CXBOXController* Player1;
//Used to compare
string pressedKey = "";

cXAudio gButtonTrack;
cXAudio gInGameSoundTrack;

bool bHit;

cD3DXTexture* enemyTextures[4];
cD3DXTexture* enemyxboxTextures[4];
//wasd
char* enemyTxtres[] = {"Images\\keycapture-w.png","Images\\keycapture-a.png","Images\\keycapture-s.png","Images\\keycapture-d.png"};
//yxab
char* enemyxboxTxtres[] = {"Images\\xbox-keycapture-y.png","Images\\xbox-keycapture-x.png","Images\\xbox-keycapture-a.png","Images\\xbox-keycapture-b.png"};
string keys[] = {"w","a","s","d"};
string xboxkeys[] = {"y","x","a","b"};

cD3DXTexture* detectorTexture;
char* detectorTxtres = "Images\\detector.png";
cD3DXTexture* weemanTexture;
char* weemanTxtres = "Images\\weeman.png";
cD3DXTexture* detectionTexture;
char* detectionTxtres = "Images\\detection.png";

bool insideRect(RECT thisSprite, POINT aPoint) 						// Return true if a point is within the rectangle
{
	if ((aPoint.x >= thisSprite.left) &&
		(aPoint.x <= thisSprite.right) &&
		(aPoint.y >= thisSprite.top ) &&
		(aPoint.y <= thisSprite.bottom))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void pressCheck(string skeyPress) {
					//if main game
					if(iGuihandle == 1) {
					//Used to count how many enemy have been detected that match the key press
					hitDex = 0;
					//For each detector we want to check if they can detect the enemy
					d_iter = aDetector.begin();
					while(d_iter != aDetector.end()) {	
					// For each enemy who's key matches the key pressed
						e_iter = aEnemy.begin();
						while (e_iter != aEnemy.end()) {
						 if((*e_iter)->getKey() == skeyPress) {
							//If the enemy's key matches the key pressed, check if the enemy is inside the detector's boundaries
							if((*e_iter)->collidedWith((*e_iter)->getBoundingRect(),(*d_iter)->getBoundingRect())) {
								expPos = (*e_iter)->getSpritePos();
								gHit = true;
										gDetectorsHit++;
										if(gDetectorsHit >= iMaxDetectorHits) { 
											
					bIsPlaying=false;
					iGuihandle = 2; 
										gButtonTrack.playSound(L"Sounds\\you-win.wav",false);
										}
										ostringstream convert;
										convert << iMaxDetectorHits;
										string _MaxDetectorHits = convert.str();
										_hitDetectorInfo = "Hit: %d / " + _MaxDetectorHits;
										sprintf_s(gDetectorsHitStr, 50, _hitDetectorInfo.c_str(), gDetectorsHit);
										hitDex++;	
										e_iter = aEnemy.erase(e_iter);
										_enemySpawnedIndex--;
										skeyPress = "";
							}
							//Check the next enemy
							else 
							{
								++e_iter;

							}
						 }
						 //Check the next enemy
							else 
							{
								++e_iter;

							}
							if(gHit) {
								gFound.push_back(new cExplosion(expPos,detectionTexture));
								gHit = false;
							}
						}
						//check the next detector
						++d_iter;
					}
					
					//If none matched or if none were hit
					if(hitDex == 0) {
							gDetectorMissed++;
							if(gDetectorMissed >= iMaxWeemanHits) { 
								
					bIsPlaying=false;
								iGuihandle = 3; 
							gButtonTrack.playSound(L"Sounds\\power-failure.wav",false);
							}
							ostringstream weemanHits;
	weemanHits << iMaxWeemanHits;
	string _MaxWeemanHits = weemanHits.str();
	string _missedInfo = "Missed: %d / " + _MaxWeemanHits;
	sprintf_s( gDetectorMissedStr, 50, _missedInfo.c_str(), gDetectorMissed);
							gButtonTrack.playSound(L"Sounds\\wrong.wav",false);
							skeyPress = "";
					} else {
					if(hitDex == 0) {
							gButtonTrack.playSound(L"Sounds\\wrong.wav",false);
					}
					if(hitDex > 0) {
							gButtonTrack.playSound(L"Sounds\\hit-effect.wav",false);
					}
							}				
					}
}
/*
==================================================================
* When called resets the game back to its initial state
==================================================================
*/
void Reset() {
						//Kills any existing enemy in the scene
						e_iter = aEnemy.begin();
						while (e_iter != aEnemy.end()) {
							
							if(_enemySpawnedIndex > 0)
							{
								e_iter = aEnemy.erase(e_iter);
								_enemySpawnedIndex--;
							}
							else {
										++e_iter;
							}
						}
					_previousTime = 0;
					_spawnTimer = 0;
					gDetectorsHit = 0;
					gDetectorMissed = 0;
					gDetectorsHit = 0;
					gDetectorMissed = 0;
					
	ostringstream convert;
	convert << iMaxDetectorHits;
	string _MaxDetectorHits = convert.str();
	string _hitDetectorInfo = "Hit: %d / " + _MaxDetectorHits;
	sprintf_s(gDetectorsHitStr, 50, _hitDetectorInfo.c_str(), gDetectorsHit);
	ostringstream weemanHits;
	weemanHits << iMaxWeemanHits;
	string _MaxWeemanHits = weemanHits.str();
	string _missedInfo = "Missed: %d / " + _MaxWeemanHits;
	sprintf_s( gDetectorMissedStr, 50, _missedInfo.c_str(), gDetectorMissed);

}

/*
==================================================================
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
==================================================================
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	switch (message)
	{
	case WM_KEYDOWN:
			{
				if(iController == 0) {
				//Key detection
				stringstream ss;
				string skeyPress;
				char ckeyPress = wParam;
				putchar (tolower(ckeyPress));
				ss << ckeyPress;
				ss >> skeyPress;
				transform(skeyPress.begin(), skeyPress.end(), skeyPress.begin(),tolower);
				//if using keyboard
				pressCheck(skeyPress);
				}
			}
		case WM_LBUTTONDOWN:
			{
			//	bFlash = true;
			//	 gFireSound.playSound(L"Sounds\\Shotgun_Blast-Jim_Rogers-1914772763.wav",false);
			//	 gBulletDropSound.playSound(L"Sounds\\empty-bullet-shell-fall-02.wav",false);
				POINT mouseXY;
				mouseXY.x = LOWORD(lParam);
				mouseXY.y = HIWORD(lParam);
				
				//if not main game
				if(iGuihandle!=1) {
				
				if(iGuihandle==0) {
					//Play button
				if (insideRect(mainMenuPos[1],mouseXY)) {
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
					Reset();
					bIsPlaying=false;
					iGuihandle = 1;
					return 0;
				}
				//Options button
				if (insideRect(mainMenuPos[2],mouseXY)) {
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
					iGuihandle = 5;
					return 0;
				}
				//How to play button
				if (insideRect(mainMenuPos[3],mouseXY)) {
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
					iGuihandle = 4;
					return 0;
				}
				//Exit button
				if (insideRect(mainMenuPos[4],mouseXY)) {
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
					PostQuitMessage(0);
					return 0;
				}
				}
				
				if(iGuihandle==2 || iGuihandle == 3) {
				//Play again or try button
				if (insideRect(retryPos,mouseXY)) {
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
					Reset();
					bIsPlaying=false;
					iGuihandle = 1;
				}
				}
				if(iGuihandle == 4 || iGuihandle == 5) {
				// Difficulty buttons
					//Easy
					if(insideRect(optionsPos[1],mouseXY)) {
						
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
						iDifficulty = 0;
						_delay=3.0f;
					_enemySpeed = 100.0f;
					iMaxDetectorHits = 15;
					iMaxWeemanHits = 5;
					sprintf_s( gDifficultySelectedStr, 50, "Easy"); }
					//Medium
					if(insideRect(optionsPos[2],mouseXY)) {
						
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
						iDifficulty = 1;
					_enemySpeed = 120.0f;
					iMaxDetectorHits = 50;
					iMaxWeemanHits = 3;
					_delay = 2.0f;
					sprintf_s( gDifficultySelectedStr, 50, "Medium"); }
					//Hard
					if(insideRect(optionsPos[3],mouseXY)) { iDifficulty = 2;
					_enemySpeed = 140.0f;
					iMaxDetectorHits = 75;
					iMaxWeemanHits = 1;
					_delay = 1.0f;
					sprintf_s( gDifficultySelectedStr, 50, "Hard"); }
					//Controller
					if(insideRect(optionsPos[8],mouseXY)) { 
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
						iController = 0;
					sprintf_s( gCurrentControllerSelectedStr, 50, "Keyboard"); }
					if(insideRect(optionsPos[9],mouseXY)) {
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
						iController = 1;
					sprintf_s( gCurrentControllerSelectedStr, 50, "Gamepad"); }
				}
				if(iGuihandle==2 || iGuihandle == 3 || iGuihandle == 4 || iGuihandle == 5) {
				//Back button
				if (insideRect(backPos,mouseXY)) {
					gButtonTrack.playSound(L"Sounds\\error_so-skullkee-8341_hifi.wav",false);
					iGuihandle = 0;
				}
				}
				}
				expPos = D3DXVECTOR3((float)mouseXY.x,(float)mouseXY.y, 0.0f);
				return 0;
			}
		case WM_CLOSE:
			{
			// Exit the Game
				PostQuitMessage(0);
				 return 0;
			}

		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
==================================================================
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
==================================================================
*/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	// Fill in the WNDCLASSEX structure. This describes how the window
	// will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MatcherIcon)); // icon to associate with the application
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));// the default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = "Matchers"; // the class name being created
	wcex.hIconSm = LoadIcon(hInstance,"matcher.ico"); // the handle to the small icon

	RegisterClassEx(&wcex);
	// Create the window
	wndHandle = CreateWindow("Matchers",			// the window class to use
							 "Matchers",			// the title bar text
							WS_OVERLAPPEDWINDOW,	// the window style
							CW_USEDEFAULT, // the starting x coordinate
							CW_USEDEFAULT, // the starting y coordinate
							800, // the pixel width of the window
							600, // the pixel height of the window
							NULL, // the parent window; NULL for desktop
							NULL, // the menu for the application; NULL for none
							hInstance, // the handle to the application instance
							NULL); // no values passed to the window
	// Make sure that the window handle that is created is valid
	if (!wndHandle)
		return false;
	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	return true;
}

/*
==================================================================
// This is winmain, the main entry point for Windows applications
==================================================================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if ( !initWindow( hInstance ) )
		return false;
	// called after creating the window
	if ( !d3dMgr->initD3DManager(wndHandle) )
		return false;
	if ( !d3dxSRMgr->initD3DXSpriteMgr(d3dMgr->getTheD3DDevice()))
		return false;

	// Grab the frequency of the high def timer
	__int64 freq = 0;				// measured in counts per second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	float sPC = 1.0f / (float)freq;			// number of seconds per count

	__int64 currentTime = 0;				// current time measured in counts per second;
	__int64 previousTime = 0;				// previous time measured in counts per second;

	float numFrames   = 0.0f;				// Used to hold the number of frames
	float timeElapsed = 0.0f;				// cumulative elapsed time

	GetClientRect(wndHandle,&clientBounds);

	float fpsRate = 1.0f/30.0f;
	
	D3DXVECTOR3 aDetectorPos;
	D3DXVECTOR3 aWeemanPos;
	D3DXVECTOR3 aEnemyPos;

	//XBOX Controller instance
	Player1 = new CXBOXController(1);
	
	//Start sound
	
				gInGameSoundTrack.playSound(L"Sounds\\djgriffin__slwzk4bkpdbb.wav",true);

	//Main Menu
	sprintf_s( gTitleStr,15,sMainMenu_TitleStr.c_str());
	sprintf_s( gPlayStr, 10, sMainMenu_PlayStr.c_str());
	sprintf_s( gHowToPlayStr, 20, sMainMenu_HowToPlayStr.c_str());
	sprintf_s( gOptionsStr, 10, sMainMenu_OptionsStr.c_str());
	sprintf_s( gExitStr, 10, sMainMenu_ExitStr.c_str());
	//How to play
	sprintf_s( gHowToPlayTitleStr, 20, sMainMenu_HowToPlayStr.c_str());
	sprintf_s( gEnemyDescription, 300, sHowToPlayMenu_EnemyDescriptionStr.c_str());
	sprintf_s( gDetectorDescription, 300, sHowToPlayMenu_DetectorDescriptionStr.c_str());
	sprintf_s( gWeemanDescription, 300, sHowToPlayMenu_WeemanDescriptionStr.c_str());
	//Shared
	sprintf_s( gBackStr, 10, sShared_BackStr.c_str());
	//Options
	sprintf_s( gEasyStr, 10, sOptionsMenu_DifficultyStr[0].c_str());
	sprintf_s( gMediumStr, 10, sOptionsMenu_DifficultyStr[1].c_str());
	sprintf_s( gHardStr, 10, sOptionsMenu_DifficultyStr[2].c_str());
	// Notification on difficulty setting
	sprintf_s( gDifficultyStr, 50, sOptionsMenu_CurrentDifficultyStr.c_str());
	sprintf_s( gDifficultySelectedStr, 50, sOptionsMenu_DefaultStr.c_str());

	// Notification on controller setting
	sprintf_s( gCurrentControllerStr, 100, sOptionsMenu_ControllerSelectionStr.c_str());
	sprintf_s( gCurrentControllerSelectedStr, 50, sOptionsMenu_DefaultStr.c_str());
	sprintf_s( gControllerKeyboard, 50, sOptionsMenu_KeyboardStr.c_str());
	sprintf_s( gControllerXboxController, 50, sOptionsMenu_ControllerStr.c_str());
	//Lose screen
	sprintf_s( gloseTitleStr, 20, sWinLoseMenu_LoseStr.c_str());
	sprintf_s( gtryAgainStr, 100, sWinLoseMenu_TryAgainStr.c_str());
	//Win Screen
	sprintf_s( gwonTitleStr, 20, sWinLoseMenu_WinStr.c_str());
	sprintf_s( gwinPlayAgainStr, 100, sWinLoseMenu_TryAgainStr.c_str());

	//Load One texture for the detector
	detectorTexture = new cD3DXTexture(d3dMgr->getTheD3DDevice(), detectorTxtres);
	//Load one texture for the weeman
	weemanTexture = new cD3DXTexture(d3dMgr ->getTheD3DDevice(), weemanTxtres);
	//Load one texture for the detection
	detectionTexture = new cD3DXTexture(d3dMgr ->getTheD3DDevice(), detectionTxtres);
	
	/*
	====================================================================
	* Load four textures for the enemy; w,s,a & d (y,a,x,b for xbox controller respectively)
	====================================================================
	*/
	// If using keyboard
	for (int txture = 0; txture < 4; txture++)
	{
		enemyTextures[txture] = new cD3DXTexture(d3dMgr->getTheD3DDevice(), enemyTxtres[txture]);
	} 
	// If using xbox controller
	for (int txture = 0; txture < 4; txture++)
	{
		enemyxboxTextures[txture] = new cD3DXTexture(d3dMgr->getTheD3DDevice(), enemyxboxTxtres[txture]);
	}
	
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );
	// Weeman position taking into account the size of the sprite so is centered correctly
	D3DXVECTOR3 weemanPos = D3DXVECTOR3(((float)clientBounds.right/2)-64.0f,((float)clientBounds.bottom/2)-64.0f,0);


	D3DXVECTOR3 dectorPos [4] = {D3DXVECTOR3((((float)clientBounds.right/2)-200.0f),(((float)clientBounds.bottom/2)-50.0f),0),
		D3DXVECTOR3((((float)clientBounds.right/2)+100.0f),(((float)clientBounds.bottom/2)-50.0f),0),
		D3DXVECTOR3(((float)clientBounds.right/2)-50.0f,(((float)clientBounds.bottom/2)-200.0f),0),
		D3DXVECTOR3(((float)clientBounds.right/2)-50.0f,(((float)clientBounds.bottom/2)+100.0f),0)};
	

	// Detector transation will be based on a global variable _detectorSpeed that can be quickly toggled to increase/reduce difficulty
	D3DXVECTOR2 dectorTrans [4] = {D3DXVECTOR2(_detectorSpeed,0.0f),
		D3DXVECTOR2(-_detectorSpeed,0.0f),
		D3DXVECTOR2(0.0f,_detectorSpeed),
		D3DXVECTOR2(0.0f,-_detectorSpeed)};
	
	// 1:w 2:a 3:s 4:d
	D3DXVECTOR3 enemyPos [4] = {
		D3DXVECTOR3(((float)clientBounds.right/2)-32.0f,-32.0f,0),
		D3DXVECTOR3(-32.0f,((float)clientBounds.bottom/2)-32.0f,0),
		D3DXVECTOR3(((float)clientBounds.right/2)-32.0f,((float)clientBounds.bottom)-32.0f,0),
		D3DXVECTOR3(((float)clientBounds.right)-32.0f,((float)clientBounds.bottom/2)-32.0f,0),
	};

	//Generates 4 detectors
	for(int x=0; x<4; x++) {
		aDetector.push_back(new cDetector());
		aDetector[x]->setSpritePos(dectorPos[x]);
		aDetector[x]->setTranslation(dectorTrans[x]);
		aDetector[x]->setTexture(detectorTexture);
	}

	//Generates 1 weeman
	for(int y=0; y<1; y++) {
	aWeeman.push_back(new cWeeman());
	aWeeman[y]->setSpritePos(weemanPos);
	aWeeman[y]->setTranslation(D3DXVECTOR2(0.0f,0.0f));
	aWeeman[y]->setTexture(weemanTexture);
	}
	
	LPDIRECT3DSURFACE9 aSurface;				// the Direct3D surface
//	LPDIRECT3DSURFACE9 aSurfaceMenu;			// the Direct3D Menu Surface
	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer

	
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	// Create the background surface for the main game
	aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\retrobackground.png");
	
	// Create the background surface for the menus
//	aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\retrobackground.png");


	// load custom font
	cD3DXFont* amCapFont = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "American Captain", IDF_MySolidFont);
	cD3DXFont* cinGothFont48 = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "Cinema Gothic BTN Inline", IDF_MySolidFont, 64);
	cD3DXFont* sogoeUIFont18 = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "Sogoe UI Light", IDF_MySolidFont, 18);

	//Main game positions
	RECT textPos;
	SetRect(&textPos, 40, 10, 320, 100);

	RECT textPosSolid;
	SetRect(&textPosSolid, 630, 10, 800, 150);
	//Main Menu Button Positions

	//Essentially the button's height is their font size
	mainMenuYPos[0] = clientBounds.top + 42;
	mainMenuYPos[1] = clientBounds.top + 100;	
	mainMenuYPos[2] = clientBounds.top + 150;	
	mainMenuYPos[3] = clientBounds.top + 200;		
	mainMenuYPos[4] = clientBounds.top + 250;	
	SetRect(&mainMenuPos[0], (int) floor(clientBounds.right*0.34), mainMenuYPos[0], (int) floor(clientBounds.right*0.7), mainMenuYPos[0]+64);
	SetRect(&mainMenuPos[1], (int) floor(clientBounds.right*0.4), mainMenuYPos[1], (int) floor(clientBounds.right*0.6), mainMenuYPos[1]+32);
	SetRect(&mainMenuPos[2], (int) floor(clientBounds.right*0.4), mainMenuYPos[2], (int) floor(clientBounds.right*0.6), mainMenuYPos[2]+32);
	SetRect(&mainMenuPos[3], (int) floor(clientBounds.right*0.4), mainMenuYPos[3], (int) floor(clientBounds.right*0.6), mainMenuYPos[3]+32);
	SetRect(&mainMenuPos[4], (int) floor(clientBounds.right*0.4), mainMenuYPos[4], (int) floor(clientBounds.right*0.6), mainMenuYPos[4]+32);
	//Back button positions
	SetRect(&backPos, (int) floor(clientBounds.right*0.5), 440, (int) floor(clientBounds.right*0.7), 460);
	SetRect(&retryPos, (int) floor(clientBounds.right*0.2), 400, (int) floor(clientBounds.right*0.5), 440);
	//How to play positions
	SetRect(&howToPlayPos[0], (int) floor(clientBounds.right*0.25), 150, (int) floor(clientBounds.right*0.75), 190);
	SetRect(&howToPlayPos[1], (int) floor(clientBounds.right*0.25), 210, (int) floor(clientBounds.right*0.75), 260);
	SetRect(&howToPlayPos[2], (int) floor(clientBounds.right*0.25), 280, (int) floor(clientBounds.right*0.75), 320);
	SetRect(&howToPlayPos[3], (int) floor(clientBounds.right*0.25), 360, (int) floor(clientBounds.right*0.75), 380);
	SetRect(&howToPlayPos[4], (int) floor(clientBounds.right*0.475), 400, (int) floor(clientBounds.right*0.7), 430);
	//Options positions
	SetRect(&optionsPos[0], (int) floor(clientBounds.right*0.3), 150, (int) floor(clientBounds.right*0.7), 190);
	SetRect(&optionsPos[1], (int) floor(clientBounds.right*0.35), 210, (int) floor(clientBounds.right*0.5), 230);
	SetRect(&optionsPos[2], (int) floor(clientBounds.right*0.35), 250, (int) floor(clientBounds.right*0.5), 270);
	SetRect(&optionsPos[3], (int) floor(clientBounds.right*0.35), 290, (int) floor(clientBounds.right*0.5), 310);
	//Current difficulty position
	SetRect(&optionsPos[4], (int) floor(clientBounds.right*0.5), 210, (int) floor(clientBounds.right*0.7), 230);
	SetRect(&optionsPos[5], (int) floor(clientBounds.right*0.5), 250, (int) floor(clientBounds.right*0.7), 270);
	//Current controller position
	SetRect(&optionsPos[6], (int) floor(clientBounds.right*0.5), 350, (int) floor(clientBounds.right*0.7), 370);
	SetRect(&optionsPos[7], (int) floor(clientBounds.right*0.5), 390, (int) floor(clientBounds.right*0.7), 410);
	SetRect(&optionsPos[8], (int) floor(clientBounds.right*0.25), 350, (int) floor(clientBounds.right*0.45), 370);
	SetRect(&optionsPos[9], (int) floor(clientBounds.right*0.25), 390, (int) floor(clientBounds.right*0.45), 410);
	
	//Title
	SetRect(&titlePos,(int) floor(clientBounds.right*0.2), mainMenuYPos[1], (int) floor(clientBounds.right*0.8), mainMenuYPos[1]+32);
	
	QueryPerformanceCounter((LARGE_INTEGER*)&previousTime);

	while( msg.message!=WM_QUIT )
	{
		// Check the message queue
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Game code goes here
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			float dt = (currentTime - previousTime)*sPC;

			// Accumulate how much time has passed.
			timeElapsed += dt;
			

			//if menu 
			if(iGuihandle != 1 && bIsPlaying==false){
				// Looping soundtrack
				gInGameSoundTrack.stopSounds();
				gInGameSoundTrack.playSound(L"Sounds\\djgriffin__slwzk4bkpdbb.wav",true);
				bIsPlaying=true;
			}
			//If main game
			else if (iGuihandle == 1 && bIsPlaying == false) {
				gInGameSoundTrack.stopSounds();
				gInGameSoundTrack.playSound(L"Sounds\\djgriffin__root-d.wav",true);
				bIsPlaying = true;
			}

			if(timeElapsed > fpsRate)
			{
	/*
	===========================================================
	Xbox Controller Controls
	===========================================================
	*/
		if(Player1->IsConnected())
		{
			// In Game Controls
			if(iGuihandle == 1) {
				if(iController == 1) {
					if(fCurrentPressTime >= fPressInterval && bPressed==false) {
			if((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A))
			{
				pressCheck("a");
				bPressed=true;
				fCurrentPressTime = 0.0f;
				fCurrentVibrateTime = 0.0f;	
			}
			if((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B))
			{
				pressCheck("b");
				bPressed=true;	
				fCurrentPressTime = 0.0f;
				fCurrentVibrateTime = 0.0f;	
			}
			if((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X))
			{
				pressCheck("x");	
				bPressed=true;	
				fCurrentPressTime = 0.0f;
				fCurrentVibrateTime = 0.0f;
			}
			else if((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y))
			{
				pressCheck("y");
				bPressed=true;
				fCurrentPressTime = 0.0f;
				fCurrentVibrateTime = 0.0f;	
			}
			}
					//Press interval
				if(bPressed == true && fCurrentPressTime >= fPressInterval) {
						bPressed = false;
				}
				if(bPressed == true) {
					fCurrentPressTime += timeElapsed;
				}

					//Turn vibration back off
			if(bVibrating == true && fCurrentVibrateTime >= fVibrateTime) {
						Player1->Vibrate(0, 0);
						bVibrating = false;
					}
			//Turn on vibration when key is pressed
			if(fCurrentVibrateTime < (fVibrateTime + 0.1f) && bVibrating == false) {
						Player1->Vibrate(65535, 65535);
						bVibrating = true;
					}
			if(bVibrating==true) {
						fCurrentVibrateTime += timeElapsed;
			}
				}
			}
			else { 
			Player1->Vibrate(0, 0);
			}
		}
				

			/*
			==============================================================
			| Spawner
			==============================================================
			*/
				if(iGuihandle == 1) {
					D3DXVECTOR2 enemyTrans [4] = {D3DXVECTOR2(0.0f,_enemySpeed),
		D3DXVECTOR2(_enemySpeed,0.0f),
		D3DXVECTOR2(0.0f,-_enemySpeed),
		D3DXVECTOR2(-_enemySpeed,0.0f)};
	
					_spawnTimer = _spawnTimer + timeElapsed;
				//If time is greater than previous time then spawn a new enemy
	if(_spawnTimer >= _previousTime) {
	_previousTime = _spawnTimer + _delay;
	int loop = 0;
	//How many objects to spawn this turn
	loop = (rand() % 3 + 1);
	int index = 0;
	for(int z=0; z<loop; z++) {
	//Picks one of the 4 points
	index = (rand() % 4);
	aEnemy.push_back(new cEnemy());
	//Dies at line below !!!
	aEnemy[_enemySpawnedIndex]->setSpritePos(enemyPos[index]);
	aEnemy[_enemySpawnedIndex]->setTranslation(enemyTrans[index]);
	if(iController == 0) {
	aEnemy[_enemySpawnedIndex]->setTexture(enemyTextures[index]);
	} else if(iController == 1) {
	aEnemy[_enemySpawnedIndex]->setTexture(enemyxboxTextures[index]);
	}
	if(iController == 0) {
	aEnemy[_enemySpawnedIndex]->setKey(keys[index]);
	} else if (iController == 1) {
	aEnemy[_enemySpawnedIndex]->setKey(xboxkeys[index]);
	}
	_enemySpawnedIndex++;
		}
	}
	
			/*
			==============================================================
			| Update the postion of the weeman
			==============================================================
			*/
				for(w_iter = aWeeman.begin(); w_iter != aWeeman.end(); ++w_iter)
				{
					(*w_iter)->update(timeElapsed);			// update weeman
					aWeemanPos = (*w_iter)->getSpritePos();  // get the position of weeman
				}	
			/*
			==============================================================
			| Update the postion of the enemy
			==============================================================
			*/
				//for(e_iter = aEnemy.begin(); e_iter != aEnemy.end(); ++e_iter)
				e_iter = aEnemy.begin();
				w_iter = aWeeman.begin();
				while(e_iter != aEnemy.end())
				{
					(*e_iter)->update(timeElapsed);			// update enemy
					aEnemyPos = (*e_iter)->getSpritePos();  // get the position of the current enemy
					/*
					==============================================================
					| Check each enemy to see if they have hit weeman
					==============================================================
					*/				
						//If enemy collided with weeman
						if ((*e_iter)->collidedWith((*e_iter)->getBoundingRect(),(*w_iter)->getBoundingRect()))
						{
							// if a collision occurs, increment hits by 1 and de-activate enemy
							gDetectorMissed++;
							if(gDetectorMissed >= iMaxWeemanHits) {
								bIsPlaying=false;
								iGuihandle = 3; }
	
							ostringstream weemanHits;
	weemanHits << iMaxWeemanHits;
	string _MaxWeemanHits = weemanHits.str();


	string _missedInfo = "Missed: %d / " + _MaxWeemanHits;
	sprintf_s( gDetectorMissedStr, 50, _missedInfo.c_str(), gDetectorMissed);
							e_iter = aEnemy.erase(e_iter);
							gButtonTrack.playSound(L"Sounds\\wrong.wav",false);
							_enemySpawnedIndex--;
						}
						else
						{
							++e_iter;
						}
				}
			
			/*
			==============================================================
			| Update the postion of the detectors
			==============================================================
			*/
				for(d_iter = aDetector.begin(); d_iter != aDetector.end(); ++d_iter)
				{
					(*d_iter)->update(timeElapsed);			// update detector
					aDetectorPos = (*d_iter)->getSpritePos();  // get the position of the current detector
				
					/*
					==============================================================
					| Check for boundry collision and change detector direction
					==============================================================
					*/
					//If | 6.25% away from right and 6.25% | left. Then | 6.25% centre 6.25% | . 6.25% top and bottom, centre. Need to take into account the size of the sprite.
					if((aDetectorPos.x < (clientBounds.right*0.1)-50.0f || (aDetectorPos.x > clientBounds.right*0.35-50.0f && aDetectorPos.x < clientBounds.right*0.5-50.0f) || 
						(aDetectorPos.x > clientBounds.right*0.5-50.0f && aDetectorPos.x < clientBounds.right*0.65-50.0f) || aDetectorPos.x > clientBounds.right*0.9-50.0f)
						|| aDetectorPos.y < (clientBounds.bottom * 0.1-50.0f) || (aDetectorPos.y > clientBounds.bottom*0.3-50.0f &&  aDetectorPos.y < clientBounds.bottom*0.5-50.0f) ||
						(aDetectorPos.y > clientBounds.bottom*0.5-50.0f && aDetectorPos.y < clientBounds.bottom*0.7-50.0f) || aDetectorPos.y > clientBounds.bottom*0.9-50.0f)
					{
						(*d_iter)->setTranslation((*d_iter)->getTranslation()*(-1));
					}				
				}
				}
		
				//if normal do this
				d3dMgr->beginRender();
				theBackbuffer = d3dMgr->getTheBackBuffer();
				d3dMgr->updateTheSurface(aSurface, theBackbuffer);				
				d3dMgr->releaseTheBackbuffer(theBackbuffer);
				d3dxSRMgr->beginDraw();
				
				//If iGuihandle = 1 (Main Game)
				if(iGuihandle == 1) {
				vector<cDetector*>::iterator d_iter= aDetector.begin();
				for(d_iter = aDetector.begin(); d_iter != aDetector.end(); ++d_iter)
				{
					d3dxSRMgr->setTheTransform((*d_iter)->getSpriteTransformMatrix());  
					d3dxSRMgr->drawSprite((*d_iter)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
				}

				vector<cWeeman*>::iterator w_iter= aWeeman.begin();
				for(w_iter = aWeeman.begin(); w_iter != aWeeman.end(); ++w_iter)
				{
					d3dxSRMgr->setTheTransform((*w_iter)->getSpriteTransformMatrix());  
					d3dxSRMgr->drawSprite((*w_iter)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
				}
				vector<cEnemy*>::iterator e_iter= aEnemy.begin();
				for(e_iter = aEnemy.begin(); e_iter != aEnemy.end(); ++e_iter)
				{
					d3dxSRMgr->setTheTransform((*e_iter)->getSpriteTransformMatrix());  
					d3dxSRMgr->drawSprite((*e_iter)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
				}
				}

				//The detector animation sprite
				list<cExplosion*>::iterator iter = gFound.begin();
				while(iter != gFound.end())
				{
					if((*iter)->isActive() == false)
					{
						iter = gFound.erase(iter);
					}
					else
					{
						(*iter)->update(timeElapsed);
						d3dxSRMgr->setTheTransform((*iter)->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite((*iter)->getTexture(),&((*iter)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
						++iter;
					}
				}
				
				d3dxSRMgr->endDraw();

				// Main Menu
				if(iGuihandle == 0) {
				cinGothFont48->printText(gTitleStr,mainMenuPos[0], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gPlayStr,mainMenuPos[1], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gOptionsStr,mainMenuPos[2], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gHowToPlayStr,mainMenuPos[3], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gExitStr,mainMenuPos[4], D3DCOLOR_XRGB( 0, 0, 0 ));
				}
				//  How to play 
				if(iGuihandle == 4) {
				cinGothFont48->printText(gHowToPlayTitleStr,howToPlayPos[0], D3DCOLOR_XRGB( 0, 0, 0 ));
				sogoeUIFont18->printText(gEnemyDescription,howToPlayPos[1], D3DCOLOR_XRGB( 0, 0, 0 ));
				sogoeUIFont18->printText(gDetectorDescription,howToPlayPos[2], D3DCOLOR_XRGB( 0, 0, 0 ));				
				sogoeUIFont18->printText(gWeemanDescription,howToPlayPos[3], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gBackStr,backPos, D3DCOLOR_XRGB( 0, 0, 0 ));
				//weemanTxtres
				}
				
				// Options
				if(iGuihandle == 5) {
				cinGothFont48->printText(gOptionsStr,optionsPos[0], D3DCOLOR_XRGB( 0, 0, 0 ));
				//Difficulty
				amCapFont->printText(gEasyStr,optionsPos[1], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gMediumStr,optionsPos[2], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gHardStr,optionsPos[3], D3DCOLOR_XRGB( 0, 0, 0 ));
				
				// Notification on difficulty
				amCapFont->printText(gDifficultyStr,optionsPos[4], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gDifficultySelectedStr,optionsPos[5], D3DCOLOR_XRGB( 0, 0, 0 ));
				
				// Notification on player controller
				amCapFont->printText(gCurrentControllerStr,optionsPos[6], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gCurrentControllerSelectedStr,optionsPos[7], D3DCOLOR_XRGB( 0, 0, 0 ));
				
				// Controller types
				amCapFont->printText(gControllerKeyboard,optionsPos[8], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gControllerXboxController,optionsPos[9], D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gBackStr,backPos, D3DCOLOR_XRGB( 0, 0, 0 ));
				}
				// Main Game
				if(iGuihandle == 1) {
				amCapFont->printText(gDetectorsHitStr,textPosSolid, D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gDetectorMissedStr,textPos, D3DCOLOR_XRGB( 0, 0, 0 ));
				}
				// Win Screen
				if(iGuihandle == 2) {
				cinGothFont48->printText(gwonTitleStr,titlePos, D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gtryAgainStr,retryPos, D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gBackStr,backPos, D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gDetectorsHitStr,mainMenuPos[3], D3DCOLOR_XRGB( 0, 0, 0 ));
				}
				// Lose Screen
				if(iGuihandle == 3) {
				cinGothFont48->printText(gloseTitleStr,titlePos, D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gtryAgainStr,retryPos, D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gBackStr,backPos, D3DCOLOR_XRGB( 0, 0, 0 ));
				amCapFont->printText(gDetectorsHitStr, mainMenuPos[3], D3DCOLOR_XRGB( 0, 0, 0 ));
				}
								
				d3dMgr->endRender();				
				timeElapsed = 0.0f;
			}
			previousTime = currentTime;
		}
		
	}
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();
	return (int) msg.wParam;
}
