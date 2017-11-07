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

	FILE* pFile = fopen("StreamAndSendWhenRecieve.txt", "a");
	struct timeval tv;	
	long int startTimeSec;
	long int startTimeuSec;
	int freq = 0;


	string address = "CD:72:26:BA:E7:32";


	Sphero* sph = new Sphero(address.c_str(), new bluez_adaptor());

	DataBuffer* sensors = sph->getDataBuffer();

	sph->onConnect([pFile]() {
		cout << "Here I come, honourable people !" << endl;
		fprintf(pFile, "%s\t%s\t%s\n","Frequency","NrOfSensors","time");
	});

	sph->onData([sph, pFile, &tv,&startTimeSec, &startTimeuSec, &freq]() {
		//sph->roll(20, 20, 1);
		uint16_t value;
		//sensors->getValue(dataTypes::FILTERED_ACCEL_X, value);
		gettimeofday(&tv, NULL);
		fprintf(pFile,"%i\t%i\t%ld\n",freq, sph->getNumberOfStreaming(), (tv.tv_sec - startTimeSec) * 1000 + (tv.tv_usec - startTimeuSec)/1000);
		
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

	usleep(2000000);
	for (int j = 0; j < 6; j++)
	{
		freq = freqArr[j];
		for (int i = 0; i < 4; i++) {
			gettimeofday(&tv, NULL);
			startTimeSec = tv.tv_sec;
			startTimeuSec = tv.tv_usec;
			
			sph->setDataStreaming(freq, 1, sensorArr[i], 0, 0);
			usleep(30000000);
		}
	}
	fclose(pFile);
	return 0;
}