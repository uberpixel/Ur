//
//  URApplication.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URApplication.h"

namespace UR
{
	Application::Application()
	{}
	Application::~Application()
	{}
	
	
	void Application::StartClient()
	{
		World *world = new World(World::Type::Client);
		RN::SceneManager::GetSharedInstance()->AddScene(world->Autorelease());
	}
	
	void Application::StartServer()
	{
		World *world = new World(World::Type::Server);
		
/*		RO::HMD *hmd = RO::System::GetSharedInstance()->GetHMD(0);
		if(hmd)
		{
			hmd->SetAsDisplay(false);
		}
		
		world->SetHMD(hmd);*/
		
        RN::SceneManager::GetSharedInstance()->AddScene(world->Autorelease());
	}
	
	
	void Application::DidFinishLaunching(RN::Kernel *kernel)
	{
		RN::Kernel::GetSharedInstance()->SetMaxFPS(200);
        
        RN::Window::SwapChainDescriptor swapchainDescriptor(RN::Texture::Format::RGBA_16F, RN::Texture::Format::Depth_32F);
        swapchainDescriptor.vsync = false;
        RN::Window *previewWindow = RN::Renderer::GetActiveRenderer()->CreateAWindow(RN::Vector2(1024, 768), RN::Screen::GetMainScreen(), swapchainDescriptor);
        previewWindow->Show();
        
        if(RN::Kernel::GetSharedInstance()->GetArguments().HasArgument("server", 0)) StartServer();
        else StartClient();
	}
}
