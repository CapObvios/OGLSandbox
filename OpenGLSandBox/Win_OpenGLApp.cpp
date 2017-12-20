//#include "common_header.h"
//
//#include "win_OpenGLApp.h"
//
//COpenGLWinApp appMain;
//
//char Keys::kp[256] = {0};
//
//// Return true if key is pressed.
//// iKey - virtual key code
//int Keys::Key(int iKey)
//{
//	return (GetAsyncKeyState(iKey)>>15)&1;
//}
//
//// Return true if key was pressed, but only once(the key must be released).
//// iKey - virtual key code
//int Keys::Onekey(int iKey)
//{
//	if(Key(iKey) && !kp[iKey]){kp[iKey] = 1; return 1;}
//	if(!Key(iKey))kp[iKey] = 0;
//	return 0;
//}
//
//// Resets application timer (for example after re-activation of application).
//void COpenGLWinApp::ResetTimer()
//{
//	dwLastFrame = GetTickCount();
//	fFrameInterval = 0.0f;
//}
//
//// Updates application timer.
//void COpenGLWinApp::UpdateTimer()
//{
//	DWORD dwCur = GetTickCount();
//	fFrameInterval = float(dwCur-dwLastFrame)*0.001f;
//	dwLastFrame = dwCur;
//}
//
//// Stands for speed optimized float.
//float COpenGLWinApp::sof(float fVal)
//{
//	return fVal*fFrameInterval;
//}
//
//// Initializes app with specified (unique) application identifier.
//bool COpenGLWinApp::InitializeApp(string a_sAppName)
//{
//	sAppName = a_sAppName;
//	hMutex = CreateMutex(NULL, 1, sAppName.c_str());
//	if(GetLastError() == ERROR_ALREADY_EXISTS)
//	{
//		MessageBox(NULL, "This application already runs!", "Multiple Instances Found.", MB_ICONINFORMATION | MB_OK);
//		return 0;
//	}
//	return 1;
//}
//
//// Registers application window class.
//// a_hInstance - application instance handle
//LRESULT CALLBACK GlobalMessageHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
//{
//	return appMain.MsgHandlerMain(hWnd, uiMsg, wParam, lParam);
//}
//
//void COpenGLWinApp::RegisterAppClass(HINSTANCE a_hInstance)
//{
//	WNDCLASSEX wcex;
//	memset(&wcex, 0, sizeof(WNDCLASSEX));
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.style = CS_OWNDC;
//
//	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//
//	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
//	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
//	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
//	wcex.hInstance = hInstance;
//
//	wcex.lpfnWndProc = GlobalMessageHandler;
//	wcex.lpszClassName = sAppName.c_str();
//
//	wcex.lpszMenuName = NULL;
//
//	RegisterClassEx(&wcex);
//}
//
//// Creates main application window.
//bool COpenGLWinApp::CreateAppWindow(string sTitle)
//{
//	hWnd = CreateWindowEx(0, sAppName.c_str(), sTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
//		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
//		NULL, hInstance, NULL);
//
//	if(!oglControl.InitOpenGL(hInstance, &hWnd, InitScene, RenderScene, NULL, &oglControl))return false;
//
//	ShowWindow(hWnd, SW_SHOW);
//
//	// Just to send WM_SIZE message again
//	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
//	UpdateWindow(hWnd);
//
//	return true;
//}
//
//// Main application body infinite loop.
//void COpenGLWinApp::AppBody()
//{
//	MSG msg;
//	while(1)
//	{
//		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			if(msg.message == WM_QUIT)break; // If the message was WM_QUIT then exit application
//			else
//			{
//				TranslateMessage(&msg); // Otherwise send message to appropriate window
//				DispatchMessage(&msg);
//			}
//		}
//		else if(bAppActive)
//		{
//			UpdateTimer();
//			oglControl.Render(&oglControl);
//		}
//		else Sleep(200); // Do not consume processor power if application isn't active
//	}
//}
//
//// Shuts down application and releases used memory.
//void COpenGLWinApp::Shutdown()
//{
//	oglControl.ReleaseOpenGLControl(&oglControl);
//
//	DestroyWindow(hWnd);
//	UnregisterClass(sAppName.c_str(), hInstance);
//	COpenGLControl::UnregisterSimpleOpenGLClass(hInstance);
//	ReleaseMutex(hMutex);
//}
//
//// Application messages handler.
//LRESULT CALLBACK COpenGLWinApp::MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
//{
//	PAINTSTRUCT ps;
//
//	switch(uiMsg)
//	{
//		case WM_PAINT:
//			BeginPaint(hWnd, &ps);					
//			EndPaint(hWnd, &ps);
//			break;
//
//		case WM_CLOSE:
//			PostQuitMessage(0);
//			break;
//
//		case WM_ACTIVATE:
//		{
//			switch(LOWORD(wParam))
//			{
//				case WA_ACTIVE:
//				case WA_CLICKACTIVE:
//					bAppActive = true;
//					break;
//				case WA_INACTIVE:
//					bAppActive = false;
//					break;
//			}
//			break;
//		}
//
//		case WM_SIZE:
//			oglControl.ResizeOpenGLViewportFull();
//			break;
//
//		default:
//			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
//	}
//	return 0;
//}
//
//// Returns application instance.
//HINSTANCE COpenGLWinApp::GetInstance()
//{
//	return hInstance;
//}
//
////Windows entry point for application.
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
//{
//	if(!appMain.InitializeApp("Семинар 7"))
//		return 0;
//	appMain.RegisterAppClass(hInstance);
//
//	if(!appMain.CreateAppWindow("OpenGL. Начало работы"))
//		return 0;
//	appMain.ResetTimer();
//
//	appMain.AppBody();
//	appMain.Shutdown();
//
//	return 0;
//}







#include <Windows.h>
#include <GL\glew.h>
#include <freeglut.h>
//#include <GL\freeglut.h>
#include <iostream>
#include "Shaders.h"
#include "common_header.h"
//#include "OpenGLControl.h"
//#include "win_OpenGLApp.h"

#define KOSTYL true

#if KOSTYL
float fSea[12];
float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
float fBoat[12];
float fSun[24];
float fSeaColor[12];
float fTriangleColor[9];
float fQuadColor[12];
float fBoatColor[12];
float fSunColor[24];

UINT uiVBO[10];
UINT uiVAO[5];

CShader shVertex, shFragment;
CShaderProgram spMain;

// Initializes OpenGL features that will be used.
// lpParam - Pointer to anything you want.
void init()
{
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f);

	// setup sea
	fSea[0] = 1.0f; fSea[1] = -0.75f; fSea[2] = 0.0f;
	fSea[3] = 1.0f; fSea[4] = -1.0f; fSea[5] = 0.0f;
	fSea[6] = -1.0f; fSea[7] = -0.75f; fSea[8] = 0.0f;
	fSea[9] = -1.0f; fSea[10] = -1.0f; fSea[11] = 0.0f;

	// setup sea color
	fSeaColor[0] = 0.0f; fSeaColor[1] = 0.65f; fSeaColor[2] = 0.8f;
	fSeaColor[3] = 0.0f; fSeaColor[4] = 0.65f; fSeaColor[8] = 0.8f;
	fSeaColor[6] = 0.0f; fSeaColor[7] = 0.65f; fSeaColor[5] = 0.8f;
	fSeaColor[9] = 0.0f; fSeaColor[10] = 0.65f; fSeaColor[11] = 0.8f;

	// Setup triangle vertices
	fTriangle[0] = -0.005f; fTriangle[1] = -0.15f; fTriangle[2] = 0.0f;
	fTriangle[3] = -0.005f; fTriangle[4] = -0.6f; fTriangle[5] = 0.0f;
	fTriangle[6] = -0.2f; fTriangle[7] = -0.6f; fTriangle[8] = 0.0f;

	// Setup triangle color

	fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
	fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
	fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;

	// Setup quad vertices

	fQuad[0] = -0.005f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
	fQuad[3] = -0.005f; fQuad[4] = -0.7f; fQuad[5] = 0.0f;
	fQuad[6] = 0.005f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
	fQuad[9] = 0.005f; fQuad[10] = -0.7f; fQuad[11] = 0.0f;

	// Setup quad color

	fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
	fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
	fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
	fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;

	// Setup boat vertices

	fBoat[0] = -0.2f; fBoat[1] = -0.8f; fBoat[2] = 0.0f;
	fBoat[3] = -0.3f; fBoat[4] = -0.7f; fBoat[5] = 0.0f;
	fBoat[6] = 0.2f; fBoat[7] = -0.8f; fBoat[8] = 0.0f;
	fBoat[9] = 0.3f; fBoat[10] = -0.7f; fBoat[11] = 0.0f;

	// Setup boat color

	fBoatColor[0] = 1.0f; fBoatColor[1] = 0.0f; fBoatColor[2] = 0.0f;
	fBoatColor[3] = 0.0f; fBoatColor[4] = 1.0f; fBoatColor[8] = 0.0f;
	fBoatColor[6] = 0.0f; fBoatColor[7] = 0.0f; fBoatColor[5] = 1.0f;
	fBoatColor[9] = 1.0f; fBoatColor[10] = 1.0f; fBoatColor[11] = 0.0f;

	// Setup sun vertices center = 0.5, 0.5
	fSun[0] = 0.54f; fSun[1] = 0.5f; fSun[2] = 0.0f;
	fSun[3] = 0.53f; fSun[4] = 0.53f; fSun[5] = 0.0f;
	fSun[6] = 0.5f; fSun[7] = 0.54f; fSun[8] = 0.0f;
	fSun[9] = 0.47f; fSun[10] = 0.53f; fSun[11] = 0.0f;
	fSun[12] = 0.46f; fSun[13] = 0.5f; fSun[14] = 0.0f;
	fSun[15] = 0.47f; fSun[16] = 0.47f; fSun[17] = 0.0f;
	fSun[18] = 0.5f; fSun[19] = 0.46f; fSun[20] = 0.0f;
	fSun[21] = 0.53f; fSun[22] = 0.47f; fSun[23] = 0.0f;

	// Setup sun color
	fSunColor[0] = 1.0;   fSunColor[1] = 0.0f; fSunColor[2] = 0.0f;
	fSunColor[3] = 1.0f;  fSunColor[4] = 0.0f; fSunColor[5] = 0.0f;
	fSunColor[6] = 1.0f;  fSunColor[7] = 0.0f; fSunColor[8] = 0.0f;
	fSunColor[9] = 1.0f;  fSunColor[10] = 0.0f; fSunColor[11] = 0.0f;
	fSunColor[12] = 1.0f; fSunColor[13] = 0.0f; fSunColor[14] = 0.0f;
	fSunColor[15] = 1.0f; fSunColor[16] = 0.0f; fSunColor[17] = 0.0f;
	fSunColor[18] = 1.0f; fSunColor[19] = 0.0f; fSunColor[20] = 0.0f;
	fSunColor[21] = 1.0f; fSunColor[22] = 0.0f; fSunColor[23] = 0.0f;

	glGenVertexArrays(5, uiVAO);
	glGenBuffers(10, uiVBO);


	// Setup whole sea
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fSea, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fSeaColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole triangle
	glBindVertexArray(uiVAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), fTriangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), fTriangleColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole quad
	glBindVertexArray(uiVAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[5]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fQuadColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole boat
	glBindVertexArray(uiVAO[3]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[6]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fBoat, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[7]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), fBoatColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup sun
	glBindVertexArray(uiVAO[4]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[8]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), fSun, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[9]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), fSunColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Load shaders and create shader program

	shVertex.LoadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER);

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shVertex);
	spMain.AddShaderToProgram(&shFragment);

	spMain.LinkProgram();
	spMain.UseProgram();
}

// Renders whole scene.
// lpParam - Pointer to anything you want.
void render()
{
	// Typecast lpParam to COpenGLControl pointer
	//COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	// We just clear color
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(uiVAO[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(uiVAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(uiVAO[2]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(uiVAO[3]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(uiVAO[4]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	//oglControl->SwapBuffersM();

	glutSwapBuffers();
}

// Releases OpenGL scene.
// lpParam - Pointer to anything you want.
void ReleaseScene(LPVOID lpParam)
{
	spMain.DeleteProgram();

	shVertex.DeleteShader();
	shFragment.DeleteShader();
}

#endif // KOSTYL

using namespace std;

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

//void render()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glutSwapBuffers();
//}


// GLEW 2.1.0
// OGL 4.5
// freeGlut 3.0.0

int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 600);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("Seminar 7. OpenGL start - Sergey Pavlov");
		
	// load glew before we load objects
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	init(); // create objects & load shaders

	// Bind rendering function	
	glutDisplayFunc(render); 

	


	glutMainLoop();
	return 0;
}