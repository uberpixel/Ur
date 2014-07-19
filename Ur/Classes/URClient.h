//
//  URClient.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_CLIENT_H_
#define _UR_CLIENT_H_

#include <Rayne/Rayne.h>
#include <enet/enet.h>

#include "URPacket.h"

namespace UR
{
	class Client
	{
	public:
		Client(uint32 port);
		
		void SendPacket(Packet &packet);
		
		bool Connect(const std::string &ip);
		void Step();
		
		bool IsConnected() const;
		bool IsReady() const { return (_clientID != 0); }
		
		uint32 GetClientID() const { return _clientID; }
		
	private:
		uint32 _port;
		
		ENetHost *_host;
		ENetPeer *_peer;
		
		bool _connected;
		uint32 _clientID;
	};
}

#endif /* _UR_CLIENT_H_ */