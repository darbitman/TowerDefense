//**************************************************************************************************//
//  Logger.cpp                                                                                      //
//                                                                                                  //
//  Definition: Implementation fo CLogger class.                                                    //
//  Version 0.1                                                                                     //
//                                                                                                  //
//  Changelog:                                                                                      //
//  4/1?/2012 - Created file.                                                                       //
//                                                                                                  //
//  Notes:  Singleton logger class to help facilitate debugging.                                    //
//                                                                                                  //
//          To use this class:                                                                      //
//                                                                                                  //
//          CLogger::instance().log ( "Message here.", CLogger::kLogLevelError );                   //
//                                                                                                  //
//          A string "Error: Message here." will be written in the tdefense.log file.               //
//                                                                                                  //
//**************************************************************************************************//

#include "Logger.h"

const std::string CLogger::kLogLevelDebug	= "DEBUG";
const std::string CLogger::kLogLevelInfo	= "INFO";
const std::string CLogger::kLogLevelError	= "ERROR";

const char * const CLogger::kLogFileName	= "tdefense.log";

CLogger CLogger::sLoggerInstance;

CLogger& CLogger::instance ( )
{
	return sLoggerInstance;
}

CLogger::~CLogger ( )
{
	mOutputStream.close ( );
}

CLogger::CLogger ( )
{
	mOutputStream.open ( kLogFileName, std::ios_base::app );
	if ( ! mOutputStream.good ( ) )
	{
		std::cerr << "Unable to initialize the Logger!" << std::endl;
	}
}

void CLogger::log ( const std::string& message, const std::string& loglevel )
{
	mOutputStream << loglevel << ": " << message << std::endl;
}
