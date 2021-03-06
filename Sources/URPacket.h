//
//  URPacket.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef _UR_PACKET_H_
#define _UR_PACKET_H_

namespace UR
{
	class Packet
	{
	public:
		enum class Type : RN::uint32
		{
			Handshake,
			PositionUpdate,
			ClientConnected,
			ShotsFired,
			GoodHit,
			GoodKill,
			ClientDisconnected
		};
		
		Packet(Type type) :
			_type(type),
			_offset(0)
		{
			WriteUInt32(static_cast<RN::uint32>(type));
		}
		
		Packet(const void *data, size_t length) :
			_offset(0)
		{
			const RN::uint8 *tdata = (const RN::uint8 *)data;
			
			_stream.insert(_stream.end(), tdata, tdata + length);
			_type = static_cast<Type>(ReadUInt32());
		}
		
		
		
		void WriteSInt8(RN::int8 val)
		{
			Write<RN::int8>(val);
		}
		void WriteUInt8(RN::uint8 val)
		{
			Write<RN::uint8>(val);
		}
		void WriteSInt32(RN::int32 val)
		{
			Write<RN::int32>(val);
		}
		void WriteUInt32(RN::uint32 val)
		{
			Write<RN::uint32>(val);
		}
		void WriteSInt64(RN::int64 val)
		{
			Write<RN::int64>(val);
		}
		void WriteUInt64(RN::uint64 val)
		{
			Write<RN::uint64>(val);
		}
		void WriteFloat(float val)
		{
			Write<float>(val);
		}
		
		
        RN::int8 ReadSInt8()
		{
			return Read<RN::int8>();
		}
        RN::uint8 ReadUInt8()
		{
			return Read<RN::uint8>();
		}
        RN::int32 ReadSInt32()
		{
			return Read<RN::int32>();
		}
        RN::uint32 ReadUInt32()
		{
			return Read<RN::uint32>();
		}
        RN::int64 ReadSInt64()
		{
			return Read<RN::int64>();
		}
        RN::uint64 ReadUInt64()
		{
			return Read<RN::uint64>();
		}
		float ReadFloat()
		{
			return Read<float>();
		}
		
		
		Type GetType() const
		{
			return _type;
		}
		const void *GetData() const
		{
			return _stream.data();
		}
		size_t GetLength()
		{
			return _stream.size();
		}
		
		
	private:
		template<class T>
		T Read()
		{
			T result;
			std::copy(_stream.begin() + _offset, _stream.begin() + _offset + sizeof(T), (RN::uint8 *)&result);
			
			_offset += sizeof(T);
			
			return result;
		}
		
		template<class T>
		void Write(T val)
		{
            RN::uint8 *data = (RN::uint8 *)&val;
			_stream.insert(_stream.end(), data, data + sizeof(T));
		}
		
		std::vector<RN::uint8> _stream;
		size_t _offset;
		
		Type _type;
	};
}

#endif
