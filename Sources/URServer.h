//
//  URServer.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_SERVER_H_
#define _UR_SERVER_H_

#include <Rayne.h>
#include <RNENetServer.h>

#include "URPacket.h"

namespace UR
{
	class Server : public RN::ENetServer
	{
	public:
		Server(RN::uint32 port);
        
        void SendPacket(Packet &packet, RN::uint16 userID);
        void BroadcastPacket(Packet &packet);
        
        void ReceivedPacket(RN::Data *data, RN::uint32 senderID, RN::uint32 channel) override;
        void HandleDidConnect(RN::uint16 userID) override;
        void HandleDidDisconnect(RN::uint16 userID, RN::uint16 data) override;
		
	private:
        
	};
}

#endif
