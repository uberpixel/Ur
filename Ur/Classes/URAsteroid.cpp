//
//  URAsteroid.cpp
//  Ur
//
//  Created by Nils Daumann on 19.07.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URAsteroid.h"
#include "URSpaceShip.h"
#include "URMissile.h"

namespace UR
{
	Asteroid::Asteroid(RN::Model *model, const RN::Vector3 &position, const RN::Vector3 &scale)
	: RN::Entity(model, position)
	{
		SetWorldScale(scale);
		RN::bullet::SphereShape *shape = RN::bullet::SphereShape::WithRadius(model->GetBoundingSphere().radius*scale.GetMax());
		RN::bullet::RigidBody *body = RN::bullet::RigidBody::WithShape(shape, 0.0f);
		AddAttachment(body);
		
		body->SetContactCallback([&](RN::bullet::CollisionObject *other){
			if(other->GetParent()->IsKindOfClass(SpaceShip::GetMetaClass()))
			{
				SpaceShip *ship = other->GetParent()->Downcast<SpaceShip>();
				if(ship->GetSpeed() > 20.0f)
					ship->Die();
			}
			
			if(other->GetParent()->IsKindOfClass(Missile::GetMetaClass()))
			{
				Missile *missile = other->GetParent()->Downcast<Missile>();
				
			}
		});
	}
	
	
}
