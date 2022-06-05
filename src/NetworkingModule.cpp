#include "NetworkingModule.h"
#include "NetworkingApi.h"
#include "NetworkingBus.h"
#include "NetworkService.h"

#include "EclipseEngine/include/ApiLoader.h"

namespace Eclipse
{
    namespace Networking
    {
        void NetworkingModule::Created()
        {
        }

        void NetworkingModule::Boot()
        {
            Engine::ApiLoader::Load<NetworkingApi>();
            m_SystemsContainer.AddComponent<NetworkingBus>();
            m_SystemsContainer.AddComponent<NetworkService>();
        }

        void NetworkingModule::Deleted()
        {
        }
    }
}