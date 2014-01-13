#include "main.h"
#include "EMBase.h"
#include "AppDelegate.h"
#include "test.h"
//#include "vld.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    srand(GetTickCount());

    // Create console for logging
    AllocConsole();
    SetConsoleTitle(L"Magnetic Pixels LOG");
    if (freopen("CONIN$", "r", stdin) == nullptr) return -1;
    if (freopen("CONOUT$", "w", stdout) == nullptr) return -1;
    if (freopen("CONOUT$", "w", stderr) == nullptr) return -1;

    // Reset color
    SetConsoleTextAttribute(
       GetStdHandle(STD_OUTPUT_HANDLE), 
       /*FOREGROUND_INTENSITY | */FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
    );

    MPix::TestClass::PerformTests();

    // create the application instance
    AppDelegate app;
    EGLView eglView;
    eglView.init("MagneticPixels", 640, 960);
    auto retVal = Application::getInstance()->run();
    FreeConsole();
    return retVal;
}
