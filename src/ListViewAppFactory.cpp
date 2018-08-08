// © You i Labs Inc. 2000-2017. All rights reserved.
#include "AppFactory.h"
#include "ListViewApp.h"

#define APP_NAME "Deep Link Tester"

#define APP_DENSITY (72)
#if defined(YI_PS4) || defined(YI_XBOX_360) || defined(YI_PS3)
#define APP_WIDTH (1920)
#define APP_HEIGHT (1080)
#else
#define APP_WIDTH (640)
#define APP_HEIGHT (480) 
#endif

CYIApp *AppFactory::Create()
{
    return new ListViewApp();
}

int AppFactory::GetWindowWidth()
{
    return APP_WIDTH;
}

int AppFactory::GetWindowHeight()
{
    return APP_HEIGHT;
}

int AppFactory::GetScreenDensity()
{
    return APP_DENSITY;
}

const char * AppFactory::GetWindowName()
{
    return APP_NAME;
}
