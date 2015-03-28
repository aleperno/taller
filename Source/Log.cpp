/*
 * Log.cpp
 *
 *  Created on: Mar 22, 2015
 *      Author: neo
 */

#include "Log.h"

Log* Log::_instance = 0;

Log* Log::Instance(string path)
{
	if (_instance == 0)
	{
		_instance = new Log(path);
	}
	return _instance;
}

Log::Log(string path)
{
	//Assign the argument to the local atribute
	_path = path;
	//Check if the file exists, if not create it.
	fstream logFile(path.c_str(),fstream::app);
	if (!logFile)
	{
		//The file does not exist, must create it
		fstream logFile(path.c_str(),fstream::out);
	}
	logFile.close();
}

void Log::log(string level, string reg)
{
	time_t timer;
	time (&timer);
	string stringTime = ctime(&timer);
	stringTime = stringTime.substr(0,stringTime.length()-1);
	//Open the stream
	fstream logFile(_path.c_str(),fstream::app);
	logFile << level << DELIMETER;
	logFile << stringTime << DELIMETER;
	logFile << reg << endl;
	logFile.close();
}


