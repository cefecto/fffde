/* Main pogramm code*/


#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "config.h"
#include "graphicsclass.h"
#include "bass\bass.h"
#include "CLogger.h"
#include "sync.h"

typedef struct
{
    //---------------
    HINSTANCE   hInstance;
    HDC         hDC;
    HGLRC       hRC;
    HWND        hWnd;
    //---------------
    int         full;
    //---------------
 	LPCWSTR		m_applicationName;
	//---------------
}WININFO;

extern "C" int _fltused = 0;


// globals
static WININFO wininfo = {  0,0,0,0,0,L"Efecto" };



static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg==WM_SYSCOMMAND && (wParam==SC_SCREENSAVE || wParam==SC_MONITORPOWER) )
		return( 0 );

	// close and escape
	if( uMsg==WM_CLOSE || uMsg==WM_DESTROY || (uMsg==WM_KEYDOWN && wParam==VK_ESCAPE) )
		{
		PostQuitMessage(0);
        return( 0 );
		}

    if( uMsg==WM_CHAR || uMsg==WM_KEYDOWN)
        {
        if( wParam==VK_ESCAPE )
            {
            PostQuitMessage(0);
            return( 0 );
            }
        }

    return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
}



static void window_end( WININFO *info )
{
    if( info->hDC  ) ReleaseDC( info->hWnd, info->hDC );
    if( info->hWnd ) DestroyWindow( info->hWnd );

    UnregisterClass( info->m_applicationName, info->hInstance );

    if( info->full )
        {
        ChangeDisplaySettings( 0, 0 );
		while( ShowCursor( 1 )<0 ); // show cursor
        }
}

static int window_init( WININFO *info )
{
    DWORD			dwExStyle, dwStyle;
    DEVMODE			dmScreenSettings;
    RECT			rec;
    WNDCLASS		wc;

    ZeroMemory( &wc, sizeof(WNDCLASS) );
    wc.style         = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = info->hInstance;
    wc.lpszClassName = info->m_applicationName;
    wc.hbrBackground=(HBRUSH)CreateSolidBrush(0x00785838);
	
    if( !RegisterClass(&wc) )
        return( 0 );

    if( info->full )
    {
        dmScreenSettings.dmSize       = sizeof(DEVMODE);
        dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmPelsWidth  = XRES;
        dmScreenSettings.dmPelsHeight = YRES;

        if( ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            return( 0 );

        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_VISIBLE | WS_POPUP;// | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		while( ShowCursor( 0 )>=0 );	// hide cursor
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle   = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    }

    rec.left   = 0;
    rec.top    = 0;
    rec.right  = XRES;
    rec.bottom = YRES;

    AdjustWindowRect( &rec, dwStyle, 0 );

    info->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, L"efecto Liquid Challenge", dwStyle,
                               (GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
                               (GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
                               rec.right-rec.left, rec.bottom-rec.top, 0, 0, info->hInstance, 0 );

    if( !info->hWnd )
        return( 0 );

    info->hDC = GetDC(info->hWnd);
    if( !info->hDC )
        return( 0 );
    
    return( 1 );
}


static void DrawTime( WININFO *info, float t )
{
    static int      frame=0;
    static float    to=0.0;
    static int      fps=0;
    char            str[64];
    int             s, m;


    if( t<0.0f) return;
    if( info->full ) return;

    frame++;
    if( (t-to)>1.0f )
        {
        fps = frame;
        to = t;
        frame = 0;
        }

    if( !(frame&3) )
        {
          
        m = floorf( t/60.0f );
        s = floorf( t-60.0f*(float)m );

        sprintf( str, "%d fps, %02d:%02d", fps, m, s );

        SetWindowTextA( info->hWnd, str );
        }
}


int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
//void entrypoint(void)
{
    MSG         msg;
    int         done=0;
    WININFO     *info = &wininfo;

	window_init(info);

	long st = GetTickCount();
	
	CLogger* myLogger;
	myLogger = new CLogger();

	GraphicsClass* m_Graphics;
	m_Graphics = new GraphicsClass();

	m_Graphics->Initialize(XRES,YRES,info->hWnd);

	// BASS
	BASS_Init(-1,44100,0,0,NULL);
	//char filename[400]="C:\\My_Projects\\frmwrk\\Debug\\lemonade-merry_xmas_nectas.xm";
	char filename[400]="C:\\My_Projects\\frmwrk\\Debug\\bass_system-questa_sera.xm";
	//char * filename= "dark_rat-xmas.xm";
	DWORD chan;
	float time;
	//chan=BASS_MusicLoad(FALSE,"dark_rat-xmas.xm",0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,0);
	if (!(chan=BASS_MusicLoad(FALSE,filename,0,0,BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,0)))
	{
		int k=5;
	}

	float dummy;
	int a;
	for (a=0;BASS_ChannelGetAttribute(chan,BASS_ATTRIB_MUSIC_VOL_CHAN+a,&dummy);a++);
	QWORD pos;
	pos=BASS_ChannelGetLength(chan,BASS_POS_BYTE);

	BASS_ChannelPlay(chan,FALSE);

	// programm loop
	long to=0;
	while( !done )
    {
		pos=BASS_ChannelGetPosition(chan,BASS_POS_BYTE);
		time=BASS_ChannelBytes2Seconds(chan,pos);
	
		myLogger->LogThis("pos");
		myLogger->LogThisFloat((float)pos);
		myLogger->LogThis("time");
		myLogger->LogThisFloat((float)time);

		if (time!=0)
		{
			int k=6;
		}
		long t = GetTickCount();
        if( !to ) to=t; 
        t-=to;

		while( PeekMessage(&msg,0,0,0,PM_REMOVE) )
        {
            if( msg.message==WM_QUIT ) done=1;
		    TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        DrawTime( info, .001f*t );
		m_Graphics->Frame();
		m_Graphics->Render();
	}

	window_end(info);
	
    ExitProcess( 0 );
}
