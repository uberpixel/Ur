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
	static World *_sharedWorld = nullptr;
	
	World::World(Type type) :
		RN::World("OctreeSceneManager"),
		_type(type),
		_enemies(new RN::Array()),
		_gamepad(nullptr),
		_ship(nullptr),
		_server(nullptr),
		_client(nullptr)
	{
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputInputDeviceRegistered, [&](RN::Message *message) {
			
			RN::GamepadDevice *device = message->GetObject()->Downcast<RN::GamepadDevice>();
			if(device)
				ActivateGamepad(device);
			
		}, this);
		
		RN::Array *devices = RN::Input::GetSharedInstance()->GetInputDevicesMatching(RN::InputDevice::Category::Gamepad);
		devices->Enumerate<RN::GamepadDevice>([&](RN::GamepadDevice *device, size_t index, bool &stop) {
			ActivateGamepad(device);
		});
		
		_sharedWorld = this;
	}
	
	World::~World()
	{
		RN::SafeRelease(_gamepad);
		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
	}
	
	World *World::GetSharedInstance()
	{
		return _sharedWorld;
	}
	
	
	void World::ActivateGamepad(RN::GamepadDevice *gamepad)
	{
		if(_gamepad)
		{
			_gamepad->Deactivate();
			_gamepad->Release();
		}
		
		_gamepad = gamepad->Retain();
		_gamepad->Activate();
		
		if(_ship)
			_ship->SetGamepad(_gamepad);
	}
	
	
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		// Client / Server
		switch(_type)
		{
			case Type::Client:
				_client = new Client(1337);
				_client->Connect("169.254.26.208");
				break;
				
			case Type::Server:
				_server = new Server(1337);
				_server->Open();
				
				{
					RN::Thread *thread;
					
					thread = new RN::Thread([&]{
						
						while(!thread->IsCancelled())
							_server->Step();
						
					}, false);
					
					thread->Detach();
					
					_client = new Client(1337);
					_client->Connect("127.0.0.1");
					
					thread->Cancel();
					thread->WaitForExit();
					thread->Release();
				}
				break;
		}
		
		if(!_client->IsConnected())
			abort();
		
		
		// Physics world
		_physicsWorld = new RN::bullet::PhysicsWorld();
		_physicsWorld->SetGravity(RN::Vector3(0.0f));
		
		AddAttachment(_physicsWorld);
		
		// Camera and sky
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, RN::Camera::Flags::Defaults&~RN::Camera::Flags::UseFog);
		RN::Model *sky = RN::Model::WithSkyCube("Sky/Skybox360 002 Up.png", "Sky/Skybox360 002 Down.png", "Sky/Skybox360 002 Left.png", "Sky/Skybox360 002 Right.png", "Sky/Skybox360 002 Back.png", "Sky/Skybox360 002 Front.png");
		_camera->SetSky(sky);
		
		// Player ship
		_ship = new SpaceShip(_client);
		_ship->SetCamera(_camera);
		_ship->SetGamepad(_gamepad);
		
		// Sun
		RN::Light *sun = new RN::Light(RN::Light::Type::DirectionalLight);
		sun->SetRotation(RN::Vector3(117.0f, 190.0f, 5.0f));
		RN::ShadowParameter shadowParameter(_camera, 2048);
		shadowParameter.distanceBlendFactor = 0.01f;
		shadowParameter.splits[0].updateInterval = 1;
		shadowParameter.splits[1].updateInterval = 1;
		shadowParameter.splits[2].updateInterval = 1;
		shadowParameter.splits[3].updateInterval = 1;
		sun->ActivateShadows(shadowParameter);
		
		// UI
		_hudWidget = new RN::UI::Widget(RN::UI::Widget::Style::Titled, RN::Rect(40.0f, 40.0f, 220.0f, 60.0f));
		_hudWidget->Open();
		
		_speedLabel = new RN::UI::Label();
		_speedLabel->SetFrame(RN::Rect(0.0f, 10.0f, 220.0f, 18.0f).Inset(10.0f, 0.0f));
		_speedLabel->SetTextColor(RN::UI::Color::WhiteColor());
		
		_hudWidget->GetContentView()->AddSubview(_speedLabel);
	}
	
	Enemy *World::GetEnemyWithID(uint32 clientID)
	{
		Enemy *result = nullptr;
		
		_enemies->Enumerate<Enemy>([&](Enemy *enemy, size_t index, bool &stop) {
			
			if(enemy->GetClientID() == clientID)
			{
				result = enemy;
				stop = true;
			}
			
		});

		if(!result)
		{
			result = new Enemy(clientID);
			_enemies->AddObject(result->Autorelease());
		}

		return result;
	}

	void World::Update(float delta)
	{
		if(_server)
			_server->Step();
		if(_client)
			_client->Step();
		
		
		_speedLabel->SetText(RNSTR("Speed: %fm/s", _ship->GetSpeed()));
	}
}
