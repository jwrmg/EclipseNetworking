#pragma once

#include <unordered_map>
#include <EclipseEngine/include/System.h>

// raknet
#include "RakPeerInterface.h"

namespace Eclipse
{
    namespace Networking
    {
        typedef unsigned int InterfaceKey;

        class NetworkService : public Engine::System
        {
        public:

            auto CreateInterface()->InterfaceKey;
            auto StartInterface(InterfaceKey key, unsigned int maxConnections, RakNet::SocketDescriptor* socketDescriptors, unsigned socketDescriptorCount, int threadPriority = -99999) -> bool;
            auto CloseInterface(InterfaceKey key, RakNet::SystemAddress, bool) -> bool;
            auto GetInterface(InterfaceKey key)->RakNet::RakPeerInterface*;
            auto GetConnectionCount(InterfaceKey key) -> unsigned;
            auto ConnectInterface(InterfaceKey key, const char* host, unsigned short remotePort, const char* passwordData, int passwordDataLength, RakNet::PublicKey* publicKey = 0, unsigned connectionSocketIndex = 0, unsigned sendConnectionAttemptCount = 12, unsigned timeBetweenSendConnectionAttemptsMS = 500, RakNet::TimeMS timeoutTime = 0) -> bool;

            void Created() override;
            void Deleted() override;

            static NetworkService* GetInstance(const std::string& function = "");
        private:
            static NetworkService* Instance;
            std::unordered_map<InterfaceKey, RakNet::RakPeerInterface*> _networkInterfaces = {};
        };
    }
}
