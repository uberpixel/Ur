//
//  URServer.cpp
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URServer.h"

namespace UR
{
	Server::Server(uint32 port) :
		_port(port),
		_host(nullptr),
		_clientIDCount(0)
	{}
	
	void Server::Open()
	{
		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = _port;
		
		RN_ASSERT((_host = enet_host_create(&address, 32, 2, 0, 0)), "Enet couldn't create server");
	}
	
	void Server::Close()
	{
		if(_host)
			enet_host_destroy(_host);
		
		_host = nullptr;
	}
	
	
	Server::ConnectedClient *Server::GetClientForPeer(ENetPeer *peer)
	{
		auto iterator = std::find_if(_clients.begin(), _clients.end(), [&](ConnectedClient *client) {
			return (client->peer == peer);
		});
		
		return (iterator != _clients.end()) ? *iterator : nullptr;
	}
	
	Server::ConnectedClient *Server::GetClientWithID(uint32 cid)
	{
		auto iterator = std::find_if(_clients.begin(), _clients.end(), [&](ConnectedClient *client) {
			return (client->clientID == cid);
		});
		
		return (iterator != _clients.end()) ? *iterator : nullptr;
	}
	
	
	void Server::SendPacketToPeer(Packet &packet, ENetPeer *peer)
	{
		ENetPacket *enetPacket = enet_packet_create(packet.GetData(), packet.GetLength(), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, enetPacket);
	}
	
	void Server::BroadcastPacket(Packet &packet)
	{
		ENetPacket *enetPacket = enet_packet_create(packet.GetData(), packet.GetLength(), ENET_PACKET_FLAG_RELIABLE);
		enet_host_broadcast(_host, 0, enetPacket);
	}
	
	
	void Server::Step()
	{
		ENetEvent event;
		
		while(enet_host_service(_host, &event, 1) > 0)
		{
			switch(event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
				{
					ConnectedClient *client = new ConnectedClient();
					client->peer = event.peer;
					client->clientID = (++ _clientIDCount);
					
					Packet handshake(Packet::Type::Handshake);
					handshake.WriteUInt32(client->clientID);
					handshake.WriteUInt32(static_cast<uint32>(_clients.size()));
					
					for(ConnectedClient *client : _clients)
					{
						handshake.WriteUInt32(client->clientID);
					}
					
					_clients.push_back(client);
					
					SendPacketToPeer(handshake, client->peer);
					break;
				}
					
				case ENET_EVENT_TYPE_RECEIVE:
				{
					Packet packet(event.packet->data, event.packet->dataLength);
					enet_packet_destroy(event.packet);
					
					switch(packet.GetType())
					{
						case Packet::Type::PositionUpdate:
						case Packet::Type::ShotsFired:
						case Packet::Type::GoodHit:
						case Packet::Type::GoodKill:
							BroadcastPacket(packet);
							break;
							
						default:
							break;
					}
					
					break;
				}
					
				case ENET_EVENT_TYPE_DISCONNECT:
				{
					auto iterator = std::find_if(_clients.begin(), _clients.end(), [&](ConnectedClient *client) {
						return (client->peer == event.peer);
					});
					
					if(iterator != _clients.end())
					{
						ConnectedClient *client = *iterator;
						
						RNDebug("Client %d disconnected", client->clientID);
						
						Packet packet(Packet::Type::ClientDisconnected);
						packet.WriteUInt32(client->clientID);
						
						BroadcastPacket(packet);
						
						delete client;
						_clients.erase(iterator);
					}
					
					break;
				}
					
				default:
					break;
			}
		}
	}
}
