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
	SpaceShip *_localShip = nullptr;
	
	SpaceShip::SpaceShip(Client *client) :
		_camera(nullptr),
		_gamepad(nullptr),
		_client(client),
		_health(150),
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
		
		RN::bullet::PhysicsMaterial *material = new RN::bullet::PhysicsMaterial();
		material->SetLinearDamping(0.25);
		material->SetFriction(0.0);
		material->SetAngularDamping(0.9);
		
		_rigidBody = new RN::bullet::RigidBody(shape, 10.0f);
		_rigidBody->SetMaterial( material);
		
		AddAttachment(_rigidBody);
		
		_localShip = this;
	}
	
	
	SpaceShip *SpaceShip::GetLocalShip()
	{
		return _localShip;
	}
	
	
	void SpaceShip::SetCamera(RN::Camera *camera)
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
	
	void SpaceShip::SetEngineState(uint8 engine, bool working)
	{
		if(working)
			_engineState |= (1 << engine);
		else
			_engineState &= ~(1 << engine);
	}
	
	void SpaceShip::TakeHit(float distance)
	{
		_damageCooldown = 0.8f;
		_health -= 40;
	}
	
	void SpaceShip::Reset()
	{
		_rigidBody->SetLinearVelocity(RN::Vector3());
		_health = 150;
	}

	bool SpaceShip::GetEngineState(uint8 engine)
	{
		return (_engineState & (1 << engine));
	}
	
	void SpaceShip::Update(float delta)
	{
		if(!_gamepad)
			return;
		
		uint8 rumble = 1;
		RN::Quaternion rotation = GetRotation();
		
		// Thrust
		RN::Vector3 velocity = _rigidBody->GetLinearVelocity();
		
		if(velocity.GetLength() > _maxVelocity)
		{
			velocity *= _maxVelocity / velocity.GetLength();
			_rigidBody->SetLinearVelocity(velocity);
		}
		
		float thrust = (_gamepad->GetTrigger1() - _gamepad->GetTrigger2()) * _maxThrust;
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
				rumble = RN::Math::FastAbs(thrust)/_maxThrust*127;
			}
		}
		
		// Rotation
		RN::Vector3 angularVelocity;
	
		angularVelocity.y = -_gamepad->GetAnalog1().x * 0.1f;
		angularVelocity.x = _gamepad->GetAnalog1().y * 0.1f;
		angularVelocity.z = -_gamepad->GetAnalog2().x * 0.1f;
		
		_rigidBody->SetAngularVelocity(rotation.GetRotatedVector(angularVelocity) + _rigidBody->GetAngularVelocity());
		_rigidBody->GetBulletCollisionObject()->activate(true);
		

		// Shooting
		if(_client->IsReady())
		{
			_weaponRumble = MAX(0.0, _weaponRumble - (1024 * delta));
			
			_weaponCoolDown1 = MAX(0.0, _weaponCoolDown1 - delta);
			_weaponCoolDown2 = MAX(0.0, _weaponCoolDown2 - delta);
			
			if(_gamepad->IsButtonPressed(8))
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
			
			if(_gamepad->IsButtonPressed(9))
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
		
		_gamepad->ExecuteCommand(RNCSTR("rumble"), RN::Number::WithUint8(rumble));
		RN::Entity::Update(delta);
	}
}
