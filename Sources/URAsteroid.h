//
//  URAsteroid.h
//  Ur
//
//  Created by Nils Daumann on 19.07.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_ASTEROID_H_
#define _UR_ASTEROID_H_

#include <Rayne.h>
#include "RNBulletRigidBody.h"

namespace UR
{
	class Asteroid : public RN::Entity
	{
	public:
		Asteroid(RN::Model *model, const RN::Vector3 &position, const RN::Vector3 &scale);
		
	private:
		
	};
}

#endif
