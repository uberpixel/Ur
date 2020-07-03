//
//  UREnemy.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "UREnemy.h"

namespace UR
{
	Enemy::Enemy(RN::uint32 clientID) :
		_clientID(clientID)
	{
		SetModel(RN::Model::WithName(RNCSTR("Models/Ship/ship_outside.sgm")));
		SetFlags(GetFlags() | RN::SceneNode::Flags::Hidden);
		
		_engineLeft = new RN::Entity(RN::Model::WithName(RNCSTR("Models/Ship/ship_engine_left.sgm")));
		_engineRight = new RN::Entity(RN::Model::WithName(RNCSTR("Models/Ship/ship_engine_right.sgm")));
		_engineWingLeft = new RN::Entity(RN::Model::WithName(RNCSTR("Models/Ship/ship_engine_leftwing.sgm")));
		_engineWingRight = new RN::Entity(RN::Model::WithName(RNCSTR("Models/Ship/ship_engine_rightwing.sgm")));
		AddChild(_engineLeft);
		AddChild(_engineRight);
		AddChild(_engineWingLeft);
		AddChild(_engineWingRight);
	}
	
	void Enemy::UpdateFromPacket(Packet &packet)
	{
		RN::Vector3 position(packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat());
		RN::Quaternion rotation(packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat());
		
		SetPosition(position);
		SetRotation(rotation);
		
		SetFlags(GetFlags() & ~RN::SceneNode::Flags::Hidden);
	}
}
