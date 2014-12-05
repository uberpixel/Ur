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
		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
		
		RN::World *world = new World(World::Type::Client);
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
	}
	
	void Application::StartServer()
	{
		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
		
		UR::World *world = new World(World::Type::Server);
		
		RO::HMD *hmd = RO::System::GetSharedInstance()->GetHMD(0);
		if(hmd)
		{
			hmd->SetAsDisplay(false);
		}
		
		world->SetHMD(hmd);
		
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
	}
	
	
	void Application::Start()
	{
		SetTitle("Ur: GOTY Edition!");
		
		RN::Kernel::GetSharedInstance()->SetMaxFPS(60);
//		RN::Window::GetSharedInstance()->ActivateConfiguration(RN::Window::GetSharedInstance()->GetMainScreen()->GetConfigurations()->GetLastObject()->Downcast<RN::WindowConfiguration>(), RN::Window::Mask::Fullscreen);
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this](RN::Message *message) {
			
			RN::Event *event = message->Downcast<RN::Event>();
			if(event->GetType() == RN::Event::Type::KeyDown)
			{
				char key = event->GetCharacter();
				switch(key)
				{
					case '0':
						StartServer();
						break;
						
					case '1':
						StartClient();
						break;
				}
			}
			
		}, this);
	}
	
	void Application::WillExit()
	{}
}
