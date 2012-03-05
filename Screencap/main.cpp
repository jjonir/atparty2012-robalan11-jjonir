#include <windows.h>
#include <time.h>
#include <stdio.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = (LPCWSTR)szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           (LPCWSTR)szClassName,         /* Classname */
           (LPCWSTR)"Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           192,                 /* The programs width */
           200,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Copy part of the screen to a window */
    
    HDC screen_dc = GetDC(NULL);
    HDC win_dc = GetDC(hwnd);
    HDC bmp_dc = CreateCompatibleDC(win_dc);
    
    int wid = GetDeviceCaps(screen_dc, HORZRES);
    int hei = GetDeviceCaps(screen_dc, VERTRES);
    
    HBITMAP bmp = CreateCompatibleBitmap(win_dc, wid, hei);
    SelectObject(bmp_dc, bmp);
    
	// Get screen
    BitBlt(bmp_dc, 0, 0, wid, hei, screen_dc, 0, 0, SRCCOPY);

    //ShowWindow(hwnd, nFunsterStil);
    SetActiveWindow(hwnd);
    
	// Initialize timers
    clock_t program_timer = 0;
	clock_t blink_timer = 0;
	bool invert = true;

    // Run the main loop.
    while (true)
    {
		// Get start time of frame
		clock_t framestart = clock();

		// At 4 seconds, kill it
		if (program_timer >= 4*CLOCKS_PER_SEC) break;

		// Invert once per second
		if (blink_timer >= CLOCKS_PER_SEC) {
			if (invert)
				BitBlt(screen_dc, 0, 0, wid, hei, bmp_dc, 0, 0, SRCCOPY);
			else
				BitBlt(screen_dc, 0, 0, wid, hei, bmp_dc, 0, 0, NOTSRCCOPY);
			invert = !invert;
			blink_timer = 0;
		}

		// See how long frame was, add to program timer - Also make sure frame was at least 1/60 second
		clock_t frameend = clock();
		clock_t frame_length = frameend - framestart;
		if (frame_length < CLOCKS_PER_SEC/60) {
			Sleep(CLOCKS_PER_SEC/60 - frame_length);
			frameend = clock();
			frame_length = frameend - framestart;
		}
		program_timer += frame_length;
		blink_timer += frame_length;
    }
    
    // Restore old screen
    BitBlt(screen_dc, 0, 0, wid, hei, bmp_dc, 0, 0, SRCCOPY);
    
	PostQuitMessage (0);

	while (GetMessage (&messages, NULL, 0, 0)) {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
	}

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE)
                PostQuitMessage(0);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
