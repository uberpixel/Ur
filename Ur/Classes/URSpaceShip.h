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
#include "URRadar.h"

namespace UR
{
	class SpaceShip : public RN::Entity
	{
	public:
		SpaceShip(Client *client);
		
		static SpaceShip *GetLocalShip();
		
		void SetCamera(RN::SceneNode *camera);
		void SetGamepad(RN::GamepadDevice *gamepad);
		void SetEngineState(uint8 engine, bool working);
		bool GetEngineState(uint8 engine);
		
		float GetSpeed() const;
		RN::Vector3 GetVelocity() const;
		int32 GetHealth() const { return _health; }
		void Die();
		
		uint32 GetKillID() const { return _killID; }
		
		void TakeHit(const RN::Vector3 &position, uint32 clientID);
		
		void Reset();
		void Update(float delta) override;
		
	protected:
//		void DidUpdate(ChangeSet changeSet) override;
		
	private:
		RN::SceneNode *_camera;
		RN::GamepadDevice *_gamepad;
		RN::bullet::RigidBody *_rigidBody;
		Radar *_radar;
		
		Client *_client;
		uint32 _killID;
		
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
		
		RNDeclareMeta(SpaceShip)
	};
}

#endif
