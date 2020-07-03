//
//  main.cpp
//  VludumdaRe NG
//
//  Copyright 2017 by �berpixel. All rights reserved.
//  Unauthorized use is punishable by torture, mutilation, and vivisection.
//

#include <Rayne.h>
#include "URApplication.h"

#if RN_BUILD_RELEASE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#if RN_PLATFORM_ANDROID
void android_main(struct android_app *app)
{
	RN::Initialize(0, nullptr, new UR::Application(), app);
}
#else
int main(int argc, const char *argv[])
{
#if RN_BUILD_DEBUG
	//	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
#endif
	RN::Initialize(argc, argv, new UR::Application());
}
#endif
