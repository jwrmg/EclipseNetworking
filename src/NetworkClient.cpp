#include "NetworkClient.h"

#include <EclipseEngine/include/Core.h>

namespace Eclipse
{
    namespace Networking
    {
        void NetworkClient::SendToServer(EclipsePacket* packet, char orderingChannel, uint32_t forceReceiptNumber)
        {
            SendPacket(packet, orderingChannel, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, forceReceiptNumber);
            //interfaceKey->Send(packet->stream_.get(), packet->priority, packet->reliability, orderingChannel, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, forceReceiptNumber);
        }

        void NetworkClient::StartProcess()
        {
            StartProcess(networkAddress);
        }

        void NetworkClient::StopProcess()
        {
            NetworkService::GetInstance()->CloseInterface(interfaceKey, hostServer, true);
        }

        int32_t NetworkClient::GetConnectionCount()
        {
            return interfaceKey != -1 ? static_cast<int>(NetworkService::GetInstance()->GetConnectionCount(interfaceKey)) : -1;
        }

        void NetworkClient::StartProcess(const std::string& uri)
        {
            // create socket.
            RakNet::SocketDescriptor socket;

            // initialize packet handler.
            handler.InitializeHandler();

            // initialize default handles. (optional)
            InitializeDefaultHandles();

            // get peer interface.
            interfaceKey = NetworkService::GetInstance()->CreateInterface();
            if (NetworkService::GetInstance()->StartInterface(interfaceKey, 1, &socket, 1))
            {
                // connect to given address and port.

                if (NetworkService::GetInstance()->ConnectInterface(interfaceKey, uri.c_str(), networkPort, nullptr, 0))
                {
                    hostServer = RakNet::SystemAddress(uri.c_str());
                }
            }
        }

        void NetworkClient::Reset() {}
    }
}