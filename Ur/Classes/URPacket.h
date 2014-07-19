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
		enum class Type : uint32
		{
			Handshake,
			PositionUpdate,
			ClientConnected,
			ShotsFired
		};
		
		Packet(Type type) :
			_type(type),
			_offset(0)
		{
			WriteUInt32(static_cast<uint32>(type));
		}
		
		Packet(const void *data, size_t length) :
			_offset(0)
		{
			const uint8 *tdata = (const uint8 *)data;
			
			_stream.insert(_stream.end(), tdata, tdata + length);
			_type = static_cast<Type>(ReadUInt32());
		}
		
		
		
		void WriteSInt8(int8 val)
		{
			Write<int8>(val);
		}
		void WriteUInt8(uint8 val)
		{
			Write<uint8>(val);
		}
		void WriteSInt32(int32 val)
		{
			Write<int32>(val);
		}
		void WriteUInt32(uint32 val)
		{
			Write<uint32>(val);
		}
		void WriteSInt64(int64 val)
		{
			Write<int64>(val);
		}
		void WriteUInt64(uint64 val)
		{
			Write<uint64>(val);
		}
		void WriteFloat(float val)
		{
			Write<float>(val);
		}
		
		
		int8 ReadSInt8()
		{
			return Read<int8>();
		}
		uint8 ReadUInt8()
		{
			return Read<uint8>();
		}
		int32 ReadSInt32()
		{
			return Read<int32>();
		}
		uint32 ReadUInt32()
		{
			return Read<uint32>();
		}
		int64 ReadSInt64()
		{
			return Read<int64>();
		}
		uint64 ReadUInt64()
		{
			return Read<uint64>();
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
			std::copy(_stream.begin() + _offset, _stream.begin() + _offset + sizeof(T), (uint8 *)&result);
			
			_offset += sizeof(T);
			
			return result;
		}
		
		template<class T>
		void Write(T val)
		{
			uint8 *data = (uint8 *)&val;
			_stream.insert(_stream.end(), data, data + sizeof(T));
		}
		
		std::vector<uint8> _stream;
		size_t _offset;
		
		Type _type;
	};
}

#endif
