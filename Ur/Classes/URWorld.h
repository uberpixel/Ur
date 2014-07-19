//
//  URWorld.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __UR_WORLD_H__
#define __UR_WORLD_H__

#include <Rayne/Rayne.h>
#include <RBPhysicsWorld.h>

#include "URSpaceShip.h"

namespace UR
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		
	private:
		RN::Camera *_camera;
		RN::GamepadDevice *_gamepad;
		
		RN::bullet::PhysicsWorld *_physicsWorld;
		
		SpaceShip *_ship;
		
		RN::UI::Widget *_hudWidget;
		RN::UI::Label *_speedLabel;
	};
}

#endif /* __UR_WORLD_H__ */
