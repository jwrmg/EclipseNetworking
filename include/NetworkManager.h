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
			Engine::EclipseEvent<EclipsePacket&> OnClientJoin = {};
			Engine::EclipseEvent<EclipsePacket&> OnClientLeave = {};
			Engine::EclipseEvent<EclipsePacket&> OnStart = {};
			Engine::EclipseEvent<EclipsePacket&> OnStop = {};

			void SendPacket(EclipsePacket* packet, char orderingChannel, const RakNet::SystemAddress& systemIdentifier, bool broadcast, uint32_t forceReceiptNumber) const;

			virtual void StartProcess() = 0;
			virtual void StopProcess() = 0;

			// overrides
			void Update() final;

			// returns the amount of connected clients to the server.
		protected:
			virtual int32_t GetConnectionCount() = 0;
			virtual void onIncomingConnection(EclipsePacket& packet);
			virtual void onIncomingDisconnection(EclipsePacket& packet);
			virtual void onConnectionLost(EclipsePacket& packet);
			void InitializeDefaultHandles();

		private:
			void _onIncomingConnection(EclipsePacket& packet);
			void _onIncomingDisconnection(EclipsePacket& packet);
			void _onConnectionLost(EclipsePacket& packet);
		protected:
			void Reset() override;
			void Deleted() final;
			void Created() final;
		};
	}
}
