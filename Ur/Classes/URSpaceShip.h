//
//  URSpaceShip.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_SPACESHIP_H_
#define _UR_SPACESHIP_H_

#include <Rayne/Rayne.h>
#include <RBRigidBody.h>

namespace UR
{
	class SpaceShip : public RN::Entity
	{
	public:
		SpaceShip();
		
		void SetCamera(RN::Camera *camera);
		void SetGamepad(RN::GamepadDevice *gamepad);
		
		float GetSpeed() const;
		
		void Update(float delta) override;
		
	private:
		RN::Camera *_camera;
		RN::GamepadDevice *_gamepad;
		RN::bullet::RigidBody *_rigidBody;
		
		RN::Entity *_gizmo;
		
		float _maxVelocity;
		float _maxThrust;
	};
}

#endif