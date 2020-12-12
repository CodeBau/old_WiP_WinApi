#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#define ID_BUTTON_WRITE 101
#define ID_BUTTON_CLOSE 102


#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cassert>

#include "../include/winapifn.h"
#include "../resource.h"

struct MyColor {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    void set(uint8_t r_ = 0, uint8_t g_ = 0, uint8_t b_ = 0)
    {
        r = r_;
        g = g_;
        b = b_;
        a = 0;

    }
};

class MyBitmap {
public:

    ~MyBitmap() {
        destroy();
    }
    void create(int width, int height)
    {
        std::vector<MyColor> pixels;
        pixels.resize(width * height);
       

        for (int y = 0; y < height; y++) 
        {
            auto* p = &pixels[y * width];
            for (int x = 0; x < width; x++)
            {
                p->set(x, y, 0);
                p++;
            }
        }

        m_bmp = CreateBitmap(width, height, 1, 32, pixels.data());
        if (!m_bmp) 
        {
            assert(false);
            return;
        }
        m_width = width;
        m_height = height;
            
    }

    void loadFromResource()
    {
        destroy();
        m_bmp=LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_BITMAP1));
        if (!m_bmp)
        {
            assert(false);
            return;
        }
        BITMAP info;
        GetObject(m_bmp, sizeof(info), &info);
        m_width = info.bmWidth;
        m_height = info.bmHeight;

    }

    void loadFromFile()
    {
    destroy();
    m_bmp = (HBITMAP)LoadImage(NULL, L"media/img/Logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!m_bmp)
    {
        assert(false);
        return;
    }

    BITMAP info;
    GetObject(m_bmp, sizeof(info), &info);
    m_width = info.bmWidth;
    m_height = info.bmHeight;
    }

    void destroy() 
    {
        if (m_bmp)
        {
            DeleteObject(m_bmp);
            m_bmp = nullptr;
        }
    }

    void draw(HDC hdc, int x, int y)
    {
        if (!m_bmp)
            return;
        auto src = CreateCompatibleDC(hdc);

        SelectObject(src, m_bmp);

        BitBlt(hdc, x, y, m_width, m_height, src, 0, 0, SRCCOPY);

        DeleteDC(src);

    }
private:
    HBITMAP m_bmp = nullptr;
    int m_width = 0; 
    int m_height = 0; 
};



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = L"WIT";
TCHAR szTitle[] = L"WIT - logowanie";
TCHAR textsave[] = L"";


HWND TextBox;

HBITMAP hbmObraz, m_bmp = nullptr;



int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    /*console test witch prinT and print function*/
    printT(szClassName);
    print(T2str(szClassName));

    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;
  
    /* The class is registered, let's create the program*/
    int scrx = GetSystemMetrics(SM_CXSCREEN);
    int scry = GetSystemMetrics(SM_CYSCREEN);
    int windx = 600;
    int windy = 400;
    int scrcentralx = (scrx / 2) - (windx / 2);
    int scrcentraly = (scry / 2) - (windy / 2);
  
    hwnd = CreateWindow(
 //           0,                   /* Extended possibilites for variation */
            szClassName,         /* Classname */
            szTitle,           /* Title Text */
            WS_OVERLAPPEDWINDOW,
            //WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, /* default window */
            scrcentralx,       /* Windows decides the position */
            scrcentraly,       /* where the window ends up on the screen */
            windx,                 /* The programs width */
            windy,                 /* and height in pixels */
            HWND_DESKTOP,        /* The window is a child-window to desktop */
            NULL,                /* No menu */
            hThisInstance,       /* Program Instance handler */
            NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    //print("Otwieram Okno");
   
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
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

void myTextOut(HDC hdc, int x, int y, const wchar_t* text)
{
    if (!text)
        return;
    TextOut(hdc, x, y, text, wcslen(text));
}


MyBitmap bmp;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:

        //bmp.create(256, 256);
        //bmp.loadFromResource();
        bmp.loadFromFile();
        
       
                 /*always load images first */
        

                /* CreateWindow(L"BUTTON",
                                     L"Przyc 1",
                                     WS_CHILD | WS_VISIBLE | BS_FLAT,
                                     10, 10, 100, 20,
                                     hwnd, (HMENU) 1, NULL, NULL);
                 CreateWindow(L"BUTTON",
                                     L"Przyc 2",
                                     WS_CHILD | WS_VISIBLE | BS_FLAT,
                                     10, 40, 100, 20,
                                     hwnd, (HMENU) 2, NULL, NULL);
                 TextBox = CreateWindow(L"EDIT",
                                      L"",
                                      WS_CHILD | WS_VISIBLE | WS_BORDER,
                                      10, 80, 100, 20,
                                      hwnd, NULL, NULL, NULL);
                 CreateWindow(L"BUTTON",
                                       L"GO",
                                       WS_CHILD | WS_VISIBLE | BS_FLAT,
                                       10, 110, 100, 20,
                                       hwnd, (HMENU)3, NULL, NULL);*/
                 

            /*g_hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCWSTR)L"EDIT", NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
                10, 10, 300, 100, hwnd, NULL, NULL, NULL);
            if (!g_hEdit)
                SendMessage(hwnd, WM_CLOSE, NULL, NULL);

            SetWindowText(g_hEdit, L"Wpisz tu co�...");

            g_hButW = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCWSTR)L"BUTTON", L"Zapisz tekst",
                WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 150, 150, 30,
                hwnd, (HMENU)ID_BUTTON_WRITE, NULL, NULL);
            if (!g_hButW) SendMessage(hwnd, WM_CLOSE, NULL, NULL);

            g_hButC = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCWSTR)L"BUTTON", L"Wyjd�",
                WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 150, 150, 30,
                hwnd, (HMENU)ID_BUTTON_CLOSE, NULL, NULL);
            if (!g_hButC) SendMessage(hwnd, WM_CLOSE, NULL, NULL);
            */

        break;



    case WM_PAINT:
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);


        bmp.draw(ps.hdc, 60, 200);
        
        break;

    case WM_COMMAND:

        /*switch (wParam) {
        case ID_BUTTON_WRITE:
            WriteFile(hFout, g_hEdit, NULL, NULL, NULL);
            break;
        case ID_BUTTON_CLOSE:
            DestroyWindow(hwnd);
            break;
        }
        break;*/

        switch (wParam) {
        case 1:
            ::MessageBox(hwnd, L"Przyc 1", L"", MB_OK);
            break;
        case 2:
            ::MessageBox(hwnd, L"Przyc 2", L"", MB_OK);
            break;
        case 3:
            int gwtstat = 0;
            gwtstat = GetWindowText(TextBox, textsave ,20);
            ::MessageBox(hwnd, textsave, textsave, MB_OK);
            break;
        }
        break;

    case WM_LBUTTONDOWN: {
    auto x = (int16_t)(lParam & 0xffff);
    auto y = (int16_t)(lParam >> 16 & 0xffff);
    printf("Left Button Down %d %d\n", x, y);

    auto hdc = GetDC(hwnd);
    bmp.draw(hdc, x, y);
    ReleaseDC(hwnd,hdc);

    //    //HDC hdc = GetDC(hwnd);

    //    //Rectangle(hdc, x, y, x + 5, y + 20);
    //    //TextOut(hdc, x, y, L"Hallo", 10);
    //    //myTextOut(hdc, x, y, L"Marcin");
    //    //myTextOut(hdc, x, y + 20, L"Lesniewski");
    //   
    //    //POINT pt;
    //    //MoveToEx(hdc, x, y, &pt);
    //    //LineTo(hdc, x+10, y+10);
    //    //LineDDA(50, 50, x, y,);

    //    /*for (int j = 0; j < 256; j++) {
    //        for (int i = 0; i < 256; i++) {
    //            COLORREF color = RGB(i, j, 0);
    //            SetPixel(hdc, x+i, y+j, color);
    //        }
    //    }*/
    //    
    //    /*HBRUSH brush=CreateSolidBrush(RGB(0, 100, 255));
    //    SelectObject(hdc, brush);
    //    Rectangle(hdc, x - 25, y - 25, x + 25, y + 25);
    //    DeleteObject(brush);*/

    //    
    //    /*HBRUSH brush=CreateHatchBrush(HS_CROSS, RGB(0, 100, 255));
    //    SelectObject(hdc, brush);
    //    Rectangle(hdc, x - 25, y - 25, x + 25, y + 25);
    //    DeleteObject(brush);*/
    //    
    //    /*HBRUSH brush=CreateHatchBrush(HS_CROSS, RGB(0, 100, 255));
    //    HPEN pen = CreatePen(PS_SOLID, 5, RGB(255,0,0));
    //    SelectObject(hdc, brush);
    //    SelectObject(hdc, pen);
    //    Rectangle(hdc, x - 25, y - 25, x + 25, y + 25);
    //    DeleteObject(brush);
    //    DeleteObject(pen);*/

    //   /*HBRUSH brush=CreateHatchBrush(HS_CROSS, RGB(0, 100, 255));
    //   HPEN pen = CreatePen(PS_SOLID, 5, RGB(255,0,0));
    //   SelectObject(hdc, brush);
    //   SelectObject(hdc, pen);
    //   SelectObject(hdc, GetStockObject(GRAY_BRUSH));
    //   Rectangle(hdc, x - 25, y - 25, x + 25, y + 25);
    //   DeleteObject(brush);
    //   DeleteObject(pen);*/

    //   /*SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
    //   myTextOut(hdc, x, y, L"Marcin");

    //    ReleaseDC(hwnd, hdc);*/

        break;
   }


    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    
    }

    return 0;
}


int main()
{
    WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);

    return 0;
}
