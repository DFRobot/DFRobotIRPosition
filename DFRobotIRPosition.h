/*
	DFRobotIRPosition.h
	DFRobot's Positioning ir camera
	Header file for DFRobot's Positioning ir camera

	Copyright (c) DFRobot, 2016
	GNU Lesser General Public License

	author Angelo, Angelo.qiao@dfrobot.com
	edited by JMF, https://github.com/Fubaxiusz
	version V1.1
	May 20, 2019
*/

#include "Arduino.h"
#include "Wire.h"

#ifndef DFRobotIRPosition_cpp
#define DFRobotIRPosition_cpp

class DFRobotIRPosition
{
	const int IRAddress = 0xB0 >> 1; // IIC address of the sensor

	// position data structure from IIC sensor
	union PositionData
	{
		byte receivedBuffer[16]; // received buffer for IIC read
		struct
		{
			byte header;
			struct
			{
				byte xLowByte; // position x low byte.
				byte yLowByte; // position y low byte.
				byte xyHighByte; // position x and y high byte.
			}__attribute__ ((packed)) rawPosition[4]; // 4 raw positions.
		}__attribute__ ((packed)) positionFrame;
	}__attribute__ ((packed)) positionData;

	unsigned short positionX[4]; // position x.
	unsigned short positionY[4]; // position y.
	byte blobSize[4]; // blob size
	bool blobVisible[4]; // blob detection status
	byte blobCount; // number of detected blobs

	// Write two byte into the sensor to initialize and send data.
	// first the first byte
	// second the second byte
	void writeTwoIICByte(byte first, byte second);

	public:
		// Constructor
		DFRobotIRPosition();

		// Destructor
		~DFRobotIRPosition();

		// Initialize the sensor.
		void begin();

		// Request the position, IIC will block the progress until all the data is transmitted.
		void requestPosition();

		// After requesting the position, and the data read from the sensor is ready, True will be returned.
		// returns whether data reading is ready.
		// true Is ready
		// false Is not ready
		bool available();

		// Get the X position of the point (0-1023), 1023 means empty.
		// input the index of the 4 light objects ranging from 0 to 3,
		// returns The X position corresponding to the index.
		unsigned short readX(byte index);

		// Get the Y position of the point (0-767), 1023 means empty.
		// input the index of the 4 light objects ranging from 0 to 3,
		// returns The Y position corresponding to the index.
		unsigned short readY(byte index);

		// Get the size of the point (0-7), 15 means empty.
		// input the index of the 4 light objects ranging from 0 to 3,
		// returns The size corresponding to the index.
		byte readSize(byte index);

		// Get the visibility of given blob
		// input the index of the 4 light objects ranging from 0 to 3,
		// returns The detection status corresponding to the index.
		bool detected(byte index);

		// Get the number of detected points from 0 to 4.
		byte count();

		// Store sensor resolution info
		const unsigned short width = 1023;
		const unsigned short height = 767;
		const byte size = 7;
};


#endif
