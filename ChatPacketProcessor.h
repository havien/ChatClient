#pragma once

#include "../AuroraUtility/Includes.h"
#include "../AuroraUtility/AuroraSingleton.h"
#include "../AuroraUtility/Queue.h"

#include "ChatDefine.h"

using namespace Aurora;
class ChatPacketProcessor : public Singleton<ChatPacketProcessor>
{
	friend class Singleton<ChatPacketProcessor>;
private:
	ChatPacketProcessor( void );
	NON_COPYABLE( ChatPacketProcessor );
public:
	virtual ~ChatPacketProcessor( void );

	static unsigned int __stdcall ParsePacket( void *pArgs );

	void SetAccountID( const wchar_t *pAccountID );

	inline CQueue<char*>* GetReceivedPackets( void )
	{
		return &_receivedPackets;
	}

	inline void SetUniqueID( const UInt32 UniqueID )
	{
		_uniqueID = UniqueID;
	}

	inline HANDLE* GetThreadEventHandle( void )
	{
		return &_waitHandle; 
	}

	inline void ResetWaitEvent( void )
	{
		ResetEvent( _waitHandle ); 
	}

	inline void StopParsePacketThread( void )
	{ 
		_waitHandle = false;
	}

	inline const UInt32 GetUniqueID( void ) const
	{
		return _uniqueID;
	}

	inline const WCHAR* GetAccountID( void ) const 
	{ 
		return _accountID;
	}

	inline bool IsLoggedInServer( void ) 
	{ 
		return _loggedinServer;
	}

	inline void SetLoggedInServer( void )
	{
		_loggedinServer = true;
	}

	inline void SetLoggedOutServer( void )
	{
		_loggedinServer = false;
	}
private:
	bool _runningParsePacketThread;
	HANDLE _waitHandle;
	CQueue<char*> _receivedPackets;

	bool _loggedinServer;
	UInt32 _uniqueID;
	WCHAR _accountID[MAX_ACCOUNT_ID_LEN];
};

#define ChattingPacketProcessor ChatPacketProcessor::Instance()