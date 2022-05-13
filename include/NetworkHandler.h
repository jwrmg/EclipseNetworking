#pragma once

#include <functional>
#include <unordered_map>
#include <MessageIdentifiers.h>
#include <EclipseEngine/include/EclipseEvent.h>
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

			Engine::EclipseEvent<EclipsePacket&>* GetReceivedHandle(unsigned int);
			Engine::EclipseEvent<EclipsePacket&>* GetSentHandle(unsigned int);

			// TODO: AddNewIDHandle
			//void AddNewIdHandle(unsigned int id);
			//void AddNewIdHandle(unsigned int id, const std::string& name);

			unsigned int GenDynID();
			unsigned int GenDynID(const std::string& hashName);
			std::unordered_map<unsigned int, Engine::EclipseEvent<EclipsePacket&>*> OnPacketReceived = {};
			std::unordered_map<unsigned int, Engine::EclipseEvent<EclipsePacket&>*> OnPacketSent = {};
		private:
			int _dynIdCount = 0;
			unsigned int _idCount = ID_USER_PACKET_ENUM;

			std::unordered_map<std::string, unsigned int> namedDynKeys = {};

			friend class INetworkHandle;
		};
	}
}
