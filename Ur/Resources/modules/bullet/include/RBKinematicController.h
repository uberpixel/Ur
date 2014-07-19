//
//  RBKinematicController.h
//  rayne-bullet
//
//  Copyright 2013 by Überpixel. All rights reserved.
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef __rayne_bullet__RBKinematicController__
#define __rayne_bullet__RBKinematicController__

#include <Rayne/Rayne.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "RBCollisionObject.h"
#include "RBShape.h"

namespace RN
{
	namespace bullet
	{
		class KinematicController : public CollisionObject
		{
		public:
			KinematicController(Shape *shape, float stepHeight);
			~KinematicController() override;
			
			void SetWalkDirection(const Vector3 &direction);
			void SetFallSpeed(float speed);
			void SetJumpSpeed(float speed);
			void SetMaxJumpHeight(float maxHeight);
			void SetMaxSlope(float maxSlope);
			
			void Update(float delta) override;
			
			bool IsOnGround();
			void Jump();
			
			virtual btCollisionObject *GetBulletCollisionObject();
			
		protected:
			void DidUpdate(SceneNode::ChangeSet changeSet) override;
			void UpdateFromMaterial(PhysicsMaterial *material) override;
			
			void InsertIntoWorld(PhysicsWorld *world) override;
			void RemoveFromWorld(PhysicsWorld *world) override;
			
			Shape *_shape;
			
			btPairCachingGhostObject *_ghost;
			btKinematicCharacterController *_controller;
			
			RNDeclareMeta(KinematicController)
		};
	}
}

#endif /* defined(__rayne_bullet__RBKinematicController__) */
