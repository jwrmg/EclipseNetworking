#include "NetworkManager.h"

#include "NetworkingBus.h"
#include "EclipseEngine/include/Core.h"

namespace Eclipse
{
    namespace Networking
    {
        NetworkManager* NetworkManager::Instance = nullptr;

        void NetworkManager::onIncomingConnection(EclipsePacket& packet) {}

        void NetworkManager::onIncomingDisconnection(EclipsePacket& packet) {}

        void NetworkManager::onConnectionLost(EclipsePacket& packet) {}

        void NetworkManager::SendPacket(EclipsePacket* packet, char orderingChannel, const RakNet::SystemAddress& systemIdentifier, bool broadcast, uint32_t forceReceiptNumber) const
        {
            NetworkingBus::GetInstance()->SendPacket(new NetworkCall(*packet, interfaceKey, systemIdentifier, forceReceiptNumber, orderingChannel, broadcast));
            //interfaceKey->Send(packet->stream_.get(), packet->priority, packet->reliability, orderingChannel, systemIdentifier, broadcast, forceReceiptNumber);
        }

        void NetworkManager::Update()
        {
            /*
             *	Loop through all incoming packets and correctly handle them.
             */

            RakNet::RakPeerInterface* networkInterface = Networking::NetworkService::GetInstance()->GetInterface(interfaceKey);
            if (!networkInterface)
            {
                External::Debug::DebugAPI::Error("NetworkInterface does not exist.", __FUNCTION__, __LINE__);
                return;
            }

            for (auto packet = networkInterface->Receive(); packet;
                networkInterface->DeallocatePacket(packet),
                packet = networkInterface->Receive())
            {
                handler.Receive(EclipsePacket(packet));
            }
        }

        void NetworkManager::InitializeDefaultHandles()
        {
            // initialize override handles.
            *handler.OnPacketReceived[ID_NEW_INCOMING_CONNECTION] += [this](auto packet) {this->_onIncomingConnection(packet); };
            *handler.OnPacketReceived[ID_DISCONNECTION_NOTIFICATION] += [this](auto packet) {this->_onIncomingDisconnection(packet); };
            *handler.OnPacketReceived[ID_CONNECTION_LOST] += [this](auto packet) {this->_onConnectionLost(packet); };
        }

        void NetworkManager::_onIncomingConnection(EclipsePacket& packet)
        {
            onIncomingConnection(packet);
            OnClientJoin.Invoke(packet);
        }

        void NetworkManager::_onIncomingDisconnection(EclipsePacket& packet)
        {
            onIncomingDisconnection(packet);
            OnClientLeave.Invoke(packet);
        }

        void NetworkManager::_onConnectionLost(EclipsePacket& packet)
        {
            onConnectionLost(packet);
            OnClientLeave.Invoke(packet);
        }

        void NetworkManager::Reset() {}

        void NetworkManager::Deleted()
        {
            delete Instance;
        }

        void NetworkManager::Created()
        {
            if (!Instance)
                Instance = this;
            else delete this;
        }
    }
}