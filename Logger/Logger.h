//**************************************************************************************************//
//  Logger.h                                                                                        //
//                                                                                                  //
//  Definition: Header file for CLogger class.                                                      //
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

#ifndef CLOGGER_H
#define CLOGGER_H

#include "../Config.h"

class CLogger
{
	public:
		static const std::string kLogLevelDebug;
		static const std::string kLogLevelInfo;
		static const std::string kLogLevelError;


	public:
		static CLogger& instance ( );
		void log ( const std::string& message, const std::string& loglevel  );


	protected:
		static CLogger sLoggerInstance;
		static const char * const kLogFileName;
		std::ofstream mOutputStream;


	private:
		CLogger ( );
		~CLogger ( );
};

#endif
