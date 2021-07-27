// 3DEngine.h: interface for the C3DEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DENGINE_H__74548AAE_C109_44C5_B748_B9AA80913CF1__INCLUDED_)
#define AFX_3DENGINE_H__74548AAE_C109_44C5_B748_B9AA80913CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <D3DX8.h>
#include "D3DFile.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DIUtil.h"
#include "DMUtil.h"
#include "DXUtil.h"

#include <basetsd.h>
#include <cguid.h>
#include <tchar.h>
#include <mmsystem.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"
#include "gamemenu.h"







// Error codes
#define DONUTS3DERR_NODIRECT3D       0x00000001
#define DONUTS3DERR_NOD3DDEVICE      0x00000002
#define DONUTS3DERR_NOTEXTURES       0x00000003
#define DONUTS3DERR_NOGEOMETRY       0x00000004
#define DONUTS3DERR_NO3DRESOURCES    0x00000005
#define DONUTS3DERR_NOINPUT          0x00000006


// States the app can be in
enum{ APPSTATE_LOADSPLASH, APPSTATE_DISPLAYSPLASH, APPSTATE_ACTIVE, 
      APPSTATE_BEGINLEVELSCREEN, APPSTATE_DISPLAYLEVELSCREEN };


// Game object types
enum{ OBJ_DONUT=0, OBJ_PYRAMID, OBJ_CUBE, OBJ_SPHERE, OBJ_CLOUD, OBJ_SHIP,
      OBJ_BULLET };


// Object dimensions and fixed properties
#define DONUT_WIDTH        64
#define DONUT_HEIGHT       64
#define PYRAMID_WIDTH      64
#define PYRAMID_HEIGHT     64
#define SPHERE_WIDTH       64
#define SPHERE_HEIGHT      64
#define CUBE_WIDTH         64
#define CUBE_HEIGHT        64
#define CLOUD_WIDTH        32
#define CLOUD_HEIGHT       32
#define BULLET_WIDTH        3
#define BULLET_HEIGHT       3

#define NUM_DONUT_FRAMES   3
#define NUM_PYRAMID_FRAMES 3
#define NUM_SPHERE_FRAMES  3
#define NUM_CUBE_FRAMES    3
#define NUM_BULLET_FRAMES 400

#define BULLET_XOFFSET    304
#define BULLET_YOFFSET      0









// Defines for the in-game menu
#define MENU_MAIN           1
#define MENU_SOUND          2
#define MENU_VIDEO          3
#define MENU_INPUT          4
#define MENU_VIEWDEVICES    5
#define MENU_CONFIGDEVICES  6
#define MENU_WINDOWED       7
#define MENU_640x480        8
#define MENU_800x600        9
#define MENU_1024x768      10
#define MENU_BACK          11
#define MENU_SOUNDON       12
#define MENU_SOUNDOFF      13
#define MENU_QUIT          14







//-----------------------------------------------------------------------------
// Custom Direct3D vertex types
//-----------------------------------------------------------------------------
struct SCREENVERTEX
{
    D3DXVECTOR4 p;
    DWORD       color;
};

struct SPRITEVERTEX
{
    D3DXVECTOR3 p;
    DWORD       color;
    FLOAT       tu, tv;
};

struct MODELVERTEX
{
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu, tv;
};

#define D3DFVF_SCREENVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define D3DFVF_SPRITEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_MODELVERTEX  (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)









//-----------------------------------------------------------------------------
// Game actions (using DInput semantic mapper). The definitions here are kind
// of the whole point of this sample. The game uses these actions to map
// physical input like, "the user pressed the 'W' key", to a more useable
// constant for the game, like "if( dwInput == INPUT_CHANGEWEAPONS )...".
//-----------------------------------------------------------------------------


// Input semantics used by this game
enum INPUT_SEMANTICS
{
    // Gameplay semantics
    INPUT_AXIS_LR=1,     INPUT_AXIS_UD,       INPUT_AXIS_HL,
    INPUT_MOUSE_LR,      INPUT_MOUSE_UD,      INPUT_MOUSE_HL,
	INPUT_RAISE,		 INPUT_LOWER,
    INPUT_TURNLEFT,      INPUT_TURNRIGHT,     INPUT_FORWARDTHRUST,
    INPUT_REVERSETHRUST, INPUT_FIREWEAPONS,   INPUT_CHANGESHIPTYPE,
    INPUT_CHANGEVIEW,    INPUT_CHANGEWEAPONS, INPUT_DISPLAYGAMEMENU,
    INPUT_QUITGAME,      INPUT_START,

    // Menu semantics
    INPUT_MENU_LR,       INPUT_MENU_UD,       INPUT_MENU_WHEEL,
    INPUT_MENU_UP,       INPUT_MENU_DOWN,     INPUT_MENU_LEFT,
    INPUT_MENU_RIGHT,    INPUT_MENU_SELECT,   INPUT_MENU_QUIT,
};






//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------

// Sound functions
HRESULT          CreateSoundObjects( HWND hWnd );

// Input functions
HRESULT          CreateInputObjects( HWND hWnd );

BOOL CALLBACK    ConfigureInputDevicesCB( IUnknown* pUnknown, VOID* pUserData );
VOID             GetInput();

// Display functions
HRESULT          CreateDisplayObjects( HWND hWnd );
HRESULT          RestoreDisplayObjects();
HRESULT          InvalidateDisplayObjects();


// Menu functions
VOID             ConstructMenus();
VOID             UpdateMenus();


// Rendering functions
HRESULT          FrameMove();
HRESULT          RenderFrame();
VOID             UpdateDisplayList();



// Misc game functions
VOID             DisplayLevelIntroScreen( DWORD dwLevel );
BOOL             IsDisplayListEmpty();
//VOID             AddToList( DisplayObject* pObject );
//VOID             DeleteFromList( DisplayObject* pObject );
VOID             CheckForHits();


// Error handling
VOID             CleanupAndDisplayError( DWORD dwError );





//-----------------------------------------------------------------------------
// Name: struct DisplayObject
// Desc: A game object that goes in the display list
//-----------------------------------------------------------------------------
struct DisplayObject
{
    DisplayObject* pNext;          // Link to next object
    DisplayObject* pPrev;          // Link to previous object
    
    DWORD          dwType;            // Type of object
    BOOL           bVisible;          // Whether the object is visible
    D3DXVECTOR3    vPos;              // Position
    D3DXVECTOR3    vVel;              // Velocity
    FLOAT          fSize;
    
    // Constructor
    DisplayObject( DWORD type, D3DVECTOR p, D3DVECTOR v );
};





//-----------------------------------------------------------------------------
// Derived classes for displayable game objects
//-----------------------------------------------------------------------------
struct C3DSprite : public DisplayObject
{
    DWORD dwFramesPerLine;   // How anim frames are packed in bitmap
    FLOAT frame;             // Current animation frame
    FLOAT fMaxFrame;         // Max animation frame value
    FLOAT delay;             // Frame/second
    
    DWORD dwColor;

    DWORD dwTextureOffsetX; // Pixel offsets into the game texture
    DWORD dwTextureOffsetY;
    DWORD dwTextureWidth;   // Width and height in pixels
    DWORD dwTextureHeight; 
    
    C3DSprite( DWORD type, D3DVECTOR p, D3DVECTOR v );
};


class CDonut : public C3DSprite
{
public:
    CDonut( D3DVECTOR p, D3DVECTOR v );
};


class CPyramid : public C3DSprite
{
public:
    CPyramid( D3DVECTOR p, D3DVECTOR v );
};


class CSphere : public C3DSprite
{
public:
    CSphere( D3DVECTOR p, D3DVECTOR v );
};


class CCube : public C3DSprite
{
public:
    CCube( D3DVECTOR p, D3DVECTOR v );
};


class CCloud : public C3DSprite
{
public:
    CCloud( D3DVECTOR p, D3DVECTOR v );
};


class CBullet : public C3DSprite
{
public:
    CBullet( D3DVECTOR p, D3DVECTOR v, DWORD dwType );
};


class CShip : public DisplayObject
{
public:
    FLOAT fRoll;
	FLOAT fRollH;


    FLOAT fAngle;
	FLOAT fAngleH;

    BOOL  bExploded;
    FLOAT fShowDelay;

public:
    CShip( D3DVECTOR p );
};










class C3DEngine  
{
public:
	C3DEngine();
	virtual ~C3DEngine();

	VOID   DestroyGameObjects();

	HRESULT CreateGameObjects( HWND hWnd );
	VOID AdvanceLevel();
	HRESULT SwitchModel();
	HRESULT FrameMove();
	HRESULT RenderFrame();
	VOID DarkenScene( FLOAT fAmount );
	VOID RenderFieryText( CD3DFont* pFont, TCHAR* strText );
	VOID DisplayLevelIntroScreen( DWORD dwLevel );
	VOID UpdateDisplayList();
	VOID CheckForHits();
	VOID DrawDisplayList();
	VOID DeleteFromList( DisplayObject* pObject );
	VOID ConstructMenus();
	VOID DestroyMenus();
	VOID UpdateMenus();
	HRESULT CreateDisplayObjects( HWND hWnd );
	HRESULT RestoreDisplayObjects();
	HRESULT InvalidateDisplayObjects();
	HRESULT DestroyDisplayObjects();
	HRESULT SwitchDisplayModes( BOOL bFullScreen, DWORD dwWidth, DWORD dwHeight );
	VOID ShowFrame();
	HRESULT CreateSoundObjects( HWND hWnd );
	VOID DestroySoundObjects();
	HRESULT CreateInputObjects( HWND hWnd );
	VOID DestroyInputObjects();
	VOID GetInput();
	VOID CleanupAndDisplayError( DWORD dwError );

	VOID AddToList( DisplayObject* pObject );
	BOOL IsDisplayListEmpty();
	HRESULT LoadTerrainModel();
	HRESULT LoadShipModel();

	//inline functions
	inline FLOAT HeightField( FLOAT x, FLOAT z );	
	inline VOID PlaySound( CMusicSegment* pSound );
	inline VOID StopSound( CMusicSegment* pSound );




//-----------------------------------------------------------------------------
// Application globals
//-----------------------------------------------------------------------------
 TCHAR*               g_strAppName;    
 GUID                 g_AppGuid;        

 HWND                 g_hWndMain;          // Main window
 DWORD                g_dwScreenWidth  ;   // Dimensions for fullscreen modes
 DWORD                g_dwScreenHeight ;
 D3DDISPLAYMODE       g_DesktopMode;
 D3DFORMAT            g_d3dfmtFullscreen;  // Pixel format for fullscreen modes
 D3DFORMAT            g_d3dfmtTexture;     // Pixel format for textures
 BOOL                 g_bFullScreen    ;   // Whether app is fullscreen (or windowed)
 BOOL                 g_bIsActive;         // Whether app is active
 BOOL                 g_bDisplayReady  ;   // Whether display class is initialized
 BOOL                 g_bMouseVisible  ;   // Whether mouse is visible
 HBITMAP              g_hSplashBitmap  ;   // Bitmap for splash screen

 DWORD                g_dwAppState;        // Current state the app is in
 DWORD                g_dwLevel        ;   // Current game level
 DWORD                g_dwScore        ;   // Current game score




// Player view mode
#define NUMVIEWMODES 3
CD3DCamera           g_Camera;                       // Camera used for 3D scene
DWORD                g_dwViewMode           ;     // Which view mode is being used
FLOAT                g_fViewTransition      ;  // Amount used to transittion views
BOOL                 g_bAnimatingViewChange ; // Whether view is transitioning
BOOL                 g_bFirstPersonView     ;  // Whether view is first-person

// Bullet mode
FLOAT                g_fBulletRechargeTime  ;  // Recharge time for firing bullets
DWORD                g_dwBulletType         ;    // Current bullet type

// Display list and player ship
DisplayObject*       g_pDisplayList ;          // Global display list
CShip*               g_pShip        ;          // Player's display object

// DirectDraw/Direct3D objects
LPDIRECT3DDEVICE8       g_pd3dDevice        ;  // Class to handle D3D device
D3DPRESENT_PARAMETERS   g_d3dpp;
LPDIRECT3DSURFACE8      g_pConfigSurface    ;  // Surface for config'ing DInput devices
LPDIRECT3DVERTEXBUFFER8 g_pViewportVB       ;
LPDIRECT3DVERTEXBUFFER8 g_pSpriteVB         ;

// Support for the ship model
CD3DMesh*            g_pShipFileObject   ;      // Geometry model of player's ship
DWORD                g_dwNumShipTypes    ;
DWORD                g_dwCurrentShipType ;
TCHAR*               g_strShipFiles[10]    ;
TCHAR*               g_strShipNames[10]    ;

// DirectMusic objects
CMusicManager*       g_pMusicManager        ;  // Class to manage DMusic objects
CMusicSegment*       g_pBeginLevelSound     ;  // Sounds for the app
CMusicSegment*       g_pEngineIdleSound     ;
CMusicSegment*       g_pEngineRevSound      ;
CMusicSegment*       g_pShieldBuzzSound     ;
CMusicSegment*       g_pShipExplodeSound    ;
CMusicSegment*       g_pFireBulletSound     ;
CMusicSegment*       g_pShipBounceSound     ;
CMusicSegment*       g_pDonutExplodeSound   ;
CMusicSegment*       g_pPyramidExplodeSound ;
CMusicSegment*       g_pCubeExplodeSound    ;
CMusicSegment*       g_pSphereExplodeSound  ;

// Game objects
LPDIRECT3DTEXTURE8   g_pGameTexture1 ; // Texture with game object animations
CD3DMesh*            g_pTerrain        ;    // Geometry model of terrain
CD3DFont*            g_pGameFont       ;    // Font for displaying score, etc.
CD3DFont*            g_pMenuFont       ;    // Font for displaying in-game menus

// Menu objects
CMenuItem*           g_pMainMenu       ;    // Menu class for in-game menus
CMenuItem*           g_pQuitMenu       ;
CMenuItem*           g_pCurrentMenu    ;


// DirectInput objects
CInputDeviceManager* g_pInputDeviceManager; // Class for managing DInput devices
DIACTIONFORMAT       g_diafGame;                   // Action format for game play
DIACTIONFORMAT       g_diafBrowser;                // Action format for menu navigation

// Game input variables
FLOAT                g_fBank           ;
FLOAT                g_fBankH          ;

FLOAT                g_fThrust         ;
BOOL                 g_bFiringWeapons  ;
BOOL                 g_bChangeView     ;
BOOL                 g_bPaused         ;

// Menu input variables
BOOL                 g_bMenuLeft       ;
BOOL                 g_bMenuRight      ;
BOOL                 g_bMenuUp         ;
BOOL                 g_bMenuDown       ;
BOOL                 g_bMenuSelect     ;
BOOL                 g_bMenuQuit       ;




protected:
	void Init();
};




FLOAT rnd( FLOAT low=-1.0f, FLOAT high=1.0f );










#endif // !defined(AFX_3DENGINE_H__74548AAE_C109_44C5_B748_B9AA80913CF1__INCLUDED_)
