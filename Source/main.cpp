/*
 * main.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: neo
 */
#include <SDL.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <json.h>
#include <Log.h>
#include <Nivel.h>

int main( int argc, char* args[] )
{
	Log::Instance()->log(DEBUG,"this is a test");
	return 0;
}
