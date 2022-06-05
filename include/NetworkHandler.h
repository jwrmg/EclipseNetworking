#pragma once

#include <functional>
#include <unordered_map>
#include <MessageIdentifiers.h>
#include <EclipseEngine/include/EclipseEvent.h>
#include <EclipseEngine/include/EventCollection.h>
#include "NetworkPacket.h"

namespace Eclipse
{
	namespace Networking
	{
		class NetworkHandler
		{
		public:
			void InitializeHandler();
			void Receive(NetworkPacket& packet);

			Engine::EventCollection<unsigned char, NetworkPacket&> OnPacketReceived;
			Engine::EventCollection<unsigned char, NetworkPacket&> OnPacketSent;
			Engine::EventCollection<unsigned short, NetworkPacket&> OnSpecialPacketReceived;
			Engine::EventCollection<unsigned short, NetworkPacket&> OnSpecialPacketSent;

		private:
			void _handleSpecializedPacket(NetworkPacket& packet);

			std::unordered_map<std::string, unsigned int> namedDynKeys = {};

			friend class INetworkHandle;
		};
	}
}
