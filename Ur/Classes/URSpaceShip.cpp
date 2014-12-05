//
//  URSpaceShip.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URSpaceShip.h"
#include "URMissile.h"

namespace UR
{
	RNDefineMeta(SpaceShip, RN::Entity)
	
	SpaceShip *_localShip = nullptr;
	
	SpaceShip::SpaceShip(Client *client) :
		_camera(nullptr),
		_gamepad(nullptr),
		_radar(nullptr),
		_client(client),
		_health(150),
		_killID(0),
		_maxThrust(5.0),
		_maxVelocity(500.0),
		_engineState(0b00001111),
		_weaponCoolDown1(0.0f),
		_weaponCoolDown2(0.0f),
		_weaponLocked1(false),
		_weaponLocked2(false),
		_weaponRumble(0.0),
		_damageCooldown(0.0)
	{
		RN::Model *model = RN::Model::WithFile("Models/Ship/ship_outside.sgm");
		
		RN::bullet::Shape *shape = RN::bullet::TriangleMeshShape::WithModel(model);
		
		SetModel(RN::Model::WithFile("Models/Ship/ship_inside.sgm"));
		RN::Material *window = GetModel()->GetMaterialAtIndex(0, 2);
		window->SetBlending(true);
		window->Define("RN_GLASS");
		window->Define("RN_SPECULARITY");
		window->SetSpecularColor(RN::Color(0.05f, 0.05f, 0.05f, 100.0f));
		window->SetDiffuseColor(RN::Color(0.01f, 0.01f, 0.01f, 0.05f));
		
		RN::Material *body = GetModel()->GetMaterialAtIndex(0, 0);
		body->Define("RN_SPECULARITY");
		body->SetSpecularColor(RN::Color(0.03f, 0.03f, 0.03f, 10.0f));
		
		RN::Material *borders = GetModel()->GetMaterialAtIndex(0, 1);
		borders->Define("RN_SPECULARITY");
		borders->SetSpecularColor(RN::Color(0.03f, 0.03f, 0.03f, 150.0f));
		
		RN::bullet::PhysicsMaterial *material = new RN::bullet::PhysicsMaterial();
		material->SetLinearDamping(0.25);
		material->SetFriction(0.0);
		material->SetAngularDamping(0.9);
		
		_rigidBody = new RN::bullet::RigidBody(shape, 10.0f);
		_rigidBody->SetMaterial(material);
		
		AddAttachment(_rigidBody);
		
//		_radar = new Radar();
		
		_localShip = this;
	}
	
	SpaceShip *SpaceShip::GetLocalShip()
	{
		return _localShip;
	}
	
	
	void SpaceShip::SetCamera(RN::SceneNode *camera)
	{
		if(_camera)
			_camera->RemoveFromParent();
		
		AddChild(camera);
		
		_camera = camera;
		_camera->SetPosition(RN::Vector3(0.0f, 0.8f, -0.8f));
		_camera->SetRotation(RN::Vector3(0.0f, -5.0f, 0.0f));
	}
	
	void SpaceShip::SetGamepad(RN::GamepadDevice *gamepad)
	{
		_gamepad = gamepad;
	}
	
	
	float SpaceShip::GetSpeed() const
	{
		RN::Vector3 velocity = _rigidBody->GetLinearVelocity();
		return velocity.GetLength();
	}
	
	RN::Vector3 SpaceShip::GetVelocity() const
	{
		return _rigidBody->GetLinearVelocity();
	}
	
	void SpaceShip::SetEngineState(uint8 engine, bool working)
	{
		if(working)
			_engineState |= (1 << engine);
		else
			_engineState &= ~(1 << engine);
	}
	
	void SpaceShip::TakeHit(const RN::Vector3 &position, uint32 clientID)
	{
		_rigidBody->ApplyImpulse((GetWorldPosition()-position).GetNormalized(250.0f));
		_damageCooldown = 0.8f;
		_health -= 40;
		
		if(_health <= 0 && _killID == 0)
			_killID = clientID;
	}
	
	void SpaceShip::Die()
	{
		_health = 0;
	}
	
	void SpaceShip::Reset()
	{
		_rigidBody->SetLinearVelocity(RN::Vector3());
		_health = 150;
		_killID = 0;
	}

	bool SpaceShip::GetEngineState(uint8 engine)
	{
		return (_engineState & (1 << engine));
	}
	
	void SpaceShip::Update(float delta)
	{
		RN::Input *input = RN::Input::GetSharedInstance();
		float keyAcceleration = input->IsKeyPressed('k')-input->IsKeyPressed('i');
		RN::Vector3 keyRotation;
		keyRotation.x = input->IsKeyPressed('d')-input->IsKeyPressed('a');
		keyRotation.y = input->IsKeyPressed('s')-input->IsKeyPressed('w');
		keyRotation.z = input->IsKeyPressed('e')-input->IsKeyPressed('q');
		bool keyShootLeft = input->IsKeyPressed('j');
		bool keyShootRight = input->IsKeyPressed('l');
		
		if(_gamepad)
		{
			keyAcceleration = _gamepad->GetTrigger1() - _gamepad->GetTrigger2();
			
			keyRotation.x = _gamepad->GetAnalog1().x;
			keyRotation.y = _gamepad->GetAnalog1().y;
			keyRotation.z = _gamepad->GetAnalog2().x;
			
			keyShootLeft = _gamepad->IsButtonPressed(8);
			keyShootRight = _gamepad->IsButtonPressed(9);
		}
		
		
		uint8 rumble = 1;
		RN::Quaternion rotation = GetRotation();
		
		// Thrust
		RN::Vector3 velocity = _rigidBody->GetLinearVelocity();
		
		if(velocity.GetLength() > _maxVelocity)
		{
			velocity *= _maxVelocity / velocity.GetLength();
			_rigidBody->SetLinearVelocity(velocity);
		}
		
		float thrust = keyAcceleration * _maxThrust;
		if(RN::Math::FastAbs(thrust) > 0.001)
		{
			RN::Vector3 impulseCenter;
			
			// Change behaviour based on missing engines
			int engineCount = 0;
			if(_engineState & (1 << 0))
			{
				impulseCenter += RN::Vector3(-20.0f, 10.0f, 0.0f);
				engineCount++;
			}
			if(_engineState & (1 << 1))
			{
				impulseCenter += RN::Vector3(-10.0f, 10.0f, 0.0f);
				engineCount++;
			}
			if(_engineState & (1 << 2))
			{
				impulseCenter += RN::Vector3(10.0f, 10.0f, 0.0f);
				engineCount++;
			}
			if(_engineState & (1 << 3))
			{
				impulseCenter += RN::Vector3(20.0f, 10.0f, 0.0f);
				engineCount++;
			}
			
			impulseCenter /= engineCount;
			
			if(engineCount > 0)
			{
				thrust *= engineCount/4.0f;
				RN::Vector3 vector(0.0f, 0.0f, thrust);
				_rigidBody->ApplyImpulse(rotation.GetRotatedVector(vector), impulseCenter);
				rumble = RN::Math::FastAbs(thrust)/_maxThrust*80;
			}
		}
		
		// Rotation
		RN::Vector3 angularVelocity;
	
		angularVelocity.y = -keyRotation.x * 0.1f;
		angularVelocity.x = keyRotation.y * 0.1f;
		angularVelocity.z = -keyRotation.z * 0.1f;
		
		_rigidBody->SetAngularVelocity(rotation.GetRotatedVector(angularVelocity) + _rigidBody->GetAngularVelocity());
		_rigidBody->GetBulletCollisionObject()->activate(true);
		

		// Shooting
		if(_client->IsReady())
		{
			_weaponRumble = MAX(0.0, _weaponRumble - (1024 * delta));
			
			_weaponCoolDown1 = MAX(0.0, _weaponCoolDown1 - delta);
			_weaponCoolDown2 = MAX(0.0, _weaponCoolDown2 - delta);
			
			if(keyShootLeft)
			{
				if(_weaponCoolDown1 <= 0.0 && !_weaponLocked1)
				{
					_weaponCoolDown1 = 0.6f;
					_weaponLocked1   = true;
					_weaponRumble    = 255;
					
					RN::Vector3 position(GetWorldPosition());
					RN::Quaternion rotation(GetWorldRotation());
					
					position += rotation.GetRotatedVector(RN::Vector3(-3.0f, 0.0f, 0.0f));
					
					new Missile(_client->GetClientID(), false, _rigidBody->GetLinearVelocity(), position, rotation);
				}
			}
			else
			{
				_weaponLocked1 = false;
			}
			
			if(keyShootRight)
			{
				if(_weaponCoolDown2 <= 0.0 && !_weaponLocked2)
				{
					_weaponCoolDown2 = 0.6f;
					_weaponLocked2   = true;
					_weaponRumble    = 255;
					
					RN::Vector3 position(GetWorldPosition());
					RN::Quaternion rotation(GetWorldRotation());
					
					position += rotation.GetRotatedVector(RN::Vector3(3.0f, 0.0f, 0.0f));
					
					new Missile(_client->GetClientID(), true, _rigidBody->GetLinearVelocity(), position, rotation);
				}
			}
			else
			{
				_weaponLocked2 = false;
			}
		
		
			RN::Vector3 position = GetWorldPosition();
			RN::Quaternion rotation = GetWorldRotation();
			
			Packet packet(Packet::Type::PositionUpdate);
			
			packet.WriteUInt32(_client->GetClientID());
			packet.WriteFloat(position.x);
			packet.WriteFloat(position.y);
			packet.WriteFloat(position.z);
			
			packet.WriteFloat(rotation.x);
			packet.WriteFloat(rotation.y);
			packet.WriteFloat(rotation.z);
			packet.WriteFloat(rotation.w);
			
			_client->SendPacket(packet);
		}
		
		_damageCooldown = MAX(0.0, _damageCooldown - delta);
		
		rumble += _weaponRumble;
		rumble = MIN(rumble, 255);
		
		if(_damageCooldown > 0.1)
			rumble = 255;
		
		if(_gamepad)
			_gamepad->ExecuteCommand(RNCSTR("rumble"), RN::Number::WithUint8(rumble));
		
		RNDebug("Ship: %u", GetLastFrame());
		
		RN::Entity::Update(delta);
	}
	
/*	void SpaceShip::DidUpdate(ChangeSet changeSet)
	{
		RN::Entity::DidUpdate(changeSet);
		if(changeSet == RN::SceneNode::ChangeSet::Position)
		{
			RN::Vector3 radarOffset(0.0f, 0.3f, -1.8f);
			radarOffset = GetWorldRotation().GetRotatedVector(radarOffset);
			_radar->SetPosition(GetWorldPosition()+radarOffset);
		}
	}*/
}
