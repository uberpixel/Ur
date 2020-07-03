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
	Server::Server(RN::uint32 port) : RN::ENetServer(port)
	{}
	
    void Server::SendPacket(Packet &packet, RN::uint16 userID)
    {
        RN::Data *serializedData = RN::Data::WithBytes(reinterpret_cast<const RN::uint8*>(packet.GetData()), packet.GetLength());
        RN::ENetServer::SendPacket(serializedData, userID, 0, true);
    }

    void Server::BroadcastPacket(Packet &packet)
    {
        RN::Data *serializedData = RN::Data::WithBytes(reinterpret_cast<const RN::uint8*>(packet.GetData()), packet.GetLength());

        for(RN::uint16 id : _activeUserIDs)
        {
           RN::ENetServer::SendPacket(serializedData, id, 0, true);
        }
    }

    void Server::ReceivedPacket(RN::Data *data, RN::uint32 senderID, RN::uint32 channel)
    {
        Packet packet(data->GetBytes(), data->GetLength());
        
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
    }

    void Server::HandleDidConnect(RN::uint16 userID)
    {
        Packet handshake(Packet::Type::Handshake);
        handshake.WriteUInt32(userID);
        handshake.WriteUInt32(static_cast<RN::uint32>(GetNumberOfConnectedUsers()));
        
        for(RN::uint16 otherUserID : _activeUserIDs)
        {
            if(userID != otherUserID)
                handshake.WriteUInt32(otherUserID);
        }
        
        SendPacket(handshake, userID);
    }

    void Server::HandleDidDisconnect(RN::uint16 userID, RN::uint16 data)
    {
        RNDebug("Client " << userID << " disconnected");
        
        Packet packet(Packet::Type::ClientDisconnected);
        packet.WriteUInt32(userID);
        
        BroadcastPacket(packet);
    }
}
