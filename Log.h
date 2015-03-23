/*
 * Log.h
 *
 *  Created on: Mar 22, 2015
 *      Author: neo
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

class Log
{
	public:
		string _path;
		Log(string path);
		void log(string reg);
};



#endif /* LOG_H_ */
