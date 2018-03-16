#include "common_header.h"

#include "win_OpenGLApp.h"

#if USE_FREEGLUT

#define FPS_CAP 120

int iFPSCountGLUT, iCurrentFPSGLUT;
clock_t tLastSecondGLUT;

float fFrameIntervalGLUT;
clock_t tLastFrameGLUT;

#endif

COpenGLWinApp appMain;

char Keys::kp[256] = {0};

// Return true if Key is pressed.
// iKey - virtual Key code
int Keys::Key(int iKey)
{
	return (GetAsyncKeyState(iKey)>>15)&1;
}

// Return true if Key was pressed, but only once(the Key must be released).
// iKey - virtual Key code
int Keys::Onekey(int iKey)
{
	if(Key(iKey) && !kp[iKey]){kp[iKey] = 1; return 1;}
	if(!Key(iKey))kp[iKey] = 0;
	return 0;
}

// Resets application timer (for example after re-activation of application).
void COpenGLWinApp::ResetTimer()
{
	tLastFrame = clock();
	fFrameInterval = 0.0f;
}


// Updates application timer.
void COpenGLWinApp::UpdateTimer()
{
	clock_t tCur = clock();
	fFrameInterval = float(tCur-tLastFrame)/float(CLOCKS_PER_SEC);
	tLastFrame = tCur;
}

// Stands for speed optimized float.
float COpenGLWinApp::sof(float fVal)
{
#if USE_FREEGLUT
	return fVal*fFrameIntervalGLUT;
#else
	return fVal*fFrameInterval;
#endif
	
}

// Initializes app with specified (unique) application identifier.
bool COpenGLWinApp::InitializeApp(string a_sAppName)
{
	sAppName = a_sAppName;
	hMutex = CreateMutex(NULL, 1, sAppName.c_str());
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "This application already runs!", "Multiple Instances Found.", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
	return 1;
}

// Registers application window class.
// a_hInstance - application instance handle
LRESULT CALLBACK GlobalMessageHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	return appMain.msgHandlerMain(hWnd, uiMsg, wParam, lParam);
}

void COpenGLWinApp::RegisterAppClass(HINSTANCE a_hInstance)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hInstance = a_hInstance;
	hInstance = a_hInstance;

	wcex.lpfnWndProc = GlobalMessageHandler;
	wcex.lpszClassName = sAppName.c_str();

	wcex.lpszMenuName = NULL;

	RegisterClassEx(&wcex);
}

// Creates main application window.
// sTitle - title of created window
bool COpenGLWinApp::CreateAppWindow(string sTitle)
{
	if(MessageBox(NULL, "Would you like to run in fullscreen?", "Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		DEVMODE dmSettings = {0};
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings); // Get current display settings

		hWnd = CreateWindowEx(0, sAppName.c_str(), sTitle.c_str(), WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // This is the commonly used style for fullscreen
		0, 0, dmSettings.dmPelsWidth, dmSettings.dmPelsHeight, NULL,
		NULL, hInstance, NULL);
	}
	else hWnd = CreateWindowEx(0, sAppName.c_str(), sTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, hInstance, NULL);

	if(!oglControl.InitOpenGL(hInstance, &hWnd, InitScene, RenderScene, ReleaseScene, &oglControl))return false;

	ShowWindow(hWnd, SW_SHOW);

	// Just to send WM_SIZE message again
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);

	ShowCursor(FALSE);

	return true;
}

// Main application body infinite loop.
void COpenGLWinApp::AppBody()
{
	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)break; // If the message was WM_QUIT then exit application
			else
			{
				TranslateMessage(&msg); // Otherwise send message to appropriate window
				DispatchMessage(&msg);
			}
		}
		else if(bAppActive)
		{
			UpdateTimer();
			oglControl.Render(&oglControl);
		}
		else Sleep(200); // Do not consume processor power if application isn't active
	}
}

// Shuts down application and releases used memory.
void COpenGLWinApp::Shutdown()
{
	oglControl.ReleaseOpenGLControl(&oglControl);

	DestroyWindow(hWnd);
	UnregisterClass(sAppName.c_str(), hInstance);
	COpenGLControl::UnregisterSimpleOpenGLClass(hInstance);
	ReleaseMutex(hMutex);
}

// Application messages handler.
LRESULT CALLBACK COpenGLWinApp::msgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch(uiMsg)
	{
		case WM_PAINT:
			BeginPaint(hWnd, &ps);					
			EndPaint(hWnd, &ps);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_ACTIVATE:
		{
			switch(LOWORD(wParam))
			{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					bAppActive = true;
					ResetTimer();
					break;
				case WA_INACTIVE:
					bAppActive = false;
					break;
			}
			break;
		}

		case WM_SIZE:
			oglControl.ResizeOpenGLViewportFull();
			oglControl.SetProjection3D(45.0f, float(LOWORD(lParam))/float(HIWORD(lParam)), 0.5f, 1000.0f);
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}

// Returns application instance.
HINSTANCE COpenGLWinApp::GetInstance()
{
	return hInstance;
}

#if USE_FREEGLUT

void ResetTimerGLUT()
{
	tLastFrameGLUT = clock();
	fFrameIntervalGLUT = 0.0f;
}

void UpdateTimerGLUT()
{
	clock_t tCur = clock();
	fFrameIntervalGLUT = float(tCur - tLastFrameGLUT) / float(CLOCKS_PER_SEC);
	tLastFrameGLUT = tCur;
}

void UpdateFunctionGLUT()
{
	
	clock_t tCurrent = clock();
	if ((tCurrent - tLastSecondGLUT) >= CLOCKS_PER_SEC)
	{
		tLastSecondGLUT += CLOCKS_PER_SEC;
		iFPSCountGLUT = iCurrentFPSGLUT;
		iCurrentFPSGLUT = 0;
	}

	UpdateTimerGLUT();
	glutPostRedisplay();
	
	iCurrentFPSGLUT++;
}


int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size	
	glutInitWindowSize(800, 600);

	// Create the window with the title "Hello,GL"
	glutCreateWindow("Seminar 8-9-10. OpenGL - Sergey Pavlov");

	// load glew before we load objects
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	if (MessageBox(NULL, "Would you like to run in fullscreen?", "Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES)
		glutFullScreenToggle();

	//glutTimerFunc();

	InitSceneGLUT(); // create objects & load shaders

	ResetTimerGLUT();

	// Bind rendering function	
	glutDisplayFunc(RenderSceneGLUT);

	// Bind main loop updater
	glutIdleFunc(UpdateFunctionGLUT);

	glutMainLoop();
	return 0;
}

#else

// Application entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
	if (!appMain.InitializeApp("Sergey Pavlov. OGL 3"))
		return 0;
	appMain.RegisterAppClass(hInstance);

	if (!appMain.CreateAppWindow("Sergey Pavlov. OGL 3"))
		return 0;
	appMain.ResetTimer();

	appMain.AppBody();
	appMain.Shutdown();

	return 0;
}
#endif

/*
Author: Sergey Pavlov 
Group: BBI 141
Tool: Visual Studio 2015
Cross-platform window manager: FreeGLUT

OGL 2 project
What has been done: 
	- Photo box with 2 textures changing each other over time.
	- A pillar with a swing hanging on rope
	- Imported various textures (new ground, metall, rope and tire textures)
	- A box imitating house
	- A new mountain skybox 

OGL 3 project
What has been done:
	- 2 flash & 2 point lights + sun (directional light)
	- Flash lights are street lights
	- Point lights are small garden lamps
	- you can turn on/off street lights by pressing 'L' and garden lamps by pressing 'P'

OGL 4 project
What has been done:
	- 3 fog types + no-fog mode. Press 'F' to switch. '-' and '+' to switch intensity and 'pgup' and 'pgdn' to control distance for linear.
	- Assimp library connected
	- Imported teapot (new), chest and house
	- There are 2 teapots: the first is opaque and another is shiny
	- Used new shader to count reflection
	control Specular intensity: 'Q' and 'E'
			Specular power:		'Z' and 'C'


Skybox taken from: https://opengameart.org/content/mountain-skyboxes
More info on skybox:
	Author
	======
	This is the work of Emil Persson, aka Humus.
	http://www.humus.name

	License
	=======
	This work is licensed under a Creative Commons Attribution 3.0 Unported License.
	http://creativecommons.org/licenses/by/3.0/


*/