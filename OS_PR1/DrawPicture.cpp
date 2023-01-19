// DrawPicture.cpp : точка входа для приложения

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

//Объявляем константы
const int pic_x = 0, pic_y = 0; //Позиция рисунка, устанавливаемая относительно правого нижнего угла
const wchar_t* picture_path = L"C:/Users/123/Desktop/Учёба/Учёба_3курс/ОС/ПР1/OS_PR1/Picture.bmp"; //Расположение файла с рисунком
const int pic_w = 559, pic_h = 559; //Размер рисунка.

//Объявляем переменные
HBITMAP loadpicture;  //Загруженная картинка
UINT wWidth, wHeight; //Ширина и высота окна

//Функция обратного вызова, прямые объявления функций
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//Создание входной точки для Win API
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    //Имя класса главного окна
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

    //Проверяем регистрацию окна и завершаем работу, если неудачно
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"Ошибка!", L"Окно не может быть зарегистрировано!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Cоздание windowex:
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,         //WS_EX_OVERLAPPEDWINDOW : необязательный расширенный стиль окна.
        szWindowClass,                  //szWindowClass: название приложения
        L"Десятиугольник",              //szTitle: текст, который появляется в строке заголовка
        WS_OVERLAPPEDWINDOW,            //WS_OVERLAPPEDWINDOW: тип окна для создания
    //Начальная позиция и размеры окна
        CW_USEDEFAULT, CW_USEDEFAULT,   //CW_USEDEFAULT, CW_USEDEFAULT: начальное положение (x, y)
        1280, 800,                       //Начальный размер (ширина, длина)
        NULL,                           //NULL: родительский элемент этого окна
        NULL,                           //NULL: в этом приложении нет строки меню
        hInstance,                      //экземпляр: первый параметр из WinMain
        NULL                            //NULL: не используется в этом приложении
    );
    //Завершение работы, окно не создано
    if (hWnd == NULL)
    {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow); //nCmdShow: четвертый параметр от WinMain
    UpdateWindow(hWnd);         //hWnd: значение, возвращаемое из CreateWindow

    //Создание петли для обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //Выход из главной функции
    return (0);
}

void drawBmp(HDC hdc, HBITMAP bmppic, int x, int y) {
    HDC
        hdcTemp,
        hdcBg, hdcFt, hdcMem, hdcObj;

    BITMAP bm;
    POINT ptSize;
    HBITMAP bmBg, bmFt, bmMem, bmObj;

    hdcTemp = CreateCompatibleDC(hdc);  //Cоздаем контекст устройства в памяти для дальнейших с ним манипуляций
    SelectObject(hdcTemp, bmppic);      //Выбираем изображение контексту - картинка из параметров

    //Извлекаем информацию bpj,hf;tybz, в перемменную 'bm'
    GetObject(bmppic, sizeof(BITMAP), (LPSTR)&bm);
    ptSize.x = bm.bmWidth;
    ptSize.y = bm.bmHeight;
    //Преобразуем координаты устройства в логические координаты
    DPtoLP(hdcTemp, &ptSize, 1);

    //Создаем контекст устройства в памяти.
    hdcBg = CreateCompatibleDC(hdc); // Background Обьекта
    hdcFt = CreateCompatibleDC(hdc); // Front обьектa
    hdcMem = CreateCompatibleDC(hdc); // Для обьединения
    hdcObj = CreateCompatibleDC(hdc); // Для наложения

    //Создаем растровое изображение
    bmBg = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);//background, однобитный
    bmFt = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);//objet, однобитный
    bmMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
    bmObj = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

    //Прежде, чем приложение сможет использовать контекст устройства памяти для операций рисования,
    //оно должно выбрать растровое изображение правильной ширины и высоты в контексте устройства
    SelectObject(hdcBg, bmBg);
    SelectObject(hdcFt, bmFt);
    SelectObject(hdcMem, bmMem);
    SelectObject(hdcObj, bmObj);

    //Копируем фон в рабочий дескриптор
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, x, y, SRCCOPY);
    //Копируем обьект из исходного
    BitBlt(hdcObj, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

    //Создание маски обьекта (монохромный) из исходного
    BitBlt(hdcFt, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
    //Создание маски фона путем инвертирования цветов обьекта
    BitBlt(hdcBg, 0, 0, ptSize.x, ptSize.y, hdcFt, 0, 0, NOTSRCCOPY);
    //Накладываем маску обьекта на скопированный фон
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcFt, 0, 0, SRCAND);
    //Накладываем маску фона на обьект
    BitBlt(hdcObj, 0, 0, ptSize.x, ptSize.y, hdcBg, 0, 0, SRCAND);
    //Объединяем цвета
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObj, 0, 0, SRCPAINT);
    //Помещаем на экран
    BitBlt(hdc, x, y, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);
    //Зыкрываем дырки, чтобы маска не сочилась
    BitBlt(hdcObj, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

    //Выбираем объекты
    SelectObject(hdcBg, bmBg);
    SelectObject(hdcFt, bmFt);
    SelectObject(hdcMem, bmMem);
    SelectObject(hdcObj, bmObj);

    //Очищаемм память
    DeleteDC(hdcBg);
    DeleteDC(hdcFt);
    DeleteDC(hdcMem);
    DeleteDC(hdcObj);

    DeleteDC(hdcTemp);
}

//Обработка сообщений для главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //Перечесление типов сообщений
    switch (message)
    {
    case WM_SIZE://Момент изменения размера окна, записываем в переменные
    {
        wWidth = LOWORD(lParam);
        wHeight = HIWORD(lParam);
        //Указываем Win API, что отрисованное окно не видно, и его нужно перерисовать
        InvalidateRect(hWnd, 0, TRUE);
    }
    break;

    case WM_CREATE://Момент создания окна, загружаем картинку из файла
    {
        loadpicture = (HBITMAP)LoadImage(nullptr, picture_path, IMAGE_BITMAP, pic_w, pic_h, LR_LOADFROMFILE);
    }
    break;

    case WM_PAINT://Момент отрисовки
    {
        HDC hdc;
        PAINTSTRUCT ps;
        int
            x = pic_x,
            y = ((wHeight - pic_h)/2);

        hdc = BeginPaint(hWnd, &ps); //Начало рисования

        //Рисуем загруженную картинку
        drawBmp(hdc, loadpicture, x, y);

        EndPaint(hWnd, &ps); //Конец рисования

    }
    break;

    case WM_CLOSE: //Момент закрытия окна
    {
        DestroyWindow(hWnd);
    }
    break;

    case WM_DESTROY://Момент удаления окна, удаляем объект картинки
    {
        DeleteObject(loadpicture);
        PostQuitMessage(0);
    }
    break;
    default://Обработка сообщений по умолчанию (вызывается, если тип сообщений не обрабатывался)
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
