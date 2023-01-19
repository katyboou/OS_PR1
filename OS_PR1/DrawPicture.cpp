// DrawPicture.cpp : ����� ����� ��� ����������

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

//��������� ���������
const int pic_x = 0, pic_y = 0; //������� �������, ��������������� ������������ ������� ������� ����
const wchar_t* picture_path = L"C:/Users/123/Desktop/�����/�����_3����/��/��1/OS_PR1/Picture.bmp"; //������������ ����� � ��������
const int pic_w = 559, pic_h = 559; //������ �������.

//��������� ����������
HBITMAP loadpicture;  //����������� ��������
UINT wWidth, wHeight; //������ � ������ ����

//������� ��������� ������, ������ ���������� �������
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//�������� ������� ����� ��� Win API
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    //��� ������ �������� ����
    static TCHAR szWindowClass[] = _T("DesktopApp");
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style           = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc     = WndProc;
    wcex.cbClsExtra      = 0;
    wcex.cbWndExtra      = 0;
    wcex.hInstance       = hInstance;
    wcex.hIcon           = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor         = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName    = NULL;
    wcex.lpszClassName   = szWindowClass;
    wcex.hIconSm         = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    //��������� ����������� ���� � ��������� ������, ���� ��������
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"������!", L"���� �� ����� ���� ����������������!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // C������� windowex:
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,         //WS_EX_OVERLAPPEDWINDOW : �������������� ����������� ����� ����.
        szWindowClass,                  //szWindowClass: �������� ����������
        L"��������������",              //szTitle: �����, ������� ���������� � ������ ���������
        WS_OVERLAPPEDWINDOW,            //WS_OVERLAPPEDWINDOW: ��� ���� ��� ��������
    //��������� ������� � ������� ����
        CW_USEDEFAULT, CW_USEDEFAULT,   //CW_USEDEFAULT, CW_USEDEFAULT: ��������� ��������� (x, y)
        1280, 800,                       //��������� ������ (������, �����)
        NULL,                           //NULL: ������������ ������� ����� ����
        NULL,                           //NULL: � ���� ���������� ��� ������ ����
        hInstance,                      //���������: ������ �������� �� WinMain
        NULL                            //NULL: �� ������������ � ���� ����������
    );
    //���������� ������, ���� �� �������
    if (hWnd == NULL)
    {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow); //nCmdShow: ��������� �������� �� WinMain
    UpdateWindow(hWnd);         //hWnd: ��������, ������������ �� CreateWindow

    //�������� ����� ��� ��������� ���������
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //����� �� ������� �������
    return (0);
}

void drawBmp(HDC hdc, HBITMAP bmppic, int x, int y) {
    HDC
        hdcTemp,
        hdcBg, hdcFt, hdcMem, hdcObj;

    BITMAP bm;
    POINT ptSize;
    HBITMAP bmBg, bmFt, bmMem, bmObj;

    hdcTemp = CreateCompatibleDC(hdc);  //C������ �������� ���������� � ������ ��� ���������� � ��� �����������
    SelectObject(hdcTemp, bmppic);      //�������� ����������� ��������� - �������� �� ����������

    //��������� ���������� bpj,hf;tybz, � ����������� 'bm'
    GetObject(bmppic, sizeof(BITMAP), (LPSTR)&bm);
    ptSize.x = bm.bmWidth;
    ptSize.y = bm.bmHeight;
    //����������� ���������� ���������� � ���������� ����������
    DPtoLP(hdcTemp, &ptSize, 1);

    //������� �������� ���������� � ������.
    hdcBg = CreateCompatibleDC(hdc); // Background �������
    hdcFt = CreateCompatibleDC(hdc); // Front ������a
    hdcMem = CreateCompatibleDC(hdc); // ��� �����������
    hdcObj = CreateCompatibleDC(hdc); // ��� ���������

    //������� ��������� �����������
    bmBg = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);//background, ����������
    bmFt = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);//objet, ����������
    bmMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
    bmObj = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

    //������, ��� ���������� ������ ������������ �������� ���������� ������ ��� �������� ���������,
    //��� ������ ������� ��������� ����������� ���������� ������ � ������ � ��������� ����������
    SelectObject(hdcBg, bmBg);
    SelectObject(hdcFt, bmFt);
    SelectObject(hdcMem, bmMem);
    SelectObject(hdcObj, bmObj);

    //�������� ��� � ������� ����������
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, x, y, SRCCOPY);
    //�������� ������ �� ���������
    BitBlt(hdcObj, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

    //�������� ����� ������� (�����������) �� ���������
    BitBlt(hdcFt, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
    //�������� ����� ���� ����� �������������� ������ �������
    BitBlt(hdcBg, 0, 0, ptSize.x, ptSize.y, hdcFt, 0, 0, NOTSRCCOPY);
    //����������� ����� ������� �� ������������� ���
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcFt, 0, 0, SRCAND);
    //����������� ����� ���� �� ������
    BitBlt(hdcObj, 0, 0, ptSize.x, ptSize.y, hdcBg, 0, 0, SRCAND);
    //���������� �����
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObj, 0, 0, SRCPAINT);
    //�������� �� �����
    BitBlt(hdc, x, y, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);
    //��������� �����, ����� ����� �� ��������
    BitBlt(hdcObj, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

    //�������� �������
    SelectObject(hdcBg, bmBg);
    SelectObject(hdcFt, bmFt);
    SelectObject(hdcMem, bmMem);
    SelectObject(hdcObj, bmObj);

    //�������� ������
    DeleteDC(hdcBg);
    DeleteDC(hdcFt);
    DeleteDC(hdcMem);
    DeleteDC(hdcObj);

    DeleteDC(hdcTemp);
}

//��������� ��������� ��� �������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //������������ ����� ���������
    switch (message)
    {
    case WM_SIZE://������ ��������� ������� ����, ���������� � ����������
    {
        wWidth = LOWORD(lParam);
        wHeight = HIWORD(lParam);
        //��������� Win API, ��� ������������ ���� �� �����, � ��� ����� ������������
        InvalidateRect(hWnd, 0, TRUE);
    }
    break;

    case WM_CREATE://������ �������� ����, ��������� �������� �� �����
    {
        loadpicture = (HBITMAP)LoadImage(nullptr, picture_path, IMAGE_BITMAP, pic_w, pic_h, LR_LOADFROMFILE);
    }
    break;

    case WM_PAINT://������ ���������
    {
        HDC hdc;
        PAINTSTRUCT ps;
        int
            x = pic_x,
            y = ((wHeight - pic_h)/2);

        hdc = BeginPaint(hWnd, &ps); //������ ���������

        //������ ����������� ��������
        drawBmp(hdc, loadpicture, x, y);

        EndPaint(hWnd, &ps); //����� ���������

    }
    break;

    case WM_CLOSE: //������ �������� ����
    {
        DestroyWindow(hWnd);
    }
    break;

    case WM_DESTROY://������ �������� ����, ������� ������ ��������
    {
        DeleteObject(loadpicture);
        PostQuitMessage(0);
    }
    break;
    default://��������� ��������� �� ��������� (����������, ���� ��� ��������� �� �������������)
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
