#include <EclipseEngine/include/Core.h>
#include "NetworkingBus.h"

namespace Eclipse
{
    namespace Networking
    {
        NetworkingBus* NetworkingBus::Instance = nullptr;

        NetworkCall::NetworkCall(const EclipsePacket& packet, InterfaceKey senderKey,
            const RakNet::SystemAddress& address, uint32_t forceReceiptNumber, char orderingChannel, bool broadcast) : packet(packet), sender(senderKey), address(address), forceReceiptNumber(forceReceiptNumber), orderingChannel(orderingChannel), broadcast(broadcast)
        {
        }

        void NetworkingBus::Created()
        {
            if (!Instance)
                Instance = this;

            networkTimer = sentFrequency;
        }
        void NetworkingBus::Deleted()
        {
            delete Instance;
        }

        void NetworkingBus::Update()
        {
            if (!INSTANCE_CHECK)
            {
                return;
            }
            networkTimer.Step();

            if (networkTimer.Ready())
                SendBufferedPackets();
        }

        void NetworkingBus::SendPacketImmediate(NetworkCall* call)
        {
            if (!INSTANCE_CHECK)
            {
                return;
            }
            if (!call->sender)
            {
                External::Debug::DebugAPI::Debug("Could not immediately send network call.");
                return;
            }

            _sendPacket(&call->packet, call->sender, call->orderingChannel, call->address, call->broadcast, call->forceReceiptNumber);
        }

        void NetworkingBus::SendPacket(NetworkCall* call)
        {
            if (!INSTANCE_CHECK)
            {
                return;
            }
            if (!call->sender || !call)
            {
                External::Debug::DebugAPI::Debug("Could not buffer network call.");
                return;
            }
            networkBuffer.push(call);
        }

        void NetworkingBus::SetSentPacketFrequency(float frequencyInMs)
        {
            if (!INSTANCE_CHECK)
            {
                return;
            }
            networkTimer.SetDelay(frequencyInMs);
        }

        NetworkingBus* NetworkingBus::GetInstance(const std::string& function)
        {
            std::string func_def = function;
            if (func_def.empty())
            {
                func_def = __FUNCTION__;
            }
            if (!Instance)
            {
                External::Debug::DebugAPI::Error("NetworkingBus has not been loaded. Did you forget to link the networking module?\n", func_def, __LINE__);
                return nullptr;
            }
            return Instance;
        }

        void NetworkingBus::SendBufferedPackets()
        {
            if (!INSTANCE_CHECK)
            {
                return;
            }

            int sentPackets = 0;

            while (!networkBuffer.empty())
            {
                // get call at the front of the queue
                NetworkCall* call = networkBuffer.front();

                // check if the calls sender still exists.
                if (!call->sender || !call)
                {
                    External::Debug::DebugAPI::Debug(("Sender on iteration " + std::to_string(sentPackets) + "no longer exists.").c_str());
                    networkBuffer.pop();
                    continue;
                }

                // send the packet
                _sendPacket(&call->packet, call->sender, call->orderingChannel, call->address, call->broadcast, call->forceReceiptNumber);

                // remove call from network buffer.
                networkBuffer.pop();
                sentPackets++;
            }

            packetSentDelta = packetsSentLastFrame - sentPackets;
            packetsSentLastFrame = sentPackets;
        }

        void NetworkingBus::_sendPacket(EclipsePacket* packet, InterfaceKey senderKey, char orderingChannel,
            const RakNet::SystemAddress& systemIdentifier, bool broadcast, uint32_t forceReceiptNumber)
        {
            if (!INSTANCE_CHECK)
            {
                return;
            }
            RakNet::RakPeerInterface* sender = NetworkService::GetInstance()->GetInterface(senderKey);
            if (sender)
            {
                sender->Send(packet->stream_.get(), packet->priority, packet->reliability, orderingChannel, systemIdentifier, broadcast, forceReceiptNumber);
            }
        }
    }
}