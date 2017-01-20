/*
 * SerialPort.cpp
 *
 *  Created on: 27.06.2016
 *      Author: JochenAlt
 */

#include "SerialPort.h"
#include "string.h"
#include "Util.h"
#include "logger.h"
#include "RS232/rs232.h"

using namespace std;


SerialPort::SerialPort() {
	_port = -1;
	comEnumerate();
}

SerialPort::~SerialPort() {
	comTerminate();
}

bool SerialPort::connect( string device, int baudRate) {
	_port = comFindPort(device.c_str());
	bool ok = comOpen(_port, baudRate);
	return ok;
}

void SerialPort::disconnect(void) {
	if (_port >=0 ) {
		LOG(DEBUG) << "disconnect from serial port";
		comClose(_port);
		_port = -1;
	}
}

int SerialPort::sendArray(char *buffer, int len) {
	int bytesWritten = comWrite(_port, buffer,len);
	return bytesWritten ;
}

int SerialPort::getArray (char *buffer, int len) {
	int bytesRead= comRead(_port, buffer,len);
	return bytesRead;
}

int SerialPort::sendString(string str) {
	str += newlineStr;
	int written = sendArray((char*)str.c_str(), str.length());
	return written;
}


int SerialPort::receive(string& str) {
	str = "";
	int totalBytesRead = 0;
	int bytesRead= 0;
	const int BufferSize = 256;
	char buffer[BufferSize];

	do {
		bytesRead= getArray(buffer, BufferSize);
		if (bytesRead > 0) {
			totalBytesRead += bytesRead;
			str += string(buffer, bytesRead);
		}
	} while ((bytesRead > 0) && (bytesRead == BufferSize));

	return totalBytesRead;
}

void SerialPort::clear() {
	if (_port >= 0) {
		string str;
		while (receive(str) > 0);
	}
}
