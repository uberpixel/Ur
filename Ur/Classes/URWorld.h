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
	};
}

#endif /* __UR_WORLD_H__ */
