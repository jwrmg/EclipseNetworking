#pragma once
#include <queue>
#include <EclipseEngine/include/System.h>
#include <EclipseEngine/include/EclipseTimer.h>
#include <RakPeerInterface.h>

#include "NetworkPacket.h"
#include "NetworkService.h"

namespace Eclipse
{
    namespace Networking
    {
        struct NetworkCall
        {
            NetworkCall(const NetworkPacket& packet, InterfaceKey senderKey,
                const RakNet::SystemAddress& address, uint32_t force_receipt_number, char ordering_channel,
                bool broadcast);

            // data to be sent
            NetworkPacket packet;
            // network sender.
            InterfaceKey sender = -1;
            // recipient address
            const RakNet::SystemAddress& address;
            uint32_t forceReceiptNumber;
            char orderingChannel;
            bool broadcast;
        };

        class NetworkingBus :
            public Engine::System
        {
        public:
            void Created() override;
            void Deleted() override;
            void Update() override;

            /**
             * \brief This will immediately send the network call to the recipient.
             * \param call network call to be sent.
             */
            void SendPacketImmediate(NetworkCall* call);
            /**
             * \brief This will buffer the call to be sent at a later point in the frame.
             * \param call network call to be sent
             */
            void SendPacket(NetworkCall* call);

            // default = 100ms. or 0.1s.
            void SetSentPacketFrequency(float frequencyInMs);

            static NetworkingBus* GetInstance(const std::string& function = "");
        private:
            static NetworkingBus* Instance;
            /**
             * \brief This will send all buffered packets.
             */
            void SendBufferedPackets();
            void _sendPacket(NetworkPacket* packet, InterfaceKey senderKey, char orderingChannel,
                const RakNet::SystemAddress& systemIdentifier, bool broadcast, uint32_t forceReceiptNumber);
            std::queue<NetworkCall*> networkBuffer = {};
            int packetsSentLastFrame = 0;
            int packetSentDelta = 0;

            float sentFrequency = 1.0f;
            Engine::EclipseTimer networkTimer = sentFrequency;
        };
    }
}
