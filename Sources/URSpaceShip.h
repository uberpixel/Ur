//
//  URSpaceShip.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_SPACESHIP_H_
#define _UR_SPACESHIP_H_

#include <Rayne.h>
#include <RNBulletRigidBody.h>

#include "URClient.h"
#include "URRadar.h"

namespace UR
{
	class SpaceShip : public RN::Entity
	{
	public:
		SpaceShip(Client *client);
        ~SpaceShip();
		
		static SpaceShip *GetLocalShip();
		
		void SetCamera(RN::SceneNode *camera);
//		void SetGamepad(RN::GamepadDevice *gamepad);
		void SetEngineState(RN::uint8 engine, bool working);
		bool GetEngineState(RN::uint8 engine);
		
		float GetSpeed() const;
		RN::Vector3 GetVelocity() const;
        RN::int32 GetHealth() const { return _health; }
		void Die();
		
        RN::uint32 GetKillID() const { return _killID; }
		
		void TakeHit(const RN::Vector3 &position, RN::uint32 clientID);
		
		void Reset();
		void Update(float delta) override;
		
	protected:
//		void DidUpdate(ChangeSet changeSet) override;
		
	private:
		RN::SceneNode *_camera;
//		RN::GamepadDevice *_gamepad;
		RN::BulletRigidBody *_rigidBody;
		Radar *_radar;
		
		Client *_client;
        RN::uint32 _killID;
		
		float _maxVelocity;
		float _maxThrust;

		bool _weaponLocked1;
		bool _weaponLocked2;
		
		float _weaponCoolDown1;
		float _weaponCoolDown2;
		
		float _damageCooldown;
		float _weaponRumble;

        RN::int32 _health;
        RN::uint8 _engineState;
		
		RNDeclareMeta(SpaceShip)
	};
}

#endif
