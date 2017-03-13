#pragma once

#include "../AuroraUtility/MiscManager.h"
#include "../AuroraUtility/StringManager.h"
#include "../AuroraNetwork/PacketBase.h"

#include "ChatPacketProcessor.h"
#include "ChatPacketDefine.h"

using namespace Aurora;
using namespace Aurora::Network;

ChatPacketProcessor::ChatPacketProcessor( void ) :
_receivedPackets( 256 ),
_runningParsePacketThread( true ),
_loggedinServer( false )
{
	_waitHandle = CreateEvent( nullptr, TRUE, FALSE, nullptr );
	if ( _waitHandle == nullptr )
	{
		//printf( "CreateEvent failed (%d)\n", GetLastError() );
	}
}

ChatPacketProcessor::~ChatPacketProcessor( void )
{

}

void ChatPacketProcessor::SetAccountID( const WCHAR* pAccountID )
{
	AuroraStringManager->ClearAndCopy( pAccountID, _accountID, MAX_ACCOUNT_ID_LEN );
}

UInt32 __stdcall ChatPacketProcessor::ParsePacket( void *pArgs )
{
	if ( pArgs )
	{
		auto pThis = (ChatPacketProcessor *)pArgs;
		auto pBufferQueue = pThis->GetReceivedPackets();
		while( true == pThis->_runningParsePacketThread )
		{
			auto WaitResult = WaitForSingleObject( pThis->_waitHandle, INFINITE );
			if ( WAIT_OBJECT_0 == WaitResult )
			{
				if ( 0 == pBufferQueue->ItemCount() )
				{
					// Sleep.
					pThis->ResetWaitEvent();
				}

				if ( 0 < pBufferQueue->ItemCount() )
				{
					char* pBuffer = pBufferQueue->Dequeue();
					if ( pBuffer )
					{
						BasePacket* pPacket = reinterpret_cast<BasePacket *>(pBuffer);
						auto Operation = static_cast<EPacketOperation>( pPacket->GetType() );
						switch ( Operation )
						{
							case EPacketOperation::RegisterAck:
							{
								auto pRegisterAck = reinterpret_cast<CPacketRegisterAck*>(pPacket);
								if( ECommonResult::OK == pRegisterAck->GetResult() )
								{									
									PRINT_NORMAL_LOG( L"Success to Register! New AccountID is [%d]\n", pRegisterAck->uniqueID );
								}
								else
								{
									PRINT_NORMAL_LOG( L"Failed to Register! EPacketResult is [%d]\n", pRegisterAck->GetResult() );
								}
								break;
							}
							case EPacketOperation::ChatAck:
							{
								/*CBroadcastPacketNormalChat *pChatPacket = (CBroadcastPacketNormalChat *)pPacket;
								PRINT_NORMAL_LOG( L"[%s|%s]\n", pChatPacket->GetSenderAccountID(), pChatPacket->GetChatText() );
								break;*/
							}
							case EPacketOperation::LoginAck:
							{
								/*CPacketLoginAck *pLoginAck = (CPacketLoginAck *)pPacket;
								if ( ePacketResult_OK == pLoginAck->GetResult() )
								{
									pThis->SetLoggedInServer();
									pThis->SetAccountID( pLoginAck->GetAccountID() );
									pThis->SetUniqueID( pLoginAck->GetUniqueID() );
									PRINT_NORMAL_LOG( L"Success to Login! [%s]\n", pLoginAck->GetAccountID() );
								}
								else
								{
									PRINT_NORMAL_LOG( L"Failed to Register! EPacketResult is [%d]\n", pLoginAck->GetResult() );
								}*/
								break;
							}
							case EPacketOperation::LogoutAck:
							{
								/*CPacketLogoutAck *pLogoutAck = (CPacketLogoutAck *)pPacket;
								if ( ePacketResult_OK == pLogoutAck->GetResult() )
								{
									pThis->SetLoggedOutServer();
									pThis->SetAccountID( L"" );
									pThis->SetUniqueID( 0 );
									PRINT_NORMAL_LOG( L"Success to Logout! [%d]\n", pLogoutAck->GetUniqueID() );
								}
								else
								{
									PRINT_NORMAL_LOG( L"Failed to Register! EPacketResult is [%d]\n", pLogoutAck->GetResult() );
								}*/
								break;
							}
							case EPacketOperation::WhisperChatAck:
							{
								/*CPacketWhisperChatAck *pWhisperAck = (CPacketWhisperChatAck *)pPacket;
								if ( ePacketResult_OK == pWhisperAck->GetResult() )
								{
									PRINT_NORMAL_LOG( L"Success to Send Whisper Message!\n" );
								}
								else
								{
									PRINT_NORMAL_LOG( L"Failed to Send Whisper Message! EPacketResult is [%d]\n", pWhisperAck->GetResult() );
								}*/
								break;
							}
							case EPacketOperation::NotifyEvent:
							{
								/*CBroadcastPacketNotifyEvent *pNotifyEvent = (CBroadcastPacketNotifyEvent *)pPacket;*/
								//PRINT_NORMAL_LOG( L"Received Notify Event! [Message : %s]", pNotifyEvent->GetNotifyMessage() );
								break;
							}
						}
					}
				}
			}
			
			pThis->ResetWaitEvent();
			Sleep( 1 );
		}
	}

	return 0;
}