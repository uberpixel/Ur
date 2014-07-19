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
#include "UREnemy.h"
#include "URServer.h"
#include "URClient.h"
#include "URMissile.h"

namespace UR
{
	class World : public RN::World
	{
	public:
		enum class Type
		{
			Client,
			Server
		};
		
		World(Type type);
		~World() override;
		
		static World *GetSharedInstance();
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		
		Enemy *GetEnemyWithID(uint32 clientID);
		
		void ReSpawn();
		
		void AddMissileTracking(Missile *missile);
		void RemoveMissileTracking(Missile *missile);
		
	private:
		void ActivateGamepad(RN::GamepadDevice *gamepad);
		void GenerateAsteroids();
		
		Type _type;
		
		Server *_server;
		Client *_client;
		
		RN::Camera *_camera;
		RN::GamepadDevice *_gamepad;
		
		RN::Random::MersenneTwister _random;
		
		RN::bullet::PhysicsWorld *_physicsWorld;
		
		SpaceShip *_ship;
		
		RN::UI::Widget *_hudWidget;
		RN::UI::Label *_statsLabel;
		
		RN::Array *_enemies;
		RN::Array *_missiles;
	};
}

#endif /* __UR_WORLD_H__ */
