//
//  URWorld.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URWorld.h"

namespace UR
{
	World::World() :
		RN::World("OctreeSceneManager"),
		_gamepad(nullptr)
	{
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputInputDeviceRegistered, [&](RN::Message *message) {
			
			RN::GamepadDevice *device = message->GetObject()->Downcast<RN::GamepadDevice>();
			if(device)
			{
				RN::SafeRelease(_gamepad);
				_gamepad = device->Retain();
			}

		}, this);
	}
	
	World::~World()
	{
		RN::SafeRelease(_gamepad);
	}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, RN::Camera::Flags::Defaults);
	}

	void World::Update(float delta)
	{
		// Do something...
	}
}
