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
#include "URSpaceShip.h"
#include "URExplosion.h"

namespace UR
{
	static Client *_localClient = nullptr;
	
	Client::Client(RN::uint32 port) :
		_connected(false),
		_clientID(0)
	{
		_localClient = this;
	}
	
	Client *Client::GetLocalClient()
	{
		return _localClient;
	}

    void Client::SendPacket(Packet &packet)
    {
        RN::Data *serializedData = RN::Data::WithBytes(reinterpret_cast<const RN::uint8*>(packet.GetData()), packet.GetLength());
        RN::ENetClient::SendPacket(serializedData, 0, 0, true);
    }
	
    void Client::ReceivedPacket(RN::Data *data, RN::uint32 senderID, RN::uint32 channel)
    {
        Packet packet(data->GetBytes(), data->GetLength());
        
        switch(packet.GetType())
        {
            case Packet::Type::Handshake:
                _clientID = packet.ReadUInt32();
                
                RNDebug("Received handshake. I'm client " << _clientID);
                break;
                
            case Packet::Type::ClientConnected:
            {
                RN::uint32 client = packet.ReadUInt32();
                
                if(client != _clientID)
                {
                    RNDebug("Another client, " << client << ", connected");
                    World::GetSharedInstance()->GetEnemyWithID(client);
                }
                
                break;
            }
            case Packet::Type::ClientDisconnected:
            {
                RN::uint32 client = packet.ReadUInt32();
                
                if(client != _clientID)
                {
                    RNDebug("Client " << client << " disconnected");
                    World::GetSharedInstance()->RemoveEnemy(client);
                }
                
                break;
            }
                
                
            case Packet::Type::PositionUpdate:
            {
                RN::uint32 client = packet.ReadUInt32();
                
                if(client != _clientID)
                {
                    Enemy *enemy = World::GetSharedInstance()->GetEnemyWithID(client);
                    enemy->UpdateFromPacket(packet);
                }
                
                break;
            }
                
            case Packet::Type::ShotsFired:
            {
                RN::uint32 client = packet.ReadUInt32();
                
                if(client != _clientID)
                {
                    Missile *missile = new Missile(client, packet);
                    World::GetSharedInstance()->AddNode(missile->Autorelease());
                }
                
                break;
            }
                
            case Packet::Type::GoodHit:
            {
                RN::uint32 client = packet.ReadUInt32();
                RN::uint32 shooter = packet.ReadUInt32();
                RN::Vector3 position(packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat());
                
                if(client == _clientID)
                    SpaceShip::GetLocalShip()->TakeHit(position, shooter);
                
                Explosion *explosion = new Explosion();
                explosion->SetPosition(position);
                World::GetSharedInstance()->AddNode(explosion->Autorelease());
                
                break;
            }
                
            case Packet::Type::GoodKill:
            {
                RN::uint32 client = packet.ReadUInt32();
                RN::uint32 killer = packet.ReadUInt32();
                
                RNDebug("Good kill on " << client << " by " << killer);
                
                if(killer == _clientID)
                    World::GetSharedInstance()->TallyKill();
                
                RN::Vector3 position(packet.ReadFloat(), packet.ReadFloat(), packet.ReadFloat());
                
                Explosion *explosion = new Explosion();
                explosion->SetPosition(position);
                World::GetSharedInstance()->AddNode(explosion->Autorelease());
                
            }
        
            default:
                break;
        }
    }

    void Client::HandleDidConnect(RN::uint16 userID)
    {
        _connected = true;
    }

    void Client::HandleDidDisconnect(RN::uint16 userID, RN::uint16 data)
    {
        _connected = false;
    }
	
	bool Client::IsConnected() const
	{
		return _connected;
	}
}
