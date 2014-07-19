//
//  URServer.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_SERVER_H_
#define _UR_SERVER_H_

#include <Rayne/Rayne.h>
#include <enet/enet.h>

#include "URPacket.h"

namespace UR
{
	class Server
	{
	public:
		Server(uint32 port);
		
		void Open();
		void Close();
		
		void Step();
		
	private:
		struct ConnectedClient
		{
			ENetPeer *peer;
			uint32 clientID;
		};
		
		void SendPacketToPeer(Packet &packet, ENetPeer *peer);
		void BroadcastPacket(Packet &packet);
		
		ConnectedClient *GetClientForPeer(ENetPeer *peer);
		ConnectedClient *GetClientWithID(uint32 cid);
		
		uint32 _port;
		ENetHost *_host;
		
		std::vector<ConnectedClient *> _clients;
		
		uint32 _clientIDCount;
	};
}

#endif
