#include "EclipsePacket.h"

namespace Eclipse
{
	namespace Networking
	{
		void EclipsePacket::Init()
		{
			coreIdentifier = Read<unsigned char>();
			if ((usingSpecializationIdentifier = Read<bool>()))
			{
				specializationIdentifier = Read<unsigned short>();
			}
		}

		EclipsePacket::EclipsePacket(RakNet::Packet* packet)
		{
			m_packet_ = packet;
			// cannot verify if there is a specialization identifier, therefore only coreIdentifier is set here.
			coreIdentifier = packet->data[0];
			stream_ = std::make_shared<RakNet::BitStream>(packet->data, packet->length, true);
		}

		EclipsePacket::EclipsePacket(unsigned char categoryId)
		{
			coreIdentifier = categoryId;
			stream_ = std::make_shared<RakNet::BitStream>();

			Write<unsigned char>(coreIdentifier);
		}

		EclipsePacket::EclipsePacket(unsigned char categoryId, u_short specialId)
		{
			coreIdentifier = categoryId;
			specializationIdentifier = specialId;
			stream_ = std::make_shared<RakNet::BitStream>();

			// write the packet id.
			Write<unsigned char>(coreIdentifier);
			Write <u_short>(specialId);
		}
		EclipsePacket::EclipsePacket(const EclipsePacket& other)
		{
			coreIdentifier = other.coreIdentifier;
			specializationIdentifier = other.specializationIdentifier;
			m_packet_ = other.m_packet_;
			stream_ = other.stream_;
		}
	}
}