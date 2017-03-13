#pragma once
#include "../AuroraUtility/AuroraDefine.h"
#include "../AuroraNetwork/PacketBase.h"

#include "ChatDefine.h"

using namespace Aurora;

#pragma pack(push, 1)
	class CPacketRegisterReq : public Network::ClientPacket
	{
	public:
		CPacketRegisterReq( void ) :
		ClientPacket( (Int16)ENormalPacketOperation::RegisterReq )
		{

		}

		virtual ~CPacketRegisterReq( void )
		{

		}

		WCHAR accountID[MAX_ACCOUNT_ID_LEN];
		WCHAR password[MAX_PASSWORD_LEN];
	};

	class CPacketRegisterAck : public Network::ServerPacket
	{
	public:
		CPacketRegisterAck( void ) :
		ServerPacket( (Int16)ENormalPacketOperation::RegisterAck ),
		uniqueID( 0 )
		{

		}

		virtual ~CPacketRegisterAck( void )
		{

		}

		UInt32 uniqueID;
	};
#pragma pack(pop)