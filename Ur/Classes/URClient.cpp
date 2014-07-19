//
//  URClient.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URClient.h"
#include "URWorld.h"
#include "URMissile.h"

namespace UR
{
	static Client *_localClient = nullptr;
	
	Client::Client(uint32 port) :
		_port(port),
		_host(nullptr),
		_peer(nullptr),
		_connected(false),
		_clientID(0)
	{
		_localClient = this;
	}
	
	Client *Client::GetLocalClient()
	{
		return _localClient;
	}
	
	bool Client::Connect(const std::string &ip)
	{
		ENetAddress address;
		ENetEvent event;
		
		enet_address_set_host(&address, ip.c_str());
		address.port = _port;
		
		RN_ASSERT((_host = enet_host_create(NULL, 1, 2, 0, 0)), "Enet couldn't create client!");
		RN_ASSERT((_peer = enet_host_connect(_host, &address, 2, 0)), "Enet couldn't create a peer!");
		
		_connected = (enet_host_service(_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT);
		return _connected;
	}
	
	bool Client::IsConnected() const
	{
		return _connected;
	}
	
	void Client::SendPacket(Packet &packet)
	{
		ENetPacket *enetPacket = enet_packet_create(packet.GetData(), packet.GetLength(), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(_peer, 0, enetPacket);
	}
	
	void Client::Step()
	{
		ENetEvent event;
		
		while(enet_host_service(_host, &event, 1) > 0)
		{
			switch(event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
				{
					Packet packet(event.packet->data, event.packet->dataLength);
					enet_packet_destroy(event.packet);
					
					switch(packet.GetType())
					{
						case Packet::Type::Handshake:
							_clientID = packet.ReadUInt32();
							
							RNDebug("Received handshake. I'm client %d", _clientID);
							break;
							
						case Packet::Type::ClientConnected:
						{
							uint32 client = packet.ReadUInt32();
							
							if(client != _clientID)
							{
								RNDebug("Another client, %d, connected", client);
								World::GetSharedInstance()->GetEnemyWithID(client);
							}
							
							break;
						}
							
						case Packet::Type::PositionUpdate:
						{
							uint32 client = packet.ReadUInt32();
							
							if(client != _clientID)
							{
								Enemy *enemy = World::GetSharedInstance()->GetEnemyWithID(client);
								enemy->UpdateFromPacket(packet);
							}
							
							break;
						}
							
						case Packet::Type::ShotsFired:
						{
							uint32 client = packet.ReadUInt32();
							
							if(client != _clientID)
							{
								new Missile(client, packet);
							}
							
							break;
						}
							
						default:
							break;
					}
					
					break;
				}
					
				case ENET_EVENT_TYPE_DISCONNECT:
					break;
					
				default:
					break;
			}
		}
	}
}
