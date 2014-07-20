//
//  URExplosion.cpp
//  Ur
//
//  Created by Sidney Just on 20/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URExplosion.h"

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
			
			storage.position += _velocity * delta;
			storage.size += 4 * delta;
			storage.color.a = _interpolator.GetValue(_totalLifespan - lifespan);
			
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
			material->AddTexture(RN::Texture::WithFile("Textures/fire.png"));
			material->SetBlending(true);
			material->SetBlendMode(RN::Material::BlendMode::SourceAlpha, RN::Material::BlendMode::One);
			
			SetMaxParticles(100);
			SetParticlesPerSecond(60);
		}
		
	protected:
		RN::Particle *CreateParticle() override
		{
			RN::RandomNumberGenerator *generator = GetGenerator();
			RN::Vector3 velocity = generator->GetRandomVector3Range(RN::Vector3(-8), RN::Vector3(8));
			
			ExplosionParticle *particle = new ExplosionParticle(generator->GetRandomFloatRange(1.6, 2.0), velocity);
			
			particle->storage.position = generator->GetRandomVector3Range(RN::Vector3(-5.0f), RN::Vector3(5.0f));
			particle->storage.size = RN::Vector2(generator->GetRandomFloatRange(7.5, 12.8));
			particle->storage.color = RN::Color(1.0f, 0.963, 0.201);
			
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
			
			storage.position += _velocity * delta;
			storage.size += 4 * delta;
			
			storage.color.r = _colorInterpolator.GetValue(_totalLifespan - lifespan);
			storage.color.g = storage.color.r;
			storage.color.b = storage.color.g;
			storage.color.a = _interpolator.GetValue(_totalLifespan - lifespan);
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
			material->AddTexture(RN::Texture::WithFile("Textures/smoke.png"));
			material->SetBlending(true);
			
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
			
			particle->storage.color = RN::Color(0.4f);
			particle->storage.position = generator->GetRandomVector3Range(RN::Vector3(-8.0f), RN::Vector3(8.0f));
			particle->storage.size = RN::Vector2(generator->GetRandomFloatRange(3.5, 6.8));
			
			return particle;
		}
	};
	
	
	
	Explosion::Explosion() :
		_lifetime(1.9)
	{
		_explosionEmitter = new ExplosionEmitter();
		_smokeEmitter = new SmokeEmitter();
//		_light = new RN::Light(RN::Light::Type::PointLight);
//		_light->SetColor(RN::Color(1.0f, 0.7f, 0.5f));
		
		AddChild(_explosionEmitter->Autorelease());
		AddChild(_smokeEmitter->Autorelease());
//		AddChild(_light->Autorelease());
	}
	
	Explosion *Explosion::WihtPosition(const RN::Vector3 &position)
	{
		Explosion *explosion = new Explosion();
		explosion->SetPosition(position);
		explosion->Autorelease();
		
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
		
//		_light->SetRange(_lifetime*80.0f);
		
		if(_lifetime <= -2.0)
		{
			RemoveFromWorld();
			return;
		}
	}
}
