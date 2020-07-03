//
//  URExplosion.cpp
//  Ur
//
//  Created by Sidney Just on 20/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URExplosion.h"
#include "URWorld.h"

namespace UR
{
	class ExplosionParticle : public RN::Particle
	{
	public:
		ExplosionParticle(float tlifespan, const RN::Vector3 &velocity) :
			_velocity(velocity),
			_interpolator(1.0, 0.0, tlifespan, RN::Interpolator<float>::Type::CircularEaseIn),
			_totalLifespan(tlifespan)
		{
			lifespan = tlifespan;
		}
		
		void Update(float delta) override
		{
			RN::Particle::Update(delta);
			
			position += _velocity * delta;
			size += 4 * delta;
			color.a = _interpolator.GetValue(_totalLifespan - lifespan);
			
			_velocity = _velocity * 0.2f;
		}
		
	private:
		float _totalLifespan;
		RN::Interpolator<float> _interpolator;
		RN::Vector3 _velocity;
	};
	
	
	class ExplosionEmitter : public RN::ParticleEmitter
	{
	public:
		ExplosionEmitter()
		{
			RN::Material *material = GetMaterial();
			material->AddTexture(RN::Texture::WithName(RNCSTR("Textures/fire.png")));
			material->SetBlendOperation(RN::BlendOperation::Add, RN::BlendOperation::Add);
            material->SetBlendFactorSource(RN::BlendFactor::SourceAlpha, RN::BlendFactor::SourceAlpha);
            material->SetBlendFactorDestination
            (RN::BlendFactor::SourceAlpha, RN::BlendFactor::One);
			
			SetMaxParticles(100);
			SetParticlesPerSecond(60);
		}
		
	protected:
		RN::Particle *CreateParticle() override
		{
			RN::RandomNumberGenerator *generator = GetGenerator();
			RN::Vector3 velocity = generator->GetRandomVector3Range(RN::Vector3(-8), RN::Vector3(8));
			
			ExplosionParticle *particle = new ExplosionParticle(generator->GetRandomFloatRange(1.6, 2.0), velocity);
			
			particle->position = generator->GetRandomVector3Range(RN::Vector3(-5.0f), RN::Vector3(5.0f));
			particle->size = RN::Vector2(generator->GetRandomFloatRange(7.5, 12.8));
			particle->color = RN::Color(1.0f, 0.963, 0.201);
			
			return particle;
		}
	};
	
	
	
	
	class SmokeParticle : public RN::Particle
	{
	public:
		SmokeParticle(float tlifespan, const RN::Vector3 &velocity) :
			_velocity(velocity),
			_totalLifespan(tlifespan),
			_interpolator(1.0, 0.0, tlifespan, RN::Interpolator<float>::Type::CircularEaseIn),
			_colorInterpolator(0.6, 0.15, tlifespan)
		{
			lifespan = tlifespan;
		}
		
		void Update(float delta) override
		{
			RN::Particle::Update(delta);
			
			position += _velocity * delta;
			size += 4 * delta;
			
			color.r = _colorInterpolator.GetValue(_totalLifespan - lifespan);
			color.g = color.r;
			color.b = color.g;
			color.a = _interpolator.GetValue(_totalLifespan - lifespan);
		}
		
	private:
		float _totalLifespan;
		RN::Interpolator<float> _interpolator;
		RN::Interpolator<float> _colorInterpolator;
		RN::Vector3 _velocity;
	};
	
	
	class SmokeEmitter : public RN::ParticleEmitter
	{
	public:
		SmokeEmitter()
		{
			RN::Material *material = GetMaterial();
            material->AddTexture(RN::Texture::WithName(RNCSTR("Textures/smoke.png")));
            material->SetBlendOperation(RN::BlendOperation::Add, RN::BlendOperation::Add);
			
			SetMaxParticles(1000);
			SetParticlesPerSecond(80);
			
			SetIsRenderedInversed(true);
		}
		
	protected:
		RN::Particle *CreateParticle() override
		{
			RN::RandomNumberGenerator *generator = GetGenerator();
			RN::Vector3 velocity = generator->GetRandomVector3Range(RN::Vector3(-3.5), RN::Vector3(3.5));
			
			SmokeParticle *particle = new SmokeParticle(generator->GetRandomFloatRange(1.2, 2.2), velocity);
			
			particle->color = RN::Color(0.4f);
			particle->position = generator->GetRandomVector3Range(RN::Vector3(-8.0f), RN::Vector3(8.0f));
			particle->size = RN::Vector2(generator->GetRandomFloatRange(3.5, 6.8));
			
			return particle;
		}
	};
	
	
	
	Explosion::Explosion() :
		_lifetime(1.9)
	{
		_explosionEmitter = new ExplosionEmitter();
		_smokeEmitter = new SmokeEmitter();
		_light = new RN::Light(RN::Light::Type::PointLight);
		_light->SetColor(RN::Color(1.0f, 0.7f, 0.5f));
		
		AddChild(_explosionEmitter->Autorelease());
		AddChild(_smokeEmitter->Autorelease());
		AddChild(_light->Autorelease());
	}
	
	Explosion *Explosion::WihtPosition(const RN::Vector3 &position)
	{
		Explosion *explosion = new Explosion();
		explosion->SetPosition(position);
        World::GetSharedInstance()->AddNode(explosion->Autorelease());
		
		return explosion;
	}
	
	void Explosion::Update(float delta)
	{
		RN::SceneNode::Update(delta);
		
		_lifetime -= delta;
		
		if(_lifetime <= 1.3)
			_explosionEmitter->SetParticlesPerSecond(0);
		
		if(_lifetime < 0.0)
			_smokeEmitter->SetParticlesPerSecond(0);
		
		_light->SetRange(_lifetime*200.0f);
		
		if(_lifetime <= -2.0)
		{
			World::GetSharedInstance()->RemoveNode(this);
			return;
		}
	}
}
