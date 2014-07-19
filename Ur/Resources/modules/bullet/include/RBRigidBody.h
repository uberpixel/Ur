//
//  RBRigidBody.h
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

#ifndef __rayne_bullet__RBRigidBody__
#define __rayne_bullet__RBRigidBody__

#include <Rayne/Rayne.h>
#include "RBCollisionObject.h"
#include "RBShape.h"

namespace RN
{
	namespace bullet
	{
		class RigidBody : public CollisionObject, public btMotionState
		{
		public:
			RigidBody(Shape *shape, float mass);
			RigidBody(Shape *shape, float mass, const Vector3 &inertia);
			
			~RigidBody() override;
			
			static RigidBody *WithShape(Shape *shape, float mass);
			static RigidBody *WithShapeAndInertia(Shape *shape, float mass, const Vector3 &inertia);
			
			void SetMass(float mass);
			void SetMass(float mass, const Vector3 &inertia);
			void SetLinearVelocity(const Vector3 &velocity);
			void SetAngularVelocity(const Vector3 &velocity);
			void SetCCDMotionThreshold(float threshold);
			void SetCCDSweptSphereRadius(float radius);
			
			void ApplyForce(const Vector3 &force);
			void ApplyForce(const Vector3 &force, const Vector3 &origin);
			void ClearForces();
			
			void ApplyTorque(const Vector3 &torque);
			void ApplyTorqueImpulse(const Vector3 &torque);
			void ApplyImpulse(const Vector3 &impulse);
			void ApplyImpulse(const Vector3 &impulse, const Vector3 &origin);
			
			Vector3 GetLinearVelocity() const;
			Vector3 GetAngularVelocity() const;
			
			Vector3 GetCenterOfMass() const;
			Matrix GetCenterOfMassTransform() const;
			
			btCollisionObject *GetBulletCollisionObject() override { return _rigidBody; }
			btRigidBody *GetBulletRigidBody() { return _rigidBody; }
			
		protected:
			void DidUpdate(SceneNode::ChangeSet changeSet) override;
			void UpdateFromMaterial(PhysicsMaterial *material) override;
		
			void InsertIntoWorld(PhysicsWorld *world) override;
			void RemoveFromWorld(PhysicsWorld *world) override;
			
		private:
			void getWorldTransform(btTransform &worldTrans) const override;
			void setWorldTransform(const btTransform &worldTrans) override;
		
			Shape *_shape;
			btRigidBody *_rigidBody;
			
			RNDeclareMeta(RigidBody)
		};
	}
}

#endif /* defined(__rayne_bullet__RBRigidBody__) */
