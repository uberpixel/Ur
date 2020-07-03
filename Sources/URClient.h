//
//  URClient.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_CLIENT_H_
#define _UR_CLIENT_H_

#include <Rayne.h>
#include <RNENetClient.h>

#include "URPacket.h"

namespace UR
{
	class Client : public RN::ENetClient
	{
	public:
		Client(RN::uint32 port);
		
		static Client *GetLocalClient();
		
		void SendPacket(Packet &packet);
        void ReceivedPacket(RN::Data *data, RN::uint32 senderID, RN::uint32 channel) override;
        void HandleDidConnect(RN::uint16 userID) override;
        void HandleDidDisconnect(RN::uint16 userID, RN::uint16 data) override;
		
		bool IsConnected() const;
		bool IsReady() const { return (_clientID != 0); }
		
        RN::uint32 GetClientID() const { return _clientID; }
		
	private:
		bool _connected;
        RN::uint32 _clientID;
	};
}

#endif /* _UR_CLIENT_H_ */
