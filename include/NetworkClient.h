#pragma once
#include "NetworkManager.h"

namespace Eclipse
{
	namespace Networking
	{
		class NetworkClient :
			public NetworkManager
		{
		public:
			void SendToServer(const EclipsePacket* packet, char orderingChannel, uint32_t forceReceiptNumber);

			void StartProcess() override;
			void StartProcess(const std::string& uri);
			void StopProcess() override;
		protected:
			void Reset() override;
			int32_t GetConnectionCount() override;
		private:
			RakNet::SystemAddress hostServer = {};
		};
	}
}
