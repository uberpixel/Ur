//
//  URTrailingEffect.h
//  Ur
//
//  Created by Sidney Just on 20/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_TRAILINGEFFECT_H_
#define _UR_TRAILINGEFFECT_H_
#include <Rayne.h>

namespace UR
{
	class TrailingEffect : public RN::ParticleEmitter
	{
	public:
		TrailingEffect();
		
	protected:
		RN::Particle *CreateParticle() override;
	};
}

#endif

// [UIColor colorWithRed:0.063 green:0.932 blue:1.000 alpha:1.000]
