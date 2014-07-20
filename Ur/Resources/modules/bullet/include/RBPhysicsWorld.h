//
//  RBPhysicsWorld.h
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

#ifndef __rayne_bullet__RBPhysicsWorld__
#define __rayne_bullet__RBPhysicsWorld__

#include <Rayne/Rayne.h>
#include <btBulletDynamicsCommon.h>
#include "RBCollisionObject.h"

namespace RN
{
	namespace bullet
	{
		class PhysicsWorld : public WorldAttachment, public INonConstructingSingleton<PhysicsWorld>
		{
		public:
			PhysicsWorld(const Vector3 &gravity = Vector3(0.0f, -9.81f, 0.0f));
			~PhysicsWorld() override;
			
			void SetGravity(const Vector3 &gravity);
			
			void StepWorld(float delta) override;
			
			Hit CastRay(const Vector3 &from, const Vector3 &to);
			
			void InsertCollisionObject(CollisionObject *attachment);
			void RemoveCollisionObject(CollisionObject *attachment);
			
			btDynamicsWorld *GetBulletDynamicsWorld() { return _dynamicsWorld; }
			
		private:
			btDynamicsWorld *_dynamicsWorld;
			btBroadphaseInterface *_broadphase;
			btCollisionConfiguration *_collisionConfiguration;
			btCollisionDispatcher *_dispatcher;
			btConstraintSolver *_constraintSolver;
			btOverlappingPairCallback *_pairCallback;
			
			static void SimulationStepTickCallback(btDynamicsWorld *world, btScalar timeStep);
			
			std::unordered_set<CollisionObject *> _collisionObjects;
			
			RNDeclareMeta(PhysicsWorld)
			RNDeclareSingleton(PhysicsWorld)
		};
	}
}

#endif /* defined(__rayne_bullet__RBPhysicsWorld__) */
