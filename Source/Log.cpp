/*
 * Log.cpp
 *
 *  Created on: Mar 22, 2015
 *      Author: neo
 */

#include <Log.h>

Log* Log::_instance = 0;

Log* Log::Instance(int log_level, string path)
{
	if (_instance == 0)
	{
		_instance = new Log(log_level,path);
	}
	return _instance;
}

Log::Log(int log_level, string path)
{
	//Assign the argument to the local atribute
	_path = path;
	_logLevel = log_level;
	//Check if the file exists, if not create it.
	fstream logFile(path.c_str(),fstream::app);
	if (!logFile)
	{
		//The file does not exist, must create it
		fstream logFile(path.c_str(),fstream::out);
	}
	logFile.close();
}

void Log::log(int level, string reg)
{
	bool allowed = canLog(level);
	if (allowed)
	{
		time_t timer;
		time (&timer);
		string stringTime = ctime(&timer);
		stringTime = stringTime.substr(0,stringTime.length()-1);
		string prefix = getPrefix(level);
		//Open the stream
		fstream logFile(_path.c_str(),fstream::app);
		logFile << prefix << DELIMETER;
		logFile << stringTime << DELIMETER;
		logFile << reg << endl;
		logFile.close();
	}
}

bool Log::canLog(int intentLevel)
{
	if (intentLevel <= _logLevel) //Ej Intento guardar un error en nivel debug
	{
		return true;
	}else{
		return false;
	}
}
string Log::getPrefix(int level)
{
	string prefix = "";
	switch(level)
	{
	case ERROR:
		prefix = "ERROR";
		break;
	case WARNING:
		prefix = "WARNING";
		break;
	case DEBUG:
		prefix = "DEBUG";
		break;
	}
	return prefix;
}


