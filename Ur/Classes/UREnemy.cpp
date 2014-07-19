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
	Enemy::Enemy(uint32 clientID) :
		_clientID(clientID)
	{
		SetModel(RN::Model::WithFile("Models/Ship/ship_outside.sgm"));
		SetFlags(GetFlags() | RN::SceneNode::Flags::Hidden);
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
