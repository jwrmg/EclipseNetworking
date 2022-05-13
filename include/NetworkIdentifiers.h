#pragma once
#include <MessageIdentifiers.h>

namespace Eclipse
{
	namespace Networking
	{
		enum class NetworkIdentifiers
		{
			EID_MESSAGE = ID_USER_PACKET_ENUM + 1,
			// extend from here.
			EID_USER_PACKET_ENUM,
		};
	}
}