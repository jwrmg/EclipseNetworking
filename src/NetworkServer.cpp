#include "NetworkServer.h"
#include <EclipseEngine/include/Core.h>

namespace Eclipse
{
    namespace Networking
    {
        void NetworkServer::Reset() {}

        void NetworkServer::onConnectionLost(NetworkPacket& packet)
        {
            auto addressToRemove = packet.m_packet_->systemAddress;
            if (_removeConnection(addressToRemove))
            {
                External::Debug::DebugAPI::Debug(std::string(addressToRemove.ToString()) + " successfully disconnected.");
            }
            else
            {
                // this should never be called.
                External::Debug::DebugAPI::Error(std::string(addressToRemove.ToString()) + " was never connected or failed to disconnect.");
            }
        }

        void NetworkServer::onIncomingConnection(NetworkPacket& packet)
        {
            auto addressToAdd = packet.m_packet_->systemAddress;
            _addConnection(addressToAdd);
            External::Debug::DebugAPI::Debug(std::string(addressToAdd.ToString()) + " successfully connected");
        }

        void NetworkServer::onIncomingDisconnection(NetworkPacket& packet)
        {
            auto addressToRemove = packet.m_packet_->systemAddress;
            if (_removeConnection(addressToRemove))
            {
                External::Debug::DebugAPI::Debug(std::string(addressToRemove.ToString()) + " successfully disconnected.");
            }
            else
            {
                // this should never be called.
                External::Debug::DebugAPI::Error(std::string(addressToRemove.ToString()) + " was never connected or failed to disconnect.");
            }
        }

        void NetworkServer::SendPacketToAll(NetworkPacket* packet, char orderingChannel, bool broadcast,
            uint32_t forceReceiptNumber, std::vector<RakNet::SystemAddress> filters) const
        {
            for (const RakNet::SystemAddress& client : ConnectedClients)
            {
                for (const RakNet::SystemAddress& filter : filters)
                {
                    if (client != filter)
                        SendPacket(packet, orderingChannel, client, broadcast, forceReceiptNumber);
                }
            }
        }

        void NetworkServer::StartProcess()
        {
            networkManagerType = NetworkManagerType::Server;

            // create socket
            RakNet::SocketDescriptor socket(networkPort, nullptr);

            // initialize event based packet handler.
            handler.InitializeHandler();

            // initialize default handles.
            InitializeDefaultHandles();

            // TODO: Replace with network interface wrapper.
            // set the peer interface.
            interfaceKey = NetworkService::GetInstance()->CreateInterface();
            RakNet::RakPeerInterface* networkInterface = NetworkService::GetInstance()->GetInterface(interfaceKey);

            // begin the server.
            if (!NetworkService::GetInstance()->StartInterface(interfaceKey, maxConnections, &socket, 1))
            {
                External::Debug::DebugAPI::Error("Network Interface failed to start.", __FUNCTION__, __LINE__);
                return;
            }
            External::Debug::DebugAPI::Debug("Successfully started server on port: " + std::to_string(networkPort));
            networkInterface->SetMaximumIncomingConnections(maxConnections);
            //auto result = networkInterface->Startup(maxConnections, &socket, 1);
            //if (result != RakNet::RAKNET_STARTED)
            //{
            //    External::Debug::DebugAPI::Error("Failed to start");
            //    return;
            //}
             // set he maximum client connections.
        }

        void NetworkServer::StopProcess() {}

        bool NetworkServer::_removeConnection(RakNet::SystemAddress address)
        {
            const auto it = std::find(ConnectedClients.begin(), ConnectedClients.end(), address);
            if (it != ConnectedClients.end())
            {
                ConnectedClients.erase(it);
                return true;
            }
            return false;
        }

        void NetworkServer::_addConnection(RakNet::SystemAddress address)
        {
            ConnectedClients.emplace_back(address);
        }

        int32_t NetworkServer::GetConnectionCount()
        {
            return interfaceKey != -1 ? static_cast<int>(NetworkService::GetInstance()->GetConnectionCount(interfaceKey)) : -1;
        }
    }
}