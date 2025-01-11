#include "pch.h"
#include "framework.h"
#include "LineDrawingGDI.h"

#include <objidl.h>
#include <gdiplus.h>

#include <cmath>
#include <array>

using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

constexpr int width = 800;
constexpr int height = 600;

struct Line {
    int x0, y0;
    int x1, y1;
};

VOID OnPaint(HDC hdc) {
    Graphics graphics(hdc);
    Pen      pen(Color(255, 0, 0, 0));

    // Draw axis.
    Pen      pen_x(Color(255, 255, 0, 0));
    graphics.DrawLine(&pen_x, 0, height / 2 - 1, width - 1, height / 2 - 1);
    Pen      pen_y(Color(255, 0, 255, 0));
    graphics.DrawLine(&pen_y, width / 2, 0, width / 2, height - 1);

    std::array<Line, 14> lines{ {
        {round(2.14715f), round(0.47057f), round(3.64715f), round(0.47057f)},
        {round(1.81549f), round(1.02138f), round(3.62763f), round(1.65096f)},
        {round(0.94892f), round(2.07485f), round(8.55611f), round(17.03125f)},
        {round(0.5f), round(2.0f), round(0.5f), round(7.5f)},
        {round(-0.8505f), round(1.64894f), round(-5.81278f), round(14.40616f)},
        {round(-5.0f), round(5.0f), round(-15.0f), round(15.0f)},
        {round(-1.4419f), round(0.66912f), round(-12.96741f), round(6.70705f)},
        {round(-8.226f), round(0.0f), round(-22.2464f), round(0.0f)},
        {round(-1.42654f), round(-0.85331f), round(-17.9893f), round(-5.21855f)},
        {round(-5.0f), round(-5.0f), round(-12.0f), round(-12.0f)},
        {round(-0.66344f), round(-1.51963f), round(-8.68639f), round(-15.49434f)},
        {round(0.52943f), round(-1.17598f), round(-3.7053f), round(-16.18403f)},
        {round(1.31772f), round(-2.10619f), round(10.16512f), round(-22.39123f)},
        {round(1.71092f), round(-0.64989f), round(3.37499f), round(-1.43225f)}
    } };

    for (const Line& line : lines) {
        graphics.DrawLine(&pen,
            (width / 2) + line.x0, (height / 2 - 1) - line.y0,
            (width / 2) + line.x1, (height / 2 - 1) - line.y1);
    }
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow) {
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("Line Drawing");

    RegisterClass(&wndClass);

    RECT rect = { 0, 0, width, height };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

    hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        TEXT("Line Drawing"),
        TEXT("Line Drawing"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int actualClientWidth = clientRect.right - clientRect.left;
    int actualClientHeight = clientRect.bottom - clientRect.top;

    wchar_t debugMessage[100];
    swprintf_s(debugMessage, L"Client Size: %dx%d\n", actualClientWidth, actualClientHeight);
    OutputDebugString(debugMessage);

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);

        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}