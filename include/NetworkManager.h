#pragma once
#include <EclipseEngine/include/Component.h>
#include <EclipseEngine/include/EclipseEvent.h>
#include <EclipseEngine/include/EventCollection.h>

#include <RakPeerInterface.h>

#include "NetworkHandler.h"
#include "NetworkService.h"

namespace Eclipse
{
	namespace Networking
	{
		enum class NetworkManagerType
		{
			Client,
			Host,
			Offline,
			Server
		};

		class NetworkManager :
			public Components::Component
		{
		public:
			// singleton instance.
			static NetworkManager* Instance;

			// Default Types
			bool isNetworkActive = false;
			std::string networkAddress = "localhost";
			unsigned short networkPort = 0;

			// Eclipse Types
			NetworkManagerType networkManagerType = NetworkManagerType::Offline;
			NetworkHandler handler;

			// RakNet Types
			InterfaceKey interfaceKey = -1;

			// Eclipse Events
			Engine::EclipseEvent<NetworkPacket&> OnClientJoin = {};
			Engine::EclipseEvent<NetworkPacket&> OnClientLeave = {};
			Engine::EclipseEvent<NetworkPacket&> OnStart = {};
			Engine::EclipseEvent<NetworkPacket&> OnStop = {};

			void SendPacket(NetworkPacket* packet, char orderingChannel, const RakNet::SystemAddress& systemIdentifier, bool broadcast, uint32_t forceReceiptNumber) const;

			virtual void StartProcess() = 0;
			virtual void StopProcess() = 0;

			// overrides
			void Update() final;

			// returns the amount of connected clients to the server.
		protected:
			virtual int32_t GetConnectionCount() = 0;
			virtual void onIncomingConnection(NetworkPacket& packet);
			virtual void onIncomingDisconnection(NetworkPacket& packet);
			virtual void onConnectionLost(NetworkPacket& packet);
			void InitializeDefaultHandles();

		private:
			void _onIncomingConnection(NetworkPacket& packet);
			void _onIncomingDisconnection(NetworkPacket& packet);
			void _onConnectionLost(NetworkPacket& packet);
		protected:
			void Reset() override;
			void Deleted() final;
			void Created() final;
		};
	}
}
