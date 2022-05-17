#pragma once

#include <functional>
#include <unordered_map>
#include <MessageIdentifiers.h>
#include <EclipseEngine/include/EclipseEvent.h>
#include <EclipseEngine/include/EventCollection.h>
#include "EclipsePacket.h"

namespace Eclipse
{
	namespace Networking
	{
		class NetworkHandler
		{
		public:
			void InitializeHandler();
			void Receive(EclipsePacket& packet);

			Engine::EventCollection<unsigned char, EclipsePacket&> OnPacketReceived;
			Engine::EventCollection<unsigned char, EclipsePacket&> OnPacketSent;
			Engine::EventCollection<unsigned short, EclipsePacket&> OnSpecialPacketReceived;
			Engine::EventCollection<unsigned short, EclipsePacket&> OnSpecialPacketSent;

		private:
			void _handleSpecializedPacket(EclipsePacket& packet);

			std::unordered_map<std::string, unsigned int> namedDynKeys = {};

			friend class INetworkHandle;
		};
	}
}
