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
#include "URWorld.h"

namespace UR
{
	Asteroid::Asteroid(RN::Model *model, const RN::Vector3 &position, const RN::Vector3 &scale)
	: RN::Entity(model)
	{
        SetWorldPosition(position);
		SetWorldScale(scale);
		RN::BulletSphereShape *shape = RN::BulletSphereShape::WithRadius(model->GetBoundingSphere().radius*scale.GetMax());
		RN::BulletRigidBody *body = RN::BulletRigidBody::WithShape(shape, 0.0f);
		AddAttachment(body);
        World::GetSharedInstance()->GetPhysicsWorld()->InsertCollisionObject(body);
		
		body->SetContactCallback([&](const RN::BulletCollisionObject *other, const RN::BulletContactInfo &contactInfo){
			if(other->GetParent()->IsKindOfClass(SpaceShip::GetMetaClass()))
			{
				SpaceShip *ship = other->GetParent()->Downcast<SpaceShip>();
				RN::Vector3 dir = GetWorldPosition()-ship->GetWorldPosition();
				if(dir.GetNormalized().GetDotProduct(ship->GetVelocity()) > 20.0f)
					ship->Die();
			}
			
			if(other->GetParent()->IsKindOfClass(Missile::GetMetaClass()))
			{
				__unused Missile *missile = other->GetParent()->Downcast<Missile>();
                missile->Explode();
			}
		});
	}
	
	
}
