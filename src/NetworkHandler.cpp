#include "NetworkHandler.h"

#include "NetworkIdentifiers.h"

namespace Eclipse
{
	namespace Networking
	{
		void NetworkHandler::InitializeHandler()
		{
			for (unsigned char i = 0; i < (unsigned char)EID_USER_PACKET_ENUM; i++)
			{
				OnPacketReceived.AddEvent(i, new Engine::EclipseEvent<EclipsePacket&>);
				OnPacketSent.AddEvent(i, new Engine::EclipseEvent<EclipsePacket&>);
			}

			*OnPacketReceived[EID_SPECIALIZATION] += [this](auto packet) {this->_handleSpecializedPacket(packet); };
		}

		void NetworkHandler::Receive(EclipsePacket& packet)
		{
			packet.coreIdentifier = packet.Read<unsigned char>();

			// invoke identifiers event.
			OnPacketReceived[packet.coreIdentifier]->Invoke(packet);
		}

		void NetworkHandler::_handleSpecializedPacket(EclipsePacket& packet)
		{
			packet.specializationIdentifier = packet.Read<u_short>();

			OnSpecialPacketReceived[packet.specializationIdentifier]->Invoke(packet);
		}
	}
}