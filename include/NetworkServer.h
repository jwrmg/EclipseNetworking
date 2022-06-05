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

            void SendPacketToAll(NetworkPacket* packet, char orderingChannel, bool broadcast, uint32_t forceReceiptNumber, std::vector<RakNet::SystemAddress> filter = {}) const;

            void StartProcess() override;
            void StopProcess() override;
        protected:
            void Reset() override;
            void onConnectionLost(NetworkPacket& packet) override;
            void onIncomingConnection(NetworkPacket& packet) override;
            void onIncomingDisconnection(NetworkPacket& packet) override;
            int32_t GetConnectionCount() override;
        private:
            bool _removeConnection(RakNet::SystemAddress address);
            void _addConnection(RakNet::SystemAddress address);
            std::vector<RakNet::SystemAddress> ConnectedClients = {};
        };
    }
}
