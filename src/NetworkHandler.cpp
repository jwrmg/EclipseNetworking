#include "NetworkHandler.h"

#include "NetworkIdentifiers.h"

namespace Eclipse
{
	namespace Networking
	{
		void NetworkHandler::InitializeHandler()
		{
			for (unsigned int i = 0; i < (unsigned int)NetworkIdentifiers::EID_USER_PACKET_ENUM; i++)
			{
				OnPacketReceived.emplace(i, new Engine::EclipseEvent<EclipsePacket&>);
				OnPacketSent.emplace(i, new Engine::EclipseEvent<EclipsePacket&>);
			}
		}

		void NetworkHandler::Receive(EclipsePacket& packet)
		{
			// scope to this.
			EclipsePacket pkt = packet;
			// get identifier of the packet.
			int id = pkt.categoryId_;
			// invoke identifiers event.
			OnPacketReceived[id]->Invoke(packet);
		}

		Engine::EclipseEvent<EclipsePacket&>* NetworkHandler::GetReceivedHandle(unsigned id)
		{
			return OnPacketReceived[id];
		}

		Engine::EclipseEvent<EclipsePacket&>* NetworkHandler::GetSentHandle(unsigned id)
		{
			return OnPacketSent[id];
		}

		unsigned NetworkHandler::GenDynID()
		{
			if (_idCount < (int)ID_USER_PACKET_ENUM)
			{
				_idCount = (int)ID_USER_PACKET_ENUM;
			}
			_dynIdCount++;
			return _idCount++;
		}

		unsigned NetworkHandler::GenDynID(const std::string& hashName)
		{
			unsigned int id = GenDynID();
			namedDynKeys.emplace(hashName, id);
			return id;
		}
	}
}