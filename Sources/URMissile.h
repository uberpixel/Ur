//
//  URMissile.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_MISSILE_H_
#define _UR_MISSILE_H_

#include <Rayne.h>
#include <RNBulletRigidBody.h>

#include "URPacket.h"

namespace UR
{
	class Missile : public RN::Entity
	{
	public:
		Missile(RN::uint32 clientID, Packet &packet);
        Missile(RN::uint32 clientID, bool direction, const RN::Vector3 &initalVelocity, const RN::Vector3 &spawnpoint, const RN::Quaternion &rotation);
        ~Missile();
		
		void Update(float delta) override;
        void Explode();
		
	private:
		void Initialize(bool direction, const RN::Vector3 &initalVelocity, const RN::Vector3 &spawnpoint, const RN::Quaternion &rotation);
		
		float _time;
        RN::uint32 _clientID;
		RN::BulletRigidBody *_rigidBody;
		
		RNDeclareMeta(Missile)
	};
}

#endif
