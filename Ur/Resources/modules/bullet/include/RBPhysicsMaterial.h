//
//  RBPhysicsMaterial.h
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

#ifndef __rayne_bullet__RBPhysicsMaterial__
#define __rayne_bullet__RBPhysicsMaterial__

#include <Rayne/Rayne.h>

namespace RN
{
	namespace bullet
	{
		class PhysicsMaterial : public Object
		{
		public:
			PhysicsMaterial();
			
			void SetLinearDamping(float damping);
			void SetAngularDamping(float damping);
			void SetFriction(float friction);
			void SetRestitution(float restitution);
			
			float GetLinearDamping() const { return _linearDamping; }
			float GetAngularDamping() const { return _angularDamping; }
			float GetFriction() const { return _friction; }
			float GetRestitution() const { return _restitution; }
			
			Signal<void (PhysicsMaterial *)> signal;
			
		private:
			float _linearDamping;
			float _angularDamping;
			
			float _friction;
			float _restitution;
			
			RNDeclareMeta(PhysicsMaterial)
		};
	}
}

#endif /* defined(__rayne_bullet__RBPhysicsMaterial__) */
