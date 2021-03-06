#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include"unittest.h"
#include"engine.h"

engine_t en;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    //--- Start debug
    unittest();
    //--- Finish debug


    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
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
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("FreePanorama"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

   	SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    //--- Start Get filename
    bool no_argument=false;
    if(lpszArgument==0)no_argument=true;
    else if(lpszArgument[0]==0)no_argument=true;
    if(no_argument){
        OPENFILENAME ofn;
        char szFileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = "Panorama Files (*.pan)\0*.pan\0";
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT;
        ofn.lpstrDefExt = "pan";

        if(GetOpenFileName(&ofn)){
            en.pf.load(szFileName);
        }else return 0;
    }else{
        char *CL = GetCommandLineA();
        char filename[1024];
        //MessageBox(NULL, CL, "Test", MB_OK);
        //--- Parse
        int uk=0;
        if(CL[0]=='"'){
            uk=1;
            while(1){
                if(CL[uk]==0)return 0;//ERROR
                if(CL[uk]=='"')break;
                uk++;
            }
            uk++;
        }else{
            while(CL[uk]!=' ')uk++;
        }
        while(CL[uk]==' ')uk++;

        int fnuk=0;
        if(CL[uk]=='"'){
            uk++;
            while(1){
                if(CL[uk]=='"'){
                    filename[fnuk]=0;
                    break;
                }
                filename[fnuk]=CL[uk];
                fnuk++;uk++;
            }

        }else{
            while(1){
                if((CL[uk]==' ')||(CL[uk]==0)){
                    filename[fnuk]=0;
                    break;
                }
                filename[fnuk]=CL[uk];
                fnuk++;uk++;
            }
        }
        if(fnuk){
            en.pf.load(filename);
        }
        //en.pf.load("L439.pan");
    }
    //--- Finish Get Filename

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
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
	RECT myrect;//Size of window
	GetClientRect(hwnd,&myrect);
	int maxx=myrect.right-myrect.left;
	int maxy=myrect.bottom-myrect.top;
	POINT a;
	GetCursorPos(&a);
	ScreenToClient(hwnd,&a);
	int msx=a.x,msy=a.y;
	bool inside=((msx>=0)&&(msy>=0)&&(msx<maxx)&&(msy<maxy));


    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
		case WM_PAINT:
			en.on_draw(&hwnd);
			break;
        case WM_MOUSEWHEEL:
            en.on_mouse_wheel(GET_WHEEL_DELTA_WPARAM(wParam));//WHEEL_DELTA
            break;
        case WM_SIZE:
            en.need_redraw = true;
            break;
        case WM_LBUTTONDOWN:
            en.on_mouse_button(msx,maxy-1-msy);
            break;
        case WM_LBUTTONUP:
            en.on_mouse_move(msx,maxy-1-msy);
            break;
        case WM_MOUSEMOVE:
            if(wParam&MK_LBUTTON)
                en.on_mouse_move(msx,maxy-1-msy);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    //--- If need redraw - redraw
    if(en.need_redraw)
        InvalidateRect(hwnd,NULL,false);
    en.need_redraw=false;

    return 0;
}
