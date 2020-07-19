#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>
#include <iostream>
using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
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
               _T("Task One G3,4 20170411_20170419"),       /* Title Text */
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
void draw8Pixels(HDC hdc, int xc,int yc,int x, int y, COLORREF rgb)
{
    SetPixel(hdc, (x)+xc, y+yc, rgb);
    SetPixel(hdc, (x)+xc, -y+yc, rgb);
    SetPixel(hdc, (-y)+xc, x+yc, rgb);
    SetPixel(hdc, (-y)+xc, -x+yc, rgb);
    SetPixel(hdc, (-x)+xc, -y+yc, rgb);
    SetPixel(hdc, (-x)+xc, y+yc, rgb);
    SetPixel(hdc, (y)+xc, -x+yc, rgb);
    SetPixel(hdc, (y)+xc, x+yc, rgb);
}
int Round(double x)
{
    return (int)(x + 0.5);
}

void drawLineDDA(HDC hdc, int xa, int ya, int xb, int yb)
{
    int dx = xb - xa, dy = yb - ya, steps, k;
    float xIncrement, yIncrement, x = xa, y = ya;
    if(abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float) steps;
    yIncrement = dy / (float) steps;

    SetPixel(hdc,Round(x),Round(y),RGB(0,0,255));

    for(int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        SetPixel(hdc,x,y,RGB(0,0,255));
    }
}
void Draw8Lines(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
    drawLineDDA(hdc, xc, yc, xc+a,yc+b);
    drawLineDDA(hdc, xc, yc, xc-a,yc-b);
}

void circleMidPoint(HDC hdc, int xc, int yc, int r, COLORREF rgb , boolean line)
{
    int d = 1 - r ;
    int x = 0 ;
    int y =  r;
    draw8Pixels(hdc,xc,yc,0,r,rgb);
    while(x<y)
    {
        if (d>0)
        {
            y--;
            d+=2*(x - y ) + 5;
        }
        else
        {
            d+= 2 * x + 3;
        }
        x++;
        draw8Pixels(hdc,xc,yc,x,y,rgb);
        if(line){
            Draw8Lines(hdc,xc,yc,x,y,rgb);
        }
    }
}



COLORREF rgb = RGB(0,0,0);
int X1,Y1,X2,R1,R2,Y2,X3,Y3,count = 0;
bool f1 = false, f2 =false;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)                  /* handle the messages */
    {

    case WM_LBUTTONDOWN:
    {
        if(count == 0)
        {
            X1 = LOWORD(lParam);
            Y1 = HIWORD(lParam);
            count++;
        }
        else if(count == 1)
        {
            X2 = LOWORD(lParam);
            Y2 = HIWORD(lParam);
            f1 = true;
            count++;
        }
        break;
    }
    case WM_RBUTTONDOWN:

        f2 = true;
        Y3 = HIWORD(lParam);
        X3 = LOWORD(lParam) ;
        R1 = max(abs(X3 - X1),abs(Y3-Y1));
        R2 = max(abs(X3 - X2),abs(Y3-Y2));
        if (f1==true &&f2==true)
        {
            HDC hdc = GetDC(hwnd);
            int a = rand()*10;
            int b = rand()*10;
            int c = rand()*10;
            circleMidPoint(hdc,X1,Y1,R1,rgb,true);
            circleMidPoint(hdc,X2,Y2,R2,rgb,false);
            ReleaseDC(hwnd,hdc);
            count = 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

