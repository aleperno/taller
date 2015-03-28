/*
 * Log.h
 *
 *  Created on: Mar 22, 2015
 *      Author: neo
 *	Usage:
 *		Log::Instance(LOG_PATH)->log(LEVEL,REPORT);
 *		LOG_PATH may be empty
 *		LEVEL & REPORT MUST NOT be empty
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
#define DEBUG "DEBUG"
#define WARNING "WARNING"
#define ERROR "ERROR"
#define DELIMETER "\t"


class Log
{
	protected:
		Log(string path);
	public:
		void log(string level, string reg);
		static Log* Instance(string path = LOG_PATH);
	private:
		static Log* _instance;
		string _path;
};



#endif /* LOG_H_ */
