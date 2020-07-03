//
//  URExplosion.h
//  Ur
//
//  Created by Sidney Just on 20/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_EXPLOSION_H_
#define _UR_EXPLOSION_H_

#include <Rayne.h>

namespace UR
{
	class ExplosionEmitter;
	class SmokeEmitter;
	
	class Explosion : public RN::SceneNode
	{
	public:
		Explosion();
		
		static Explosion *WihtPosition(const RN::Vector3 &position);
		
		void Update(float delta) override;
		
	private:
		float _lifetime;
		ExplosionEmitter *_explosionEmitter;
		SmokeEmitter *_smokeEmitter;
		RN::Light *_light;
	};
}

#endif
