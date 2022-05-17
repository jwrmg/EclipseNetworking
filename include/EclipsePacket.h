#pragma once
#include <RakNetTypes.h>
#include <BitStream.h>
#include <PacketPriority.h>

#include <EclipseEngine/include/EngineMacros.h>

namespace Eclipse
{
	namespace Networking
	{
		class EclipsePacket
		{
		public:
			RakNet::Packet* m_packet_ = nullptr;
			std::shared_ptr<RakNet::BitStream> stream_ = nullptr;

			// allows for 65 535 identifiers.
			unsigned short specializationIdentifier = -1;

			// allows for 255 identifiers.
			unsigned char coreIdentifier = -1;

			// not used
			bool async = false;

			// signifies if a specialization identifier has been used.
			bool usingSpecializationIdentifier{};

			PacketPriority priority = LOW_PRIORITY;
			PacketReliability reliability = RELIABLE;

			void Init();

			template<typename T>
			void Write(T data)
			{
				stream_->Write<T>(data);
			}

			template<typename T>
			void ReadNonAlloc(T& dataBuffer)
			{
				const char* buffer;
				stream_->Read(buffer, sizeof(T));
				dataBuffer = reinterpret_cast<T>(buffer);
			}

			template<typename T>
			T Read()
			{
				T data;
				stream_->Read<T>(data);
				return data;
			}

			EclipsePacket(RakNet::Packet* packet);
			EclipsePacket(unsigned char categoryId);
			EclipsePacket(unsigned char categoryId, u_short specialId);
			EclipsePacket(const EclipsePacket& other);
		};

		template<>
		inline void EclipsePacket::Write<std::string>(const std::string data)
		{
			RakNet::RakString string = data.c_str();
			Write<RakNet::RakString>(string);
		}

		template<>
		inline std::string EclipsePacket::Read<std::string>()
		{
			const RakNet::RakString string = Read<RakNet::RakString>();
			return string.C_String();
		}
	}
}
