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

#include "URClient.h"

namespace UR
{
	class SpaceShip : public RN::Entity
	{
	public:
		SpaceShip(Client *client);
		
		static SpaceShip *GetLocalShip();
		
		void SetCamera(RN::Camera *camera);
		void SetGamepad(RN::GamepadDevice *gamepad);
		void SetEngineState(uint8 engine, bool working);
		bool GetEngineState(uint8 engine);
		
		float GetSpeed() const;
		int32 GetHealth() const { return _health; }
		
		void TakeHit(RN::Vector3 position);
		
		void Reset();
		void Update(float delta) override;
		
	private:
		RN::Camera *_camera;
		RN::GamepadDevice *_gamepad;
		RN::bullet::RigidBody *_rigidBody;
		
		Client *_client;
		
		float _maxVelocity;
		float _maxThrust;

		bool _weaponLocked1;
		bool _weaponLocked2;
		
		float _weaponCoolDown1;
		float _weaponCoolDown2;
		
		float _damageCooldown;
		float _weaponRumble;

		int32 _health;
		uint8 _engineState;
	};
}

#endif
