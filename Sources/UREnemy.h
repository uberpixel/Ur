//
//  UREnemy.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_ENEMY_H_
#define _UR_ENEMY_H_

#include <Rayne.h>
#include "URPacket.h"

namespace UR
{
	class Enemy : public RN::Entity
	{
	public:
		Enemy(RN::uint32 clientID);
		
		void UpdateFromPacket(Packet &packet);
		
        RN::uint32 GetClientID() const { return _clientID; }
		
	private:
        RN::uint32 _clientID;
		
		RN::Entity *_engineLeft;
		RN::Entity *_engineRight;
		RN::Entity *_engineWingLeft;
		RN::Entity *_engineWingRight;
	};
}

#endif /* _UR_ENEMY_H_ */
