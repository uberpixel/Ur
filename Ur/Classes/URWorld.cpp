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
		_gamepad(nullptr),
		_ship(nullptr)
	{
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputInputDeviceRegistered, [&](RN::Message *message) {
			
			RN::GamepadDevice *device = message->GetObject()->Downcast<RN::GamepadDevice>();
			if(device)
			{
				if(_gamepad)
				{
					_gamepad->Deactivate();
					_gamepad->Release();
				}
				
				_gamepad = device->Retain();
				_gamepad->Activate();
				
				if(_ship)
					_ship->SetGamepad(_gamepad);
			}

		}, this);
	}
	
	World::~World()
	{
		RN::SafeRelease(_gamepad);
		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
	}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		// Physics world
		_physicsWorld = new RN::bullet::PhysicsWorld();
		_physicsWorld->SetGravity(RN::Vector3(0.0f));
		
		AddAttachment(_physicsWorld);
		
		// Camera and sky
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, RN::Camera::Flags::Defaults);
		RN::Model *sky = RN::Model::WithSkyCube("Sky/Skybox360 002 Up.png", "Sky/Skybox360 002 Down.png", "Sky/Skybox360 002 Left.png", "Sky/Skybox360 002 Right.png", "Sky/Skybox360 002 Back.png", "Sky/Skybox360 002 Front.png");
		_camera->SetSky(sky);
		
		// Player ship
		_ship = new SpaceShip();
		_ship->SetCamera(_camera);
		_ship->SetGamepad(_gamepad);
		
		// Sun
		RN::Light *sun = new RN::Light(RN::Light::Type::DirectionalLight);
		RN::ShadowParameter shadowParameter(_camera, 2048);
		shadowParameter.distanceBlendFactor = 0.01f;
		sun->ActivateShadows(shadowParameter);
		
		// UI
		_hudWidget = new RN::UI::Widget(RN::UI::Widget::Style::Titled, RN::Rect(40.0f, 40.0f, 220.0f, 60.0f));
		_hudWidget->Open();
		
		_speedLabel = new RN::UI::Label();
		_speedLabel->SetFrame(RN::Rect(0.0f, 10.0f, 220.0f, 18.0f).Inset(10.0f, 0.0f));
		_speedLabel->SetTextColor(RN::UI::Color::WhiteColor());
		
		_hudWidget->GetContentView()->AddSubview(_speedLabel);
		
		
		RN::Entity *temp = new RN::Entity(RN::Model::WithFile("Models/Ship/ship_outside.sgm"));
		temp->SetPosition(RN::Vector3(0.0f, 0.0f, 40.0f));
	}

	void World::Update(float delta)
	{
		RN::World::Update(delta);
		
		_speedLabel->SetText(RNSTR("Speed: %fm/s", _ship->GetSpeed()));
	}
}
