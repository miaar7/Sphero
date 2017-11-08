#include <stddef.h>
#include <unistd.h>
#include <iostream>
#include "sphero/bluetooth/bluez_adaptor.h"
#include "sphero/Sphero.hpp"
#include "sphero/packets/Constants.hpp"

using namespace std;

void test()
{
	cout << "I Recieved som Data that wass parsed!!" << endl;
}

int main(void) {

	//FILE* pFile = fopen("StreamAndSendRandomWith100Hz_10Hz.txt", "a");
	//FILE* pFile = fopen("StreamAndSendWhenRecieve.txt", "a");
	FILE* pFile = fopen("StreamSensorsWithoutSending.txt", "a");
	
	struct timeval tv;	
	long int startTimeSec;
	long int startTimeuSec;
	
	int freq = 0;
	int sensorsStreaming = 0;
	struct timeval randomSend;


	string address = "CD:72:26:BA:E7:32";


	Sphero* sph = new Sphero(address.c_str(), new bluez_adaptor());

	DataBuffer* sensors = sph->getDataBuffer();

	sph->onConnect([pFile]() {
		cout << "Here I come, honourable people !" << endl;
		//fprintf(pFile, "%s\t%s\t%s\n","Frequency","NrOfSensors","time");
	});

	sph->onData([sph,pFile, &tv,&startTimeSec, &startTimeuSec, &freq, &sensorsStreaming]() {
		//sph->roll(20, (rand() % 100 + 1), 1); // Send as soon a message is recieved and interpretet
		//uint16_t value;
		//sensors->getValue(dataTypes::FILTERED_ACCEL_X, value);
		gettimeofday(&tv, NULL);
		long int timeStamp = (tv.tv_sec - startTimeSec) * 1000000 + (tv.tv_usec - startTimeuSec);
		//fprintf(pFile,"%i\t%i\t%ld\n",freq, sensorsStreaming, timeStamp);
		
	});

	if (sph->connect())
	{
		cout << "Connected to Sphero" << endl;
	}
	else
	{
		cout << "Could not connect to Sphero" << endl;
		return -1;
	}

	sph->setColor(255, 0, 0);
	sph->disableCollisionDetection();

	uint32_t sensorArr[] = {
		mask::RAW_ACCEL_X,
		mask::RAW_ACCEL_X | mask::RAW_GYRO_X,
		mask::RAW_ACCEL_X | mask::RAW_GYRO_X | mask::RAW_RIGHT_MOTOR_BACK_EMF,
		mask::RAW_ACCEL_X | mask::RAW_GYRO_X | mask::RAW_RIGHT_MOTOR_BACK_EMF | mask::FILTERED_RIGHT_MOTOR_BACK_EMF

	};

	int freqArr[] = {
		10,
		20,
		50,
		100,
		200,
		300
	};

	long int currentTimeSec;
	long int currentTimeuSec;


	usleep(2000000);

	while (1)
	{
		sph->setRawMotor(1, 255, 2, 255);
		usleep(100000);
	}

	/*
	for (int j = 0; j < 6; j++)
	{
		freq = freqArr[j];
		for (int i = 0; i < 4; i++) {
			// Update timestamp to use in datalogging
			gettimeofday(&tv, NULL);
			startTimeSec = tv.tv_sec;
			startTimeuSec = tv.tv_usec;	

			// Update time values to use to stop looping
			currentTimeSec = startTimeSec;
			currentTimeuSec = startTimeuSec;

			// Start datastreaming with new values.
			sph->setDataStreaming(freq, 1, sensorArr[i], 0, 0);
			sensorsStreaming = sph->getNumberOfStreaming();

			
			// Loop for 30 sec. 
			while (currentTimeSec- startTimeSec < 30)
			{

				*/
				//Used when Random send		
				/*
				usleep((rand() % 10 + 1) * 10000); // Wait a random time between 0.01sec. and 0.1sec.
			    // Send a command
				sph->roll(20, (rand() % 100 + 1), 1); // Random direction
				*/

				/*
				// Update time
				gettimeofday(&randomSend, NULL); 
				currentTimeSec = randomSend.tv_sec;
				currentTimeuSec = randomSend.tv_usec;

			}			
	

			cout << "j: " << j << " i: " << i << endl;
			
		}
		
		
	}
	*/
	fclose(pFile);
	return 0;
}