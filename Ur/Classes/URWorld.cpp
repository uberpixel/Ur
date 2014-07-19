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
		_missiles(new RN::Array()),
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
		_camera->SetClipFar(100000);
		
		// Player ship
		_ship = new SpaceShip(_client);
		_ship->SetCamera(_camera);
		_ship->SetGamepad(_gamepad);
		
		ReSpawn();
		
		// Sun
		RN::Light *sun = new RN::Light(RN::Light::Type::DirectionalLight);
		sun->SetColor(RN::Color(1.0f, 0.9f, 0.6f));
		sun->SetRotation(RN::Vector3(117.0f, 190.0f, 5.0f));
		RN::ShadowParameter shadowParameter(_camera, 4096);
		shadowParameter.distanceBlendFactor = 0.00001f;
		shadowParameter.splits[0].updateInterval = 1;
		shadowParameter.splits[1].updateInterval = 1;
		shadowParameter.splits[2].updateInterval = 1;
		shadowParameter.splits[3].updateInterval = 1;
		sun->ActivateShadows(shadowParameter);
		
		// Spawn asteroids
		GenerateAsteroids();
		
		// UI
		_hudWidget = new RN::UI::Widget(RN::UI::Widget::Style::Titled, RN::Rect(40.0f, 40.0f, 220.0f, 60.0f));
		_hudWidget->Open();
		
		_statsLabel = new RN::UI::Label();
		_statsLabel->SetFrame(RN::Rect(0.0f, 0.0f, 220.0f, 60).Inset(5.0f, 5.0f));
		_statsLabel->SetTextColor(RN::UI::Color::WhiteColor());
		_statsLabel->SetNumberOfLines(0);
		
		_hudWidget->GetContentView()->AddSubview(_statsLabel);
	}
	
	void World::GenerateAsteroids()
	{
		RN::Random::MersenneTwister random;
		random.Seed(0x1024);
		
		RN::Model *asteroid = RN::Model::WithFile("Models/Asteroids/asteroid_3.sgm");
		RN::InstancingNode *asteroidNode = new RN::InstancingNode();
		asteroidNode->SetModels(RN::Array::WithObjects(asteroid, nullptr));
		asteroidNode->SetPivot(_camera);
		asteroidNode->SetFlags(asteroidNode->GetFlags() | RN::SceneNode::Flags::NoSave);
		asteroidNode->SetMode(RN::InstancingNode::Mode::Thinning | RN::InstancingNode::Mode::Clipping);
		asteroidNode->SetCellSize(128.0f);
		asteroidNode->SetClippingRange(1024.0f);
		asteroidNode->SetThinningRange(512.0f);
		asteroidNode->Release();
		
		for(int i = 0; i < 5000; i ++)
		{
			RN::Vector3 pos(random.GetRandomFloatRange(-2000.0f, 2000.0f), random.GetRandomFloatRange(-2000.0f, 2000.0f), random.GetRandomFloatRange(-2000.0f, 2000.0f));
			
			RN::Entity *entity = new RN::Entity(asteroid, pos);
			entity->SetFlags(entity->GetFlags() | RN::SceneNode::Flags::Static | RN::SceneNode::Flags::NoSave);
			entity->SetScale(RN::Vector3(random.GetRandomFloatRange(1.0f, 10.0f)));
			entity->SetRotation(random.GetRandomVector3Range(RN::Vector3(0.0f), RN::Vector3(360.0f)));
			entity->Release();
					
			asteroidNode->AddChild(entity);
		}
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
	
	void World::ReSpawn()
	{
		_ship->Reset();
		_ship->SetPosition(_random.GetRandomVector3Range(RN::Vector3(-128), RN::Vector3(128)));
		
	}

	
	void World::AddMissileTracking(Missile *missile)
	{
		Lock();
		_missiles->AddObject(missile);
		Unlock();
	}
	void World::RemoveMissileTracking(Missile *missile)
	{
		Lock();
		_missiles->RemoveObject(missile);
		Unlock();
	}
	
	
	void World::Update(float delta)
	{
		if(_ship->GetHealth() <= 0)
			ReSpawn();
		
		
		if(_server)
			_server->Step();
		if(_client)
			_client->Step();
		
		std::unordered_set<Missile *> hits;
		
		_missiles->Enumerate<Missile>([&](Missile *missile, size_t index, bool &stop) {
			
			RN::Vector3 position = missile->GetPosition();
			
			_enemies->Enumerate<Enemy>([&](Enemy *enemy, size_t index, bool &stop) {
				
				float distance = (position.GetDistance(enemy->GetPosition()));
				
				if(distance < 10.0f)
				{
					// Good hit, good hit!
					
					Packet packet(Packet::Type::GoodHit);
					packet.WriteUInt32(enemy->GetClientID());
					packet.WriteFloat(distance);
					
					_client->SendPacket(packet);
					
					hits.insert(missile);
				}
				
			});
			
		});
		
		for(Missile *hit : hits)
		{
			_missiles->RemoveObject(hit);
			hit->RemoveFromWorld();
		}
		
		RN::Vector3 position = _ship->GetWorldPosition();
		_statsLabel->SetText(RNSTR("Speed: %.2fm/s\nPosition: {%.2f, %.2f, %.2f}\nHealth: %d", _ship->GetSpeed(), position.x, position.y, position.z, _ship->GetHealth()));
	}
}
