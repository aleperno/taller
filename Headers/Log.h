/*
 * Log.h
 *
 *  Created on: Mar 22, 2015
 *      Author: neo
 *  Construction:
 *  	Log::Instance(log_level, path)
 *		log_level sets the available log levels
 *		path sets the path to save.
 *		Both may be empty.
 *	Usage:
 *		Log::Instance()->log(LEVEL,REPORT);
 *		LOG_PATH may be empty
 *		LEVEL & REPORT MUST NOT be empty
 *
 *	logLevel: There's 3 options
 *		0: Error -> Default
 *		1: Warnings
 *		2: Debug
 */

#ifndef LOG_H_
#define LOG_H_

#include <time.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
//Default log path
#define LOG_PATH "log.txt"
//Level Options
#define DEBUG 2
#define WARNING 1
#define ERROR 0
#define DELIMETER "\t"


class Log
{
	protected:
		Log(int log_level, string path);
	public:
		void log(int level, string reg);
		static Log* Instance(int log_level = ERROR, string path = LOG_PATH);
	private:
		bool canLog(int level);
		static Log* _instance;
		string _path;
		int _logLevel;
		string getPrefix(int level);
};



#endif /* LOG_H_ */
