//
//  URMissile.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_MISSILE_H_
#define _UR_MISSILE_H_

#include <Rayne/Rayne.h>
#include <RBRigidBody.h>

#include "URPacket.h"

namespace UR
{
	class Missile : public RN::Entity
	{
	public:
		Missile(uint32 clientID, Packet &packet);
		Missile(uint32 clientID, bool direction, const RN::Vector3 &initalVelocity, const RN::Vector3 &spawnpoint, const RN::Quaternion &rotation);
		
		void Update(float delta) override;
		
	private:
		void Initialize(bool direction, const RN::Vector3 &initalVelocity, const RN::Vector3 &spawnpoint, const RN::Quaternion &rotation);
		
		float _time;
		uint32 _clientID;
		RN::bullet::RigidBody *_rigidBody;
	};
}

#endif
