#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

HGLRC		hRenderingContext = NULL;
HDC			hDeviceContext = NULL;
HWND		hWindowHandle = NULL;
HINSTANCE	hAppInstance = NULL;

bool keys[256];
bool active = true;
bool fullScreen = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ResizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   

	glClearDepth(1.0f);                         // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return true;
}

bool DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	return true;
}

GLvoid KillGLWindow(GLvoid)
{
	if (fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if (hRenderingContext)
	{
		if (!wglMakeCurrent(NULL, NULL))
			MessageBox(NULL, "Release of Device Contexts failed", "Shutdown Error", MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(hRenderingContext))
			 MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		
		hRenderingContext = NULL;
	}

	if (hDeviceContext && !ReleaseDC(hWindowHandle, hDeviceContext))
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDeviceContext = NULL;                          
	}

	if (hWindowHandle && !DestroyWindow(hWindowHandle))
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWindowHandle = NULL;
	}

	if (!UnregisterClass("OpenGL", hAppInstance))               // Are We Able To Unregister Class
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hAppInstance = NULL;
    }
}

bool CreateGLWindow(char *title, int width, int height, int bits, bool fullScreenFlag)
{
	GLuint pixelFormat;
	WNDCLASS wc;
	DWORD exStyle;
	DWORD style;

	RECT windowRect;
	windowRect.left = (long)0;
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	fullScreen = fullScreenFlag;

	hAppInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hAppInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL - CS 455";

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "Failed to register the window class.", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (fullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL, "The requested fullscreen mode isn't supported by your hardware, use windowed mode instead?",
						   "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				fullScreen = false;
			else
				return false;
		}
	}

	if (fullScreen)
	{
		exStyle = WS_EX_APPWINDOW;
		style = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

	if (!(hWindowHandle = CreateWindowEx(exStyle, "OpenGL - CS 455", title, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
										 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
										 NULL, NULL, hAppInstance, NULL)))
	{
		KillGLWindow();
		MessageBox(NULL, "Window Creation Error.", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,
		bits, 0, 0, 0, 0, 0, 0,
		0, 0, 0,
		0, 0, 0, 0,
		16, 0, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	if (!(hDeviceContext = GetDC(hWindowHandle)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't create a GL Device context", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!(pixelFormat = ChoosePixelFormat(hDeviceContext, &pfd)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't find a suitable pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!SetPixelFormat(hDeviceContext, pixelFormat, &pfd))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't set the pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!(hRenderingContext = wglCreateContext(hDeviceContext)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't create a GL Rendering Context", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!wglMakeCurrent(hDeviceContext, hRenderingContext))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't activate the GL Rendering Context", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	ShowWindow(hWindowHandle, SW_SHOW);
	SetForegroundWindow(hWindowHandle);
	SetFocus(hWindowHandle);
	ResizeGLScene(width, height);

	if (!InitGL())
	{
		KillGLWindow();
		MessageBox(NULL, "Initialization Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))
				active = true;
			else
				active = false;

			return 0;
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}

			break;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_KEYDOWN:
		{
			keys[wParam] = true;
			return 0;
		}

		case WM_KEYUP:
		{
			keys[wParam] = false;
			return 0;
		}

		case WM_SIZE:
		{
			ResizeGLScene(LOWORD(lParam), HIWORD(wParam));
			return 0;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	bool done = false;

	if (MessageBox(NULL, "Would you like to run in fullscreen mode?", "Start Fullscreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
		fullScreen = false;

	if (!CreateGLWindow("OpenGL - CS 455", 640, 480, 16, fullScreen))
		return 0;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (active)
			{
				if (keys[VK_ESCAPE])
					done = true;
				else
				{
					DrawGLScene();
					SwapBuffers(hDeviceContext);
				}
			}

			if (keys[VK_F1])
			{
				keys[VK_F1] = false;
				KillGLWindow();
				fullScreen = !fullScreen;
				if (!CreateGLWindow("OpenGL - CS 455", 640, 480, 16, fullScreen))
					return 0;
			}
		}
	}

	KillGLWindow();
	
	return msg.wParam;
}

