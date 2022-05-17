#pragma once
#include "NetworkManager.h"

namespace Eclipse
{
	namespace Networking
	{
		class NetworkServer :
			public NetworkManager
		{
		public:

			unsigned short maxConnections = 32;

			void SendPacketToAll(const EclipsePacket* packet, char orderingChannel, bool broadcast, uint32_t forceReceiptNumber) const;
			void SendPacketToAll(const EclipsePacket* packet, char orderingChannel, bool broadcast, uint32_t forceReceiptNumber, std::vector<RakNet::SystemAddress> filter) const;
			
			void StartProcess() override;
			void StopProcess() override;
		protected:
			void Reset() override;
			void onConnectionLost(EclipsePacket& packet) override;
			void onIncomingConnection(EclipsePacket& packet) override;
			void onIncomingDisconnection(EclipsePacket& packet) override;
			int32_t GetConnectionCount() override;
		private:
			bool _removeConnection(RakNet::SystemAddress address);
			void _addConnection(RakNet::SystemAddress address);
			std::vector<RakNet::SystemAddress> ConnectedClients = {};
		};
	}
}
