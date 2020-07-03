//
//  URWorld.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URWorld.h"
#include "URExplosion.h"
#include "URAsteroid.h"

namespace UR
{
	static World *_sharedWorld = nullptr;
	
	World::World(Type type) :
		_type(type),
		_enemies(new RN::Array()),
		_missiles(new RN::Array()),
//		_gamepad(nullptr),
		_ship(nullptr),
		_server(nullptr),
		_client(nullptr),
		_kills(0),
		_deaths(0)
//		_hmd(nullptr)
	{
/*		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputInputDeviceRegistered, [&](RN::Message *message) {
			
			RN::GamepadDevice *device = message->GetObject()->Downcast<RN::GamepadDevice>();
			if(device)
				ActivateGamepad(device);
			
		}, this);
		
		RN::Array *devices = RN::Input::GetSharedInstance()->GetInputDevicesMatching(RN::InputDevice::Category::Gamepad);
		devices->Enumerate<RN::GamepadDevice>([&](RN::GamepadDevice *device, size_t index, bool &stop) {
			ActivateGamepad(device);
		});*/
		
		_audioWorld = new RN::OpenALWorld();
		AddAttachment(_audioWorld);
        
        _networkWorld = new RN::ENetWorld();
        AddAttachment(_networkWorld->Autorelease());
		
		_sharedWorld = this;
	}
	
	World::~World()
	{
//		RN::SafeRelease(_gamepad);
//		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
	}
	
	World *World::GetSharedInstance()
	{
		return _sharedWorld;
	}
	
	
/*	void World::ActivateGamepad(RN::GamepadDevice *gamepad)
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
	}*/
	
/*	RN::PostProcessingPipeline *World::PPCreateBloomPipeline(RN::Camera *cam)
	{
		RN::Shader *combineShader = RN::Shader::WithFile("shader/rn_PPCombine");
		RN::Shader *blurShader = RN::Shader::WithFile("shader/rn_GaussBlur");
		RN::Shader *updownShader = RN::Shader::WithFile("shader/rn_PPCopy");
		
		float blurWeights[10];
		float sigma = 4.5/3;
		sigma *= sigma;
		int n = 0;
		float gaussfactor = 1.0f/(sqrt(2.0f*RN::k::Pi*sigma));
		float weightsum = 0;
		for(float i = -4.5f; i <= 4.6f; i += 1.0f)
		{
			blurWeights[n] = exp(-(i*i/2.0f*sigma))*gaussfactor;
			weightsum += blurWeights[n];
			n++;
		}
		for(int i = 0; i < n; i++)
		{
			blurWeights[i] /= weightsum;
		}
		
		RN::Material *blurXMaterial = new RN::Material(blurShader);
		blurXMaterial->Define("RN_BLURX");
		blurXMaterial->AddShaderUniform("kernelWeights", RN::Material::ShaderUniform::Type::Float1, blurWeights, 10, true);
		
		RN::Material *blurYMaterial = new RN::Material(blurShader);
		blurYMaterial->Define("RN_BLURY");
		blurYMaterial->AddShaderUniform("kernelWeights", RN::Material::ShaderUniform::Type::Float1, blurWeights, 10, true);
		
		RN::Material *downMaterial = new RN::Material(updownShader);
		downMaterial->Define("RN_DOWNSAMPLE");
		
		// Filter bright
		RN::Shader *filterBrightShader = RN::Shader::WithFile("shader/rn_FilterBright");
		RN::Material *filterBrightMaterial = new RN::Material(filterBrightShader);
		RN::Camera *filterBright = new RN::Camera(cam->GetFrame().GetSize() / 2.0f, RN::Texture::Format::RGB16F, RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		filterBright->SetMaterial(filterBrightMaterial);
		
		// Down sample
		RN::Camera *downSample4x = new RN::Camera(cam->GetFrame().GetSize() / 4.0f, RN::Texture::Format::RGB16F, RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		downSample4x->SetMaterial(downMaterial);
		
		// Down sample
		RN::Camera *downSample8x = new RN::Camera(cam->GetFrame().GetSize() / 8.0f, RN::Texture::Format::RGB16F, RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		downSample8x->SetMaterial(downMaterial);
		
		// Blur X
		RN::Camera *bloomBlurXlow = new RN::Camera(cam->GetFrame().GetSize() / 8.0f, RN::Texture::Format::RGB16F, RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		bloomBlurXlow->SetMaterial(blurXMaterial);
		
		// Blur Y
		RN::Camera *bloomBlurYlow = new RN::Camera(cam->GetFrame().GetSize() / 8.0f, RN::Texture::Format::RGB16F, RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		bloomBlurYlow->SetMaterial(blurYMaterial);
		
		// Blur X
		RN::Camera *bloomBlurXhigh = new RN::Camera(cam->GetFrame().GetSize() / 4.0f, RN::Texture::Format::RGB16F, RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		bloomBlurXhigh->SetMaterial(blurXMaterial);
		
		// Blur Y
		RN::Camera *bloomBlurYhigh = new RN::Camera(cam->GetFrame().GetSize() / 4.0f, RN::Texture::Format::RGB16F, RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		bloomBlurYhigh->SetMaterial(blurYMaterial);
		
		// Combine
		RN::Material *bloomCombineMaterial = new RN::Material(combineShader);
		bloomCombineMaterial->AddTexture(bloomBlurYhigh->GetStorage()->GetRenderTarget());
		
		RN::Camera *bloomCombine = new RN::Camera(RN::Vector2(0.0f), RN::Texture::Format::RGB16F, RN::Camera::Flags::Inherit | RN::Camera::Flags::UpdateStorageFrame, RN::RenderStorage::BufferFormatColor);
		bloomCombine->SetMaterial(bloomCombineMaterial);
		
		RN::PostProcessingPipeline *bloomPipeline = cam->AddPostProcessingPipeline("Bloom", 3);
		bloomPipeline->AddStage(filterBright->Autorelease(), RN::RenderStage::Mode::ReUsePreviousStage);
		bloomPipeline->AddStage(downSample4x->Autorelease(), RN::RenderStage::Mode::ReUsePreviousStage);
		bloomPipeline->AddStage(downSample8x->Autorelease(), RN::RenderStage::Mode::ReUsePreviousStage);
		bloomPipeline->AddStage(bloomBlurXlow->Autorelease(), RN::RenderStage::Mode::ReUsePreviousStage);
		bloomPipeline->AddStage(bloomBlurYlow->Autorelease(), RN::RenderStage::Mode::ReUsePreviousStage);
		bloomPipeline->AddStage(bloomBlurXhigh->Autorelease(), RN::RenderStage::Mode::ReUsePreviousStage);
		bloomPipeline->AddStage(bloomBlurYhigh->Autorelease(), RN::RenderStage::Mode::ReUsePreviousStage);
		bloomPipeline->AddStage(bloomCombine->Autorelease(), RN::RenderStage::Mode::ReUsePipeline);
		
		return bloomPipeline;
	}*/
	
	
	void World::WillBecomeActive()
	{
		// Client / Server
		switch(_type)
		{
			case Type::Client:
				_client = new Client(1337);
                _networkWorld->AddHost(_client);
				_client->Connect(RNCSTR("localhost"), 1337);
				break;
				
			case Type::Server:
				_server = new Server(1337);
                _networkWorld->AddHost(_server);
				{
/*					RN::Thread *thread;
					
					thread = new RN::Thread([&]{
						
						while(!thread->IsCancelled())
							_server->Step();
						
					}, false);
					
					thread->Detach();*/
					
					_client = new Client(1337);
                    _networkWorld->AddHost(_client);
					_client->Connect(RNCSTR("127.0.0.1"), 1337);
					
				/*	thread->Cancel();
					thread->WaitForExit();
					thread->Release();*/
				}
				break;
		}
		
//		if(!_client->IsConnected())
//			abort();
		
		
		// Physics world
		_physicsWorld = new RN::BulletWorld();
		_physicsWorld->SetGravity(RN::Vector3(0.0f));
		
		AddAttachment(_physicsWorld);
		
		// Camera and sky
        RN::Model *sky = RN::Model::WithSkycube(RNCSTR("Sky/Skybox360 002 Left.png"), RNCSTR("Sky/Skybox360 002 Front.png"), RNCSTR("Sky/Skybox360 002 Right.png"), RNCSTR("Sky/Skybox360 002 Back.png"), RNCSTR("Sky/Skybox360 002 Up.png"), RNCSTR("Sky/Skybox360 002 Down.png"));
        RN::Entity *skyEntity = new RN::Entity(sky);
        skyEntity->SetScale(100000.0f);
        skyEntity->AddFlags(RN::Entity::Flags::DrawLate);
        AddNode(skyEntity->Autorelease());
		
		// Sun
		RN::Light *sun = new RN::Light(RN::Light::Type::DirectionalLight);
		sun->SetColor(RN::Color(1.0f, 0.9f, 0.6f));
		sun->SetRotation(RN::Vector3(117.0f, 190.0f, 5.0f));
        AddNode(sun->Autorelease());
		
/*		if(_hmd)
		{
			RO::Camera *tempCamera = new RO::Camera(RN::Texture::Format::RGB16F);
			tempCamera->SetHMD(_hmd);
			tempCamera->SetAmbientColor(RN::Color::WithHSV(0.0f, 0.0f, 1.5f));
			tempCamera->GetLeftCamera()->SetSky(sky);
			tempCamera->GetRightCamera()->SetSky(sky);
			tempCamera->GetLeftCamera()->SetClipFar(50000);
			tempCamera->GetRightCamera()->SetClipFar(50000);
			
			_camera = tempCamera;
		}
		else*/
		{
			
/*			RN::Camera *tempCamera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, RN::Camera::Flags::Defaults&~RN::Camera::Flags::UseFog);
			tempCamera->SetBlitShader(RN::Shader::WithFile("shader/rn_DrawFramebufferTonemap"));
			PPCreateBloomPipeline(tempCamera);
			tempCamera->SetSky(sky);
			tempCamera->SetClipFar(50000);
			tempCamera->SetRenderGroups(RN::Camera::RenderGroups::Group0|RN::Camera::RenderGroups::Group2);
			tempCamera->GetLightManager()->Downcast<RN::ClusteredLightManager>()->SetClusterSize(RN::Vector3(256, 256, 1000));
			
			RN::ShadowParameter shadowParameter(tempCamera, 2048);
			shadowParameter.distanceBlendFactor = 0.00001f;
			shadowParameter.splits[0].updateInterval = 1;
			shadowParameter.splits[1].updateInterval = 1;
			shadowParameter.splits[2].updateInterval = 1;
			shadowParameter.splits[3].updateInterval = 1;
			sun->ActivateShadows(shadowParameter);
			
			_camera = tempCamera;*/
            
            _camera = new RN::Camera();
            _camera->SetClipFar(50000);
            //_camera->SetRenderGroups(RN::Camera::RenderGroups::Group0|RN::Camera::RenderGroups::Group2);
            
 /*           RN::Window::SwapChainDescriptor swapchainDescriptor(RN::Texture::Format::RGBA_16F, RN::Texture::Format::Depth_32F);
            swapchainDescriptor.vsync = false;
            RN::Window *previewWindow = RN::Renderer::GetActiveRenderer()->CreateAWindow(RN::Vector2(1024, 768), RN::Screen::GetMainScreen(), swapchainDescriptor);
            previewWindow->Show();
            _camera->GetRenderPass()->SetFramebuffer(previewWindow->GetFramebuffer());*/
		}
		
//		RN::Renderer::GetSharedInstance()->SetHDRExposure(1.0f);
//		RN::Renderer::GetSharedInstance()->SetHDRWhitePoint(2.5f);
		
		// Player ship
		_ship = new SpaceShip(_client);
        AddNode(_ship->Autorelease());
		_ship->SetCamera(_camera);
//		_ship->SetGamepad(_gamepad);
		
		ReSpawn();
		
		// Spawn asteroids
		GenerateAsteroids();
		
		// UI
/*		_hudWidget = new RN::UI::Widget(RN::UI::Widget::Style::Titled, RN::Rect(40.0f, 40.0f, 220.0f, 60.0f));
		_hudWidget->Open();
		
		_statsLabel = new RN::UI::Label();
		_statsLabel->SetFrame(RN::Rect(0.0f, 0.0f, 220.0f, 60).Inset(5.0f, 5.0f));
		_statsLabel->SetTextColor(RN::UI::Color::White());
		_statsLabel->SetNumberOfLines(0);
		
		_hudWidget->GetContentView()->AddSubview(_statsLabel);
		*/
		RN::OpenALListener *audioListener = new RN::OpenALListener();
		_camera->AddAttachment(audioListener);
        _audioWorld->SetListener(audioListener);
		
		RN::AudioAsset *audio = RN::AudioAsset::WithName(RNCSTR("Audio/spaceship-ambience.ogg"));
		RN::OpenALSource *audioSource = _audioWorld->PlaySound(audio);
		audioSource->SetRepeat(true);
	}
	
	void World::GenerateAsteroids()
	{
		RN::Random::MersenneTwister random;
		random.Seed(0x1024);
		
		RN::Model *asteroid1 = RN::Model::WithName(RNCSTR("Models/Asteroids/asteroid_3.sgm"));
		asteroid1->GetLODStage(0)->GetMaterialAtIndex(0)->SetAmbientColor(RN::Color::Black());
		
		RN::Model *asteroid2 = RN::Model::WithName(RNCSTR("Models/Asteroids/asteroid_2.sgm"));
		asteroid2->GetLODStage(0)->GetMaterialAtIndex(0)->SetAmbientColor(RN::Color::Black());
		
		//RN::InstancingNode *asteroidNode = new RN::InstancingNode();
		//asteroidNode->SetModels(RN::Array::WithObjects(asteroid1, asteroid2, nullptr));
		//asteroidNode->Release();
		
		for(int i = 0; i < 500; i ++)
		{
			RN::Vector3 pos(random.GetRandomVector3Range(RN::Vector3(-1200.0f), RN::Vector3(1200.0f)));
			
			Asteroid *entity = new Asteroid((random.GetRandomInt32()>0)?asteroid1:asteroid2, pos, RN::Vector3(random.GetRandomFloatRange(1.0f, 10.0f)));
			//entity->SetFlags(entity->GetFlags() | RN::SceneNode::Flags::Static | RN::SceneNode::Flags::NoSave);
			entity->SetRotation(random.GetRandomVector3Range(RN::Vector3(0.0f), RN::Vector3(360.0f)));
            
            AddNode(entity->Autorelease());
			//asteroidNode->AddChild(entity);
		}
	}
	
	Enemy *World::GetEnemyWithID(RN::uint32 clientID)
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
            AddNode(result);
		}

		return result;
	}
	
	void World::RemoveEnemy(RN::uint32 clientID)
	{
		Enemy *result = nullptr;
		
		_enemies->Enumerate<Enemy>([&](Enemy *enemy, size_t index, bool &stop) {
			
			if(enemy->GetClientID() == clientID)
			{
				result = enemy;
				stop = true;
			}
			
		});
		
		if(result)
		{
            RemoveNode(result);
			_enemies->RemoveObject(result);
		}
	}
	
	RN::Array *World::GetEnemies() const
	{
		return _enemies;
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
	
	void World::TallyKill()
	{
		Lock();
		_kills ++;
		Unlock();
	}

	void World::DidUpdate(float delta)
	{
        SceneBasic::DidUpdate(delta);
        
		if(_ship->GetHealth() <= 0)
		{
			RN::Vector3 position = _ship->GetWorldPosition();
			
			Packet packet(Packet::Type::GoodKill);
			packet.WriteUInt32(_client->GetClientID());
			packet.WriteUInt32(_ship->GetKillID());
			
			packet.WriteFloat(position.x);
			packet.WriteFloat(position.y);
			packet.WriteFloat(position.z);
			
			if(_ship->GetKillID() == 0)
				_kills --;
			
			_client->SendPacket(packet);
			_deaths ++;
			
			ReSpawn();
		}
		
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
					packet.WriteUInt32(_client->GetClientID());
					
					RN::Vector3 position = missile->GetWorldPosition();
					
					packet.WriteFloat(position.x);
					packet.WriteFloat(position.y);
					packet.WriteFloat(position.z);
					
					_client->SendPacket(packet);
					
					hits.insert(missile);
				}
				
			});
			
		});
		
		for(Missile *hit : hits)
		{
			_missiles->RemoveObject(hit);
            RemoveNode(hit);
		}
		
/*		RN::Vector3 position = _ship->GetWorldPosition();
		_statsLabel->SetText(RNSTR("Speed: %.2fm/s\nKills: %d, Deaths: %d\nHealth: %d", _ship->GetSpeed(), _kills, _deaths, position.x, position.y, position.z, _ship->GetHealth()));
		
		if(_gamepad)
			_gamepad->ExecuteCommand(RNCSTR("light"), RN::Value::WithVector3(RN::Vector3(255.0f, 0.0f, 0.0f).GetLerp(RN::Vector3(0.0f, 255.0f, 0.0f), _ship->GetHealth()*0.0066f)));*/
	}
	
/*	void World::SetHMD(RO::HMD *hmd)
	{
		if(_hmd)
			return;
		
		_hmd = hmd;
	}*/

    RN::Model *World::MakeDeepCopy(RN::Model *model) const
    {
        RN::Model *result = model->Copy();
        
        RN::Model::LODStage *lodStage = result->GetLODStage(0);
        for(int i = 0; i < lodStage->GetCount(); i++)
        {
            RN::Material *material = lodStage->GetMaterialAtIndex(i)->Copy();
            lodStage->ReplaceMaterial(material->Autorelease(), i);
        }
        
        return result->Autorelease();
    }
}
