//
//  URSpaceShip.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URSpaceShip.h"

namespace UR
{
	SpaceShip::SpaceShip(Client *client) :
		_camera(nullptr),
		_gamepad(nullptr),
		_client(client),
		_maxThrust(0.5),
		_maxVelocity(5.0)
	{
		RN::Model *model = RN::Model::WithFile("Models/Ship/ship_outside.sgm");
		
		RN::bullet::Shape *shape = RN::bullet::TriangleMeshShape::WithModel(model);
		
		SetModel(RN::Model::WithFile("Models/Ship/ship_inside.sgm"));
		
		RN::bullet::PhysicsMaterial *material = new RN::bullet::PhysicsMaterial();
		material->SetLinearDamping(0.0);
		material->SetFriction(0.0);
		material->SetAngularDamping(0.0);
		
		_rigidBody = new RN::bullet::RigidBody(shape, 10.0f);
		_rigidBody->SetMaterial( material);
		
		AddAttachment(_rigidBody);
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
			RN::Vector3 vector(0.0f, 0.0f, thrust);
			_rigidBody->ApplyImpulse(rotation.GetRotatedVector(vector));
			rumble = RN::Math::FastAbs(thrust)/_maxThrust*127;
		}
		
		// Rotation
		RN::Vector3 angularVelocity;
	
		angularVelocity.z = -_gamepad->GetAnalog1().x;
		angularVelocity.x = _gamepad->GetAnalog1().y;
		
		
		_rigidBody->SetAngularVelocity(rotation.GetRotatedVector(angularVelocity));
		_rigidBody->GetBulletCollisionObject()->activate(true);
		
		_gamepad->ExecuteCommand(RNCSTR("rumble"), RN::Number::WithUint8(rumble));

		RN::Entity::Update(delta);
		
		if(_client->IsReady())
		{
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
	}
}
