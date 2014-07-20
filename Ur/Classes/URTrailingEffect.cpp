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
			storage.size = RN::Vector2(_sizeInterpolator.GetValue(_totalLifespan - lifespan));
			storage.color.a = storage.size.x;
			
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
		material->AddTexture(RN::Texture::WithFile("Textures/smoke.png"));
		material->SetBlending(true);
		material->SetBlendMode(RN::Material::BlendMode::SourceAlpha, RN::Material::BlendMode::One);
		
		SetMaxParticles(20);
		SetParticlesPerSecond(50);
	}
	
	RN::Particle *TrailingEffect::CreateParticle()
	{
		RN::Particle *particle = new TrailingParticle(0.3);
		
		particle->storage.position = GetWorldPosition();
		particle->storage.color = RN::Color(0.063f, 0.932, 1.0f);
		
		return particle;
	}
}
