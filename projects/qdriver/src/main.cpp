
#define NOMINMAX

#include <windows.h>

#include "qmath.h"
#include "qcamera.h"
#include "cINI.h"
#include "qmath.h"
#include "qrender.h"
#include "debug.h"
#include "app.h"
#include "playpen.h"


CApplication* g_pApp = NULL;
CTimer* mainTimer;

////////////////////////////////////////////////////////////////////////////////////
// WinMain 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	float totalFrameTime = 0.0f;
	mainTimer = new CTimer;

	// Mem dump shit
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);

	// Initialization of MFC for nVidia PerfHUD (cheap workaround for perfHUD bug)
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// Load settings from config.ini //
	// Using a NULL callback (2nd param) uses a default system defined callback which only handles exits //
	g_pApp = new CApplication( "Media/config.ini", PlaypenEventCallback );

	// preset game loop //
	MSG msg;

	//HACK: PlayInit() //
	PlayInit();

	// enter game loop
	bool done = false;
	while(!done)
	{
		mainTimer->Start();

		// use PeekMessage so we dont get stuck waiting for one
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				done = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if(keys[VK_ESCAPE])
			done = true;

		else if(!done) 
		{
			// Do updates //
			PlayUpdate();

			g_pRender->BeginRendering();

			g_pRender->ClearBuffers(QRENDER_CLEAR_BACKBUFFER | QRENDER_CLEAR_ZBUFFER,  QRENDER_MAKE_ARGB(0, 0, 0, 0), 1.0f);


			PlayRender(totalFrameTime);


			g_pRender->EndRendering();


			// Main loop logic goes here //
		}

		else
		{
			PostQuitMessage(0);
			done = true;
		}

		totalFrameTime = mainTimer->GetElapsedMilliSec();
	}

	PlayDestroy();
	QMem_SafeDelete(g_pApp);

	// Dump the leaks
	//_CrtDumpMemoryLeaks();
	//CloseHandle(hLogFile);

	//exit(0);
	return((int)msg.wParam);
}
