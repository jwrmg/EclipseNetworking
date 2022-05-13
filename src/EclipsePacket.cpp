#include "EclipsePacket.h"

namespace Eclipse
{
	namespace Networking
	{
		EclipsePacket::EclipsePacket(RakNet::Packet* packet)
		{
			m_packet_ = packet;
			categoryId_ = packet->data[0];

			stream_ = std::make_shared<RakNet::BitStream>(packet->data, packet->length, true);
		}

		EclipsePacket::EclipsePacket(unsigned char categoryId)
		{
			categoryId_ = categoryId;
			stream_ = std::make_shared<RakNet::BitStream>();

			Write<unsigned char>(categoryId_);
			//Write<bool>(false);
		}

		EclipsePacket::EclipsePacket(unsigned char categoryId, u_short specialId)
		{
			categoryId_ = categoryId;
			specialId_ = specialId;
			stream_ = std::make_shared<RakNet::BitStream>();

			Write<unsigned char>(categoryId_);
			//Write<bool>(true);
			//Write<u_short>(specialId_);
		}

		EclipsePacket::EclipsePacket(const EclipsePacket& other)
		{
			categoryId_ = other.categoryId_;
			specialId_ = other.specialId_;
			m_packet_ = other.m_packet_;
			stream_ = other.stream_;

			//stream_.SetData(other.stream_.GetData());
		}
	}
}