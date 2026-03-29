#include <windows.h>
#include "../../Document.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static Document document;
    switch (msg) {
    case WM_KEYDOWN: {
        if (GetKeyState(VK_CONTROL) & 0x8000) { // ctrl check

            // code for num 0
            if (wParam == 0x30) {
                document.alignment = 0;
                return 0; 
            }
            else if (wParam == 0x31) {
                document.alignment = 1;
                return 0;
            }
            else if (wParam == 0x32) {
                document.alignment = 2;
                return 0;
            }
            else if (wParam == 0x33) {
                document.alignment = 3;
                return 0;
            }
        }
        break;
    }

    case WM_CHAR: {
        
        if (wParam == 6) {
            document.isSearching = true;
            document.searchBufferLength = 0;
            document.searchBuffer[0] = L'\0';
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        if (wParam == L'0x1b') {

        }

        if (document.isSearching) {
            document.search(static_cast<wchar_t>(wParam));
        }
        else {
            if (wParam == '\r') wParam = '\n';
            document.insertCharacter(static_cast<wchar_t>(wParam));
            document.reflow();
        }

        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH bgBrush = CreateSolidBrush(RGB(20, 20, 40));
        FillRect(hdc, &rect, bgBrush);
        DeleteObject(bgBrush);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));

        HFONT font = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Trebuchet MS");
        SelectObject(hdc, font);

        if (document.getPageCount() > 0) {
            if (document.isSearching && document.searchBufferLength > 0) {
                document.getPage(0)->draw(hdc, rect, 50, 50, 32, 400, document.alignment, document.searchBuffer, document.searchBufferLength);
            }
            else {
                document.getPage(0)->draw(hdc, rect, 50, 50, 32, 400, document.alignment,nullptr, 0);
            }
        }

        document.draw(hdc, rect);

        DeleteObject(font);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"FastianWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Blackwood & Hargrove",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1440, 720,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}