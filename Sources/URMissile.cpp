//
//  URMissile.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URMissile.h"
#include "URClient.h"
#include "URWorld.h"
#include "URExplosion.h"
#include "URTrailingEffect.h"

#define kURMissileAngularVelocity 5.0f
#define kURMissileFlightTime      4.5f

namespace UR
{
	RNDefineMeta(Missile, RN::Entity)
	
	Missile::Missile(RN::uint32 clientID, Packet &packet) :
		_time(kURMissileFlightTime),
		_clientID(clientID)
	{
		bool direction = packet.ReadUInt8();
		
		RN::Vector3 spawnpoint(packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat());
		RN::Quaternion rotation(packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat());
		RN::Vector3 initialVelocity(packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat());
		
		Initialize(direction, initialVelocity, spawnpoint, rotation);
	}
	
	Missile::Missile(RN::uint32 clientID, bool direction, const RN::Vector3 &initialVelocity, const RN::Vector3 &spawnpoint, const RN::Quaternion &rotation) :
		_time(kURMissileFlightTime),
		_clientID(clientID)
	{
		Initialize(direction, initialVelocity, spawnpoint, rotation);
		
		if(Client::GetLocalClient()->GetClientID() == clientID)
		{
			Packet packet(Packet::Type::ShotsFired);
			packet.WriteUInt32(clientID);
			packet.WriteUInt8(direction);
			
			packet.WriteFloat(spawnpoint.x);
			packet.WriteFloat(spawnpoint.y);
			packet.WriteFloat(spawnpoint.z);
			
			packet.WriteFloat(rotation.x);
			packet.WriteFloat(rotation.y);
			packet.WriteFloat(rotation.z);
			packet.WriteFloat(rotation.w);
			
			packet.WriteFloat(initialVelocity.x);
			packet.WriteFloat(initialVelocity.y);
			packet.WriteFloat(initialVelocity.z);
			
			Client::GetLocalClient()->SendPacket(packet);
			World::GetSharedInstance()->AddMissileTracking(this);
		}
	}

    Missile::~Missile()
    {
        World::GetSharedInstance()->GetPhysicsWorld()->RemoveCollisionObject(_rigidBody);
    }
								
	void Missile::Initialize(bool direction, const RN::Vector3 &initialVelocity, const RN::Vector3 &spawnpoint, const RN::Quaternion &rotation)
	{
		SetPosition(spawnpoint);
		SetRotation(rotation);
		
		RN::Model *model = RN::Model::WithName(RNCSTR("Models/Rocket/rocket.sgm"));
		
		_rigidBody = new RN::BulletRigidBody(RN::BulletTriangleMeshShape::WithModel(model), 0.5f);
		_rigidBody->SetAngularVelocity(rotation.GetRotatedVector(RN::Vector3(0.0f, 0.0f, direction ? -5.0f : 5.0f)));
		_rigidBody->SetLinearVelocity(initialVelocity);
		
		SetModel(model);
		AddAttachment(_rigidBody->Autorelease());
        World::GetSharedInstance()->GetPhysicsWorld()->InsertCollisionObject(_rigidBody);
		
		TrailingEffect *effect = new TrailingEffect();
		AddChild(effect->Autorelease());
		
		effect->SetPosition(RN::Vector3(0.0f, 0.0f, 3.0f));
	}

    void Missile::Explode()
    {
        _time = 0.0f;
    }
	
	void Missile::Update(float delta)
	{		
		RN::Entity::Update(delta);
		
		_time -= delta;
		
		if(_time <= 0.0f)
		{
			Explosion::WihtPosition(GetWorldPosition());
            World::GetSharedInstance()->RemoveNode(this);
			World::GetSharedInstance()->RemoveMissileTracking(this);
			
			return;
		}
		
		_rigidBody->ApplyForce(GetRotation().GetRotatedVector(RN::Vector3(0.0f, 0.0f, -45.0f)));
	}
}
