//
//  RBCollisionObject.h
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

#ifndef __rayne_bullet__RBCollisionObject__
#define __rayne_bullet__RBCollisionObject__

#include <Rayne/Rayne.h>
#include <btBulletDynamicsCommon.h>
#include "RBPhysicsMaterial.h"

namespace RN
{
	namespace bullet
	{
		class PhysicsWorld;
		
		class CollisionObject : public SceneNodeAttachment
		{
		public:
			friend class PhysicsWorld;
			
			CollisionObject();
			~CollisionObject() override;
			
			void SetCollisionFilter(short int filter);
			void SetCollisionFilterMask(short int mask);
			void SetMaterial(PhysicsMaterial *material);
			void SetContactCallback(std::function<void(CollisionObject *)> &&callback);
			
			short int GetCollisionFilter() const { return _collisionFilter; }
			short int GetCollisionFilterMask() const { return _collisionFilterMask; }
			PhysicsMaterial *GetMaterial() const { return _material; }
			
			virtual btCollisionObject *GetBulletCollisionObject() = 0;
			
		protected:
			void DidUpdate(SceneNode::ChangeSet changeSet) override;
			void DidAddToParent() override;
			void WillRemoveFromParent() override;
			
			void ReInsertIntoWorld();
			virtual void UpdateFromMaterial(PhysicsMaterial *material) = 0;
			virtual void InsertIntoWorld(PhysicsWorld *world);
			virtual void RemoveFromWorld(PhysicsWorld *world);
			
			Vector3 offset;
			
		private:
			Connection *_connection;
			PhysicsWorld *_owner;
			PhysicsMaterial *_material;
			
			std::function<void(CollisionObject *)> _callback;
			
			short int _collisionFilter;
			short int _collisionFilterMask;
			
			RNDeclareMeta(CollisionObject)
		};
	}
}

#endif /* defined(__rayne_bullet__RBCollisionObject__) */
