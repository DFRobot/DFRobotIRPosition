/*
	DFRobotIRPosition.cpp
	DFRobot's Positioning ir camera
	CPP source file for DFRobot's Positioning ir camera

	Copyright (c) DFRobot, 2016
	GNU Lesser General Public License

	author Angelo, Angelo.qiao@dfrobot.com
	edited by JMF, https://github.com/Fubaxiusz
	version V1.1
	May 20, 2019
*/

#include "Arduino.h"
#include "Wire.h"
#include "DFRobotIRPosition.h"

// Constructor
DFRobotIRPosition::DFRobotIRPosition() {}
// Destructor
DFRobotIRPosition::~DFRobotIRPosition() {}

void DFRobotIRPosition::writeTwoIICByte(byte first, byte second)
{
	Wire.beginTransmission(IRAddress);
	delay(50);
	Wire.write(first);
	delay(50);
	Wire.write(second);
	delay(50);
	Wire.endTransmission();
	delay(50);
}

void DFRobotIRPosition::begin()
{
	Wire.begin();
	writeTwoIICByte(0x30,0x01);
	writeTwoIICByte(0x30,0x08);
	writeTwoIICByte(0x06,0x90);
	writeTwoIICByte(0x08,0xC0);
	writeTwoIICByte(0x1A,0x40);
	writeTwoIICByte(0x33,0x33);

	delay(100);
}

void DFRobotIRPosition::requestPosition()
{
	Wire.beginTransmission(IRAddress);
	Wire.write(0x36);
	Wire.endTransmission();
	Wire.requestFrom(IRAddress, 16);
}

bool DFRobotIRPosition::available()
{
	blobCount = 0; // Set initial number of detected blobs

	if (Wire.available() == 16) // read only if data length fits
	{
		for (byte i=0; i<16; i++)
		{ positionData.receivedBuffer[i]=Wire.read(); }

		for (byte i=0; i<4; i++)
		{
			positionX[i] = (unsigned short)(positionData.positionFrame.rawPosition[i].xLowByte)
			+ ((unsigned short)(positionData.positionFrame.rawPosition[i].xyHighByte & 0x30U) << 4);

			positionY[i] = (unsigned short)(positionData.positionFrame.rawPosition[i].yLowByte)
			+ ((unsigned short)(positionData.positionFrame.rawPosition[i].xyHighByte & 0xC0U) << 2);

			blobSize[i] = (unsigned short)(positionData.positionFrame.rawPosition[i].xyHighByte & 0x0F);
		}

		// Set detected blobs status
		for (byte i=0; i<4; i++)
		{
			blobVisible[i] = blobSize[i] != 15; // Blob status
			blobCount += blobVisible[i];
		}

		return true;
	}
	else // otherwise skip them
	{
		while ( Wire.available() ) { Wire.read(); }
		return false;
	}
}

unsigned short DFRobotIRPosition::readX(byte index)
{ return positionX[index]; }

unsigned short DFRobotIRPosition::readY(byte index)
{ return positionY[index]; }

byte DFRobotIRPosition::readSize(byte index)
{ return blobSize[index]; }

bool DFRobotIRPosition::detected(byte index)
{ return blobVisible[index]; }

byte DFRobotIRPosition::count()
{ return blobCount; }
