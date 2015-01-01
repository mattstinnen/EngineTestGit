#ifndef LOG_H
#define LOG_H
//====================================================================================================
//	Filename:		Log.h
//	Created by:		Matt Stinnen
//	Discription:	singleton class for logging messages. 
//					the messages are serialized to an external HTML file
//====================================================================================================

//====================================================================================================
// Namespace
//====================================================================================================

#include"NoCopyAssign.h"

namespace MAGE3D
{
namespace CORE
{

enum messageTypes
{
	kDefault = 0,
	kSystem,
	kError

};

class Log : public NoCopyAssign
{
public:
	static Log& Get();
private:
	// private constructor for singleton
	Log();
public:
	
	~Log();

	//functions to initialize/terminate the error log
	void Initilize(const char* pFilename = "Log.html");
	void Terminate();
	void Message(const char* pMessage, messageTypes messageLevel = kDefault, ...);// customize later(setup for taking color)

private:
	FILE* mpFile;
	bool mInitialized;

};


}// namespace Core
}// namespace MAGE3D


#endif // LOG_H