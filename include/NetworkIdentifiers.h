#pragma once
#include <MessageIdentifiers.h>

namespace Eclipse
{
	namespace Networking
	{
		enum NetworkIdentifiers
		{
			EID_MESSAGE = ID_USER_PACKET_ENUM + 1,
			// use this to signify that there is a special id.
			// and use the special id for the handler.
			EID_SPECIALIZATION,
			// extend from here.
			EID_USER_PACKET_ENUM,
		};

		enum SpecialNetworkIdentifier : unsigned short
		{
			// WARNING: YOU MUST EXTEND FROM HERE
			// USAGE:
			/*
			 *	enum class yourEnum : unsigned short
			 *	{
			 *		yourEnumName = SPEC_USER_PACKET_ENUM;
			 *	}
			 */
			SPEC_USER_PACKET_ENUM,
		};
	}
}