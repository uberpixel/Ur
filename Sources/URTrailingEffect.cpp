//
//  URTrailingEffect.cpp
//  Ur
//
//  Created by Sidney Just on 20/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URTrailingEffect.h"

namespace UR
{
	class TrailingParticle : public RN::Particle
	{
	public:
		TrailingParticle(float tlifespan) :
			_sizeInterpolator(1.0f, 0.0f, tlifespan),
			_totalLifespan(tlifespan)
		{
			lifespan = tlifespan;
		}
		
		void Update(float delta)
		{
            size = RN::Vector2(_sizeInterpolator.GetValue(_totalLifespan - lifespan));
			color.a = size.x;
			
			RN::Particle::Update(delta);
		}
		
	private:
		float _totalLifespan;
		RN::Interpolator<float> _sizeInterpolator;
	};
	
	
	TrailingEffect::TrailingEffect()
	{
		SetIsLocal(false);
		SetIsRenderedInversed(false);
		
		RN::Material *material = GetMaterial();
		material->AddTexture(RN::Texture::WithName(RNCSTR("Textures/smoke.png")));
		//material->SetBlendOperation(RN::BlendOperation::Add, RN::BlendOperation::Add);
        //material->SetBlendFactorSource(RN::BlendFactor::One, RN::BlendFactor::Zero);
        //material->SetBlendFactorDestination(RN::BlendFactor::One, RN::BlendFactor::One);
		
		SetMaxParticles(20);
		SetParticlesPerSecond(50);
	}
	
	RN::Particle *TrailingEffect::CreateParticle()
	{
		RN::Particle *particle = new TrailingParticle(0.3);
		
		particle->position = GetWorldPosition();
        particle->color = RN::Color::White();//RN::Color(0.063f, 0.932, 1.0f);
		
		return particle;
	}
}
