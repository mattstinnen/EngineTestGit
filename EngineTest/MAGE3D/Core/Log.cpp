//====================================================================================================
//	Filename:		Log.h
//	Created by:		Matt Stinnen
//	Discription:	singleton class for logging messages. 
//					the messages are serialized to an external HTML file
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "precompiled.h"
#include "Log.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace MAGE3D
{
namespace CORE
{

//====================================================================================================
// Definitions
//====================================================================================================

Log& Log::Get()
{
	static Log sInstance;
	return sInstance;
}

//----------------------------------------------------------------------------------------------------

Log::Log()
	:mpFile(NULL)
	,mInitialized(false)
{

}

//----------------------------------------------------------------------------------------------------

Log::~Log()
{
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void Log::Initilize(const char* pFilename)
{
	// Check if we have already initilized
	ASSERT(!mInitialized, "[Log] Already initialized");

	// open for writing
	fopen_s(&mpFile, pFilename, "wt");
	ASSERT(NULL != mpFile, "[Log] Cannot open file for writing.");

	// write the header
	fprintf_s(mpFile, "<HTML>\n<HEAD><TITLE>MAGE3D Log</TITLE></HEAD>\n<BODY>\n");

	// get time and date
	char time[kMaxStringSize];
	char date[kMaxStringSize];
	_strtime_s(time, kMaxStringSize);
	_strdate_s(date, kMaxStringSize);

	// Write the date/time
	fprintf_s(mpFile, "<FONT size = '55'>Log started at %s on %s</FONT><BR><BR>\n", time, date);

	// set flag
	mInitialized = true;
}

//----------------------------------------------------------------------------------------------------

void Log::Terminate()
{
	if(mInitialized)
	{
		// make sure we have a file to write to
		ASSERT(NULL != mpFile, "[Log] No file for writing")
		
		// get time and date
		char time[kMaxStringSize];
		char date[kMaxStringSize];
		_strtime_s(time, kMaxStringSize);
		_strdate_s(date, kMaxStringSize);

		// Write the date/time
		fprintf_s(mpFile, "</BODY>\n<FONT size= '55'>Log closed at %s on %s</FONT></HTML>\n", time, date);

		// close file
		fclose(mpFile);
		mpFile = NULL;

		// clear flag
		mInitialized = false;
	}
}

//----------------------------------------------------------------------------------------------------

void Log::Message(const char* pMessage, messageTypes messageLevel, ...)
{
	// MAke sure er have initialized firtst
	ASSERT(mInitialized, "[Log] initialize() must be called before writing messages.");

	// get message
	char msg[kMaxStringSize];
	va_list va;
	va_start(va,pMessage);
	vsprintf_s(msg, kMaxStringSize, pMessage, va);
	va_end(va);

	// get time
	char time[kMaxStringSize];
	_strtime_s(time, kMaxStringSize);
    
	
	// write message
	switch (messageLevel)
	{
	case kDefault:
		{
			fprintf_s(mpFile, "<FONT color='black'>%s&nbsp;&nbsp;&nbsp;&nbsp;%s</FONT><BR>\n", time, msg);
		}break;
	case kError:
		{
			fprintf_s(mpFile, "<FONT color='red'>%s&nbsp;&nbsp;&nbsp;&nbsp;%s</FONT><BR>\n", time, msg);
		}break;
	case kSystem:
		{
			fprintf_s(mpFile, "<FONT color='blue'>%s&nbsp;&nbsp;&nbsp;&nbsp;%s</FONT><BR>\n", time, msg);
		}break;
	}
	

	// also write to output window
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
}

//----------------------------------------------------------------------------------------------------


}// namespace Core
}// namespace MAGE3D

