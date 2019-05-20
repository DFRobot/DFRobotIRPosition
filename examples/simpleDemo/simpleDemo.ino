/*
	simpleDemo.ino
	DFRobot's Positioning ir camera

	[Get the module here]
	http://www.dfrobot.com/index.php?route=product/product&product_id=1088

	This example get the four lightest positions of the IR sources.

	[Connection and Diagram]
	http://www.dfrobot.com/wiki/index.php/Positioning_ir_camera#Connection_Diagram

	Copyright (c) DFRobot, 2016
	GNU Lesser General Public License

	author Angelo, Angelo.qiao@dfrobot.com
	edited by JMF, https://github.com/Fubaxiusz
	version V1.1
	May 20, 2019
*/


/*
Library for DFRobot's IR Position Camera,
by Angelo, Angelo.qiao@dfrobot.com
*/
#include "DFRobotIRPosition.h"

DFRobotIRPosition myDFRobotIRPosition;

unsigned short positionX[4]; // Store beacon X position from 0-1023 (1023 means empty)
unsigned short positionY[4]; // Store beacon Y position from 0-767 (1023 means empty)
float blobSize[4]; // Store beacon size from 0-7 (15 means empty)
byte blobsDetected; // Store number of detected blobs 0-4

// Print the position result.
void printResult();

void setup()
{
	Serial.begin(19200);

	// Initailize the module.
	myDFRobotIRPosition.begin();

	delay(2000); // Wait 2 seconds

	// Print data header
	Serial.println("Pos1X,Pos1Y,Size1;Pos2X,Pos2Y,Size2;Pos3X,Pos3Y,Size3;Pos4X,Pos4Y,Size4;");
}

void loop()
{
	// request the position
	myDFRobotIRPosition.requestPosition();

	// If there is data available, print it. Otherwise show the error message.
	if ( myDFRobotIRPosition.available() )
	{
		blobsDetected = myDFRobotIRPosition.count();
		for (byte i=0; i<4; i++)
		{
			positionX[i] = myDFRobotIRPosition.readX(i);
			positionY[i] = myDFRobotIRPosition.readY(i);
			blobSize[i] = myDFRobotIRPosition.readSize(i);
		}

		printResult();
	}
	else Serial.println("Device not available!");

	delay(50);
}

// Print the position result.
void printResult()
{
	if ( blobsDetected == 0 )
	{
		Serial.println("No blobs detected!");
		delay(2000); // Wait 2 seconds
	}
	else
	{
		for (byte i=0; i<4; i++)
		{
			// Print only visible blobs position (max 4)
			if ( myDFRobotIRPosition.detected(i) )
			{
				Serial.print(positionX[i]);
				Serial.print(",");

				Serial.print(positionY[i]);
				Serial.print(",");

				Serial.print(blobSize[i],1);
				Serial.print(";");
			}
			else Serial.print("none,none,none;");
		}
		Serial.println();
	}
}
