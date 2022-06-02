#include "NetworkService.h"

#include <EclipseEngine/include/Core.h>
#include <EclipseEngine/include/EngineMacros.h>

namespace Eclipse
{
    namespace Networking
    {
        NetworkService* NetworkService::Instance = nullptr;

        auto NetworkService::CreateInterface() -> InterfaceKey
        {
            if (!INSTANCE_CHECK)
            {
                return -1;
            }

            InterfaceKey key = _networkInterfaces.size() + 1;
            RakNet::RakPeerInterface* networkInterface = RakNet::RakPeerInterface::GetInstance();

            if (networkInterface)
            {
                _networkInterfaces.emplace(key, networkInterface);
                return key;
            }
            return -1;
        }

        auto NetworkService::StartInterface(InterfaceKey key, unsigned int maxConnections, RakNet::SocketDescriptor* socketDescriptors, unsigned socketDescriptorCount, int threadPriority) -> bool
        {
            if (!INSTANCE_CHECK)
            {
                return false;
            }
            RakNet::RakPeerInterface* networkInterface = _networkInterfaces[key];
            if (networkInterface)
            {
                RakNet::StartupResult result = networkInterface->Startup(maxConnections, socketDescriptors, socketDescriptorCount, threadPriority);
                if (result != RakNet::RAKNET_STARTED)
                {
                    External::Debug::DebugAPI::Error("Failed to start interface:\nKEY: " + std::to_string(key) + "\nERROR CODE: " + std::to_string(result), __FUNCTION__, __LINE__);
                    return false;
                }
                return true;
            }
            External::Debug::DebugAPI::Error("Failed to start interface as it does not exist:\nKEY: " + std::to_string(key), __FUNCTION__, __LINE__);
            return false;
        }

        auto NetworkService::CloseInterface(InterfaceKey key, RakNet::SystemAddress host, bool sendNotification) -> bool
        {
            if (!INSTANCE_CHECK)
            {
                return false;
            }

            RakNet::RakPeerInterface* networkInterface = GetInterface(key);
            if (networkInterface)
            {
                networkInterface->CloseConnection(host, sendNotification);
                return true;
            }
            return false;
        }

        auto NetworkService::GetInterface(InterfaceKey key) -> RakNet::RakPeerInterface*
        {
            if (!INSTANCE_CHECK)
            {
                return nullptr;
            }

            return _networkInterfaces[key];
        }

        auto NetworkService::GetConnectionCount(InterfaceKey key) -> unsigned
        {
            if (!GetInstance())
            {
                return -1;
            }

            RakNet::RakPeerInterface* networkInterface = GetInterface(key);
            if (networkInterface)
            {
                return networkInterface->NumberOfConnections();
            }
            return -1;
        }

        auto NetworkService::ConnectInterface(InterfaceKey key, const char* host, unsigned short remotePort,
            const char* passwordData, int passwordDataLength, RakNet::PublicKey* publicKey,
            unsigned connectionSocketIndex, unsigned sendConnectionAttemptCount,
            unsigned timeBetweenSendConnectionAttemptsMS, RakNet::TimeMS timeoutTime) -> bool
        {
            if (!INSTANCE_CHECK)
            {
                return false;
            }

            RakNet::RakPeerInterface* networkInterface = GetInterface(key);
            if (networkInterface)
            {
                RakNet::ConnectionAttemptResult result = networkInterface->Connect(host, remotePort, passwordData, passwordDataLength, publicKey, connectionSocketIndex, sendConnectionAttemptCount, timeBetweenSendConnectionAttemptsMS, timeoutTime);
                if (result != RakNet::CONNECTION_ATTEMPT_STARTED)
                {
                    External::Debug::DebugAPI::Error(
                        "Failed to connect interface:\n\tKEY: " + std::to_string(key) + "\n\tERROR CODE: " + std::to_string(result) + "\n\tIP: " + host + "\n\tPORT:" + std::to_string(remotePort), __FUNCTION__, __LINE__);
                    return false;
                }
                return true;
            }
            return false;
        }

        void NetworkService::Created()
        {
            if (!Instance)
            {
                Instance = this;
            }
            else delete this;
        }

        void NetworkService::Deleted()
        {
            delete Instance;
        }

        NetworkService* NetworkService::GetInstance(const std::string& function)
        {
            std::string func_def = function;
            if(func_def.empty())
            {
                func_def = __FUNCTION__;
            }
            if (!Instance)
            {
                External::Debug::DebugAPI::Error("NetworkService has not been loaded. Did you forget to link the networking module?\n", func_def, __LINE__);
                return nullptr;
            }
            return Instance;
        }
    }
}