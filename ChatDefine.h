#pragma once
#include "../AuroraUtility/AuroraDefine.h"

using namespace Aurora;

const UInt16 MAX_ACCOUNT_ID_LEN = 20 + 1;
const UInt16 MAX_PASSWORD_LEN = 16 + 1;
const UInt16 MAX_NICKNAME_LEN = 12 + 1;
const UInt16 MAX_CHAT_TEXT_LEN = 256 + 1;
const UInt16 MAX_NOTIFY_MESSAGE_LEN = 128 + 1;


enum class ENormalPacketOperation : Int16
{
	RegisterReq = 175,
	RegisterAck,
	LoginReq = 226,
	LoginAck,
	LogoutReq,
	LogoutAck,
	ChatReq,
	ChatAck,
	NormalSendReq,
	NormalSendAck,
	CommonFail,
};

enum class EEventPacketOperation : Int16
{
	WhisperChat,
};

enum class EBroadcastPacketOperation : Int16
{
	LoginUser = 28,
	NormalChat,
	LogoutUser,
};