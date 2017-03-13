// ChatClient.cpp : Defines the entry point for the console application.
//
#pragma once

#include "../AuroraUtility/AuroraDefine.h"
#include "../AuroraUtility/StringManager.h"
#include "../AuroraUtility/MiscManager.h"
#include "../AuroraUtility/ThreadManager.h"

#include "../AuroraNetwork/WinsockManager.h"
#include "../AuroraNetwork/NetworkManager.h"

#include "ChatDefine.h"
#include "ChatPacketDefine.h"
#include "ChatPacketProcessor.h"

using namespace Aurora;
using namespace Aurora::Network;

static bool ConnectedServer = false;
static bool LoggedinServer = false;

static UInt32 UserAccountUniqueID;
static WCHAR ChatUserAccountID[MAX_ACCOUNT_ID_LEN];

static unsigned int __stdcall UserInput( void *pArgs )
{
	pArgs;

	char sendBuffer[NORMAL_BUFFER_LEN] = {0};
	char recvBuffer[NORMAL_BUFFER_LEN] ={0};

	bool TrySend = false;
	int SendResult = 0;
	WCHAR recvInput[MAX_CHAT_TEXT_LEN] ={0};

	printf( "------------------- Command Information ----------------------------\n" );
	printf( "------------------- 1. register ------------------------------------\n" );
	printf( "------------------- 2. login ---------------------------------------\n" );
	printf( "------------------- 3. logout --------------------------------------\n" );
	printf( "------------------- 4. whisper -------------------------------------\n" );
	printf( "------------------- 5. info ----------------------------------------\n" );
	printf( "------------------- Command Information ----------------------------\n" );

	while ( true )
	{
		bool RecvPacket = false;

		memset( sendBuffer, 0, NORMAL_BUFFER_LEN );
		memset( recvInput, 0, MAX_CHAT_TEXT_LEN );

		printf( "Input> " );

		_getws_s( recvInput );
		if( 0 >= wcslen( recvInput ) )
		{
			continue;
		}

		WCHAR* pExit = wcsstr( recvInput, L"exit" );
		if( nullptr != pExit )
		{
			//ChatPacketProcessor::Instance()->StopParsePacketThread();
			AuroraWinsockManager->StopEventSelectClient();

			// wait thread에서 infinite로 대기하고 있으므로 스레드가 바로 안 끝남.
			// logout 패킷을 보내서 받게 되면 스레드가 깨어나므로 자동 종료됨.
			//AuroraNetworkManager->
			/*AuroraNetwork::CPacketLogoutReq PacketLogoutReq;
			PacketLogoutReq.AccountUniqueID = ChatPacketProcessor::Instance()->GetUniqueID();
			PacketLogoutReq.CalculateSize();*/

			SendResult = 0;
			//TrySend = AuroraNetworkManager->SendToServer( (const char *)&PacketLogoutReq, PacketLogoutReq.GetSize(), SendResult );

			printf( "exit Event, exit program.\n" );
			return 0;
		}

		wchar_t *pInfo = wcsstr( recvInput, L"info" );
		if( nullptr != pInfo )
		{
			printf( "------------------- Command Information ----------------------------\n" );
			printf( "------------------- 1. register ------------------------------------\n" );
			printf( "------------------- 2. login ---------------------------------------\n" );
			printf( "------------------- 3. logout --------------------------------------\n" );
			printf( "------------------- 4. whisper -------------------------------------\n" );
			printf( "------------------- 5. info ----------------------------------------\n" );
			printf( "------------------- Command Information ----------------------------\n" );
			continue;
		}

		wchar_t *pRegister = wcsstr( recvInput, L"register" );
		if( nullptr != pRegister )
		{
			CPacketRegisterReq PacketRegisterReq;

			printf( "AccountID: " );
			_getws_s( PacketRegisterReq.accountID );

			printf( "Password: " );
			_getws_s( PacketRegisterReq.password );
			PacketRegisterReq.CalculateSize();

			SendResult = 0;
			TrySend = AuroraNetworkManager->SendToServer( (const char *)&PacketRegisterReq, 
														  PacketRegisterReq.GetSize(), 
														  OUT SendResult );
			if( false == TrySend )
			{
				continue;
			}

			if( 0 < SendResult )
			{
				RecvPacket = true;
			}

			continue;
		}

		/*pRegister = wcsstr( recvInput, L"login" );
		if( nullptr != pRegister )
		{
			AuroraNetwork::CPacketLoginReq PacketLoginReq;

			printf( "AccountID: " );
			_getws_s( PacketLoginReq.AccountID );

			printf( "Password: " );
			_getws_s( PacketLoginReq.Password );
			PacketLoginReq.CalculateSize();

			SendResult = 0;
			TrySend = pNetworkManager->SendToServer( (const char *)&PacketLoginReq, PacketLoginReq.GetSize(), SendResult );
			if( false == TrySend )
			{
				continue;
			}

			if( 0 < SendResult )
			{
				RecvPacket = true;
			}
			continue;
		}

		pRegister = wcsstr( recvInput, L"logout" );
		if( nullptr != pRegister )
		{
			AuroraNetwork::CPacketLogoutReq PacketLogoutReq;
			PacketLogoutReq.AccountUniqueID = ChatPacketProcessor::Instance()->GetUniqueID();
			PacketLogoutReq.CalculateSize();

			SendResult = 0;
			TrySend = pNetworkManager->SendToServer( (const char *)&PacketLogoutReq, PacketLogoutReq.GetSize(), SendResult );
			if( false == TrySend )
			{
				continue;
			}

			if( 0 < SendResult )
			{
				RecvPacket = true;
			}
			continue;
		}

		pRegister = wcsstr( recvInput, L"whisper" );
		if( nullptr != pRegister )
		{
			AuroraNetwork::CPacketWhisperChatReq PacketWhisperChatReq;
			
			printf( "Target User AccountID: " );
			_getws_s( PacketWhisperChatReq.m_TargetAccountID );

			printf( "Message: " );
			_getws_s( PacketWhisperChatReq.m_ChatText );

			CCstringManager::Instance()->MemsetAndCopyStringW( PacketWhisperChatReq.m_SenderAccountID, ChatPacketProcessor::Instance()->GetAccountID() );
			PacketWhisperChatReq.CalculateSize();

			SendResult = 0;
			TrySend = pNetworkManager->SendToServer( (const char *)&PacketWhisperChatReq, PacketWhisperChatReq.GetSize(), SendResult );
			if( false == TrySend )
			{
				continue;
			}

			if( 0 < SendResult )
			{
				RecvPacket = true;
			}
			continue;
		}
*/
		if( false == ChatPacketProcessor::Instance()->IsLoggedInServer() )
		{
			PRINT_NORMAL_LOG( L"Don't Send Chat Message, Login First!!\n" );
			continue;
		}

		/*CPacketNormalChatReq ChatReq;
		ChatReq.SetSenderAccountID( ChatPacketProcessor::Instance()->GetAccountID() );
		ChatReq.SetChatText( recvInput );
		ChatReq.CalculateSize();

		TrySend = pNetworkManager->SendToServer( (const char *)&ChatReq, ChatReq.GetSize(), SendResult );
		if( false == TrySend )
		{
			continue;
		}

		if( 0 < SendResult )
		{
			RecvPacket = true;
		}*/
	}

	return 0;
}

int _tmain( int argc, _TCHAR* argv [] )
{
	argc; argv;

	AuroraMiscManager->SetLogFile( L"..\\ChatClientLog.txt" );
	
	AuroraWinsockManager->SetNetworkMode( ENetworkRunMode::Client );
	AuroraNetworkManager->SetNetworkMode( ENetworkRunMode::Client );

	char serverIP[MAX_IPV4_IP_LEN] = {0};
	AuroraStringManager->Copy( "127.0.0.1", serverIP, MAX_IPV4_IP_LEN );

	UInt16 port = 15541;
	bool InitResult = AuroraNetworkManager->InitClientNetwork( serverIP, port );
	if( true == InitResult )
	{
		int RetryCount = 10;
		bool TryConnect = false;
		for ( auto Current = 0; Current < RetryCount; ++Current )
		{
			TryConnect = AuroraNetworkManager->ConnectToServer();
			if( true == TryConnect )
			{
				break;
			}

			Sleep( 2000 );
		}

		AuroraThreadManager->BeginThread( UserInput, nullptr, nullptr );
		if( true == TryConnect )
		{
			HANDLE ParseHandle = AuroraThreadManager->BeginThread( ChattingPacketProcessor->ParsePacket, 
																   ChattingPacketProcessor,
																   nullptr );


			InitResult = AuroraWinsockManager->StartEventSelectClient( AuroraNetworkManager->GetClientSocket(), 
																	   ChattingPacketProcessor->GetReceivedPackets(),
																	   ChattingPacketProcessor->GetThreadEventHandle() );

			WaitForSingleObject( ParseHandle, INFINITE );

			if( true == InitResult )
			{
				PRINT_FILE_LOG( L"Success to Connect Server![IP : %S, Port : %d]\n", serverIP, port );
				return 0;
			}
		}
		else
		{
			PRINT_FILE_LOG( L"Failed to Connect Server![IP : %S, Port : %d]\n", serverIP, port );
			return 0;
		}
	}

	return 0;
}