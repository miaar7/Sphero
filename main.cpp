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
	//FILE* pFile = fopen("StreamSensorsWithoutSending.txt", "a");
	//FILE* pFile = fopen("StreamSensorsSync53Hz.txt", "a");
	FILE* pFile = fopen("SineWave_SyncSend50Hz.txt", "a");
	
	struct timeval tv;	
	long int startTimeSec;
	long int startTimeuSec;
	
	int freq = 0;
	int sensorsStreaming = 0;
	struct timeval randomSend;

	float sinWave[] = { 100, 87.21, 74.63, 62.47, 50.93, 40.19, 30.43, 21.82, 14.49, 8.56, 4.13, 1.28, 0.05, 0.46, 2.51, 6.15, 11.34, 17.98, 25.97, 35.18, 45.45, 56.61, 68.49, 80.88, 93.59, 106.41, 119.12, 131.51, 143.39, 154.55, 164.82, 174.03, 182.02, 188.66, 193.85, 197.49, 199.54, 199.95, 198.72, 195.87, 191.44, 185.51, 178.18, 169.57, 159.81, 149.07, 137.53, 125.37, 112.79, 100 };

	//string address = "CD:72:26:BA:E7:32";
	string address = "C5:A2:54:D0:39:C9";

	Sphero* sph = new Sphero(address.c_str(), new bluez_adaptor());

	DataBuffer* sensors = sph->getDataBuffer();

	sph->onConnect([pFile]() {
		cout << "Here I come, honourable people !" << endl;
		//fprintf(pFile, "%s\t%s\t%s\n","Frequency","NrOfSensors","time");
	});

	sph->onData([sph,pFile, &tv,&startTimeSec, &startTimeuSec, &freq, &sensorsStreaming, &sinWave, &sensors]() {
		static int i = 0;
		sph->setRawMotor(1, sinWave[i], 1, sinWave[i]); // Send as soon a message is recieved and interpretet
		if (i < sizeof(sinWave) / sizeof(sinWave[0]) - 1) {
			i++;
		}
		else
		{
			i = 0;
		}
		
		uint16_t value;
		sensors->getValue(dataTypes::RAW_RIGHT_MOTOR_PWM, value);
		gettimeofday(&tv, NULL);
		long int timeStamp = (tv.tv_sec - startTimeSec) * 1000000 + (tv.tv_usec - startTimeuSec);
		//cout << sinWave[i] << " " << i << " " << value << endl;
		fprintf(pFile,"%i\t%f\t%ld\n",value, sinWave[i], timeStamp);
		
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
		//mask::RAW_ACCEL_X | mask::RAW_GYRO_X,
		//mask::RAW_ACCEL_X | mask::RAW_GYRO_X | mask::RAW_RIGHT_MOTOR_BACK_EMF,
		mask::RAW_ACCEL_X | mask::RAW_GYRO_X | mask::RAW_RIGHT_MOTOR_BACK_EMF | mask::FILTERED_RIGHT_MOTOR_BACK_EMF

	};

	int freqArr[] = {
		53,
		20,
		30,
		40,
		50,
		60
	};

	long int currentTimeSec;
	long int currentTimeuSec;


	usleep(2000000);
	sph->setDataStreaming(50, 1, mask::RAW_RIGHT_MOTOR_PWM, 0, 0);

	

	//for (int j = 0; j < 1; j++)
	//{
		//freq = freqArr[j];
		//for (int i = 0; i < 2; i++) {
			// Update timestamp to use in datalogging
			gettimeofday(&tv, NULL);
			startTimeSec = tv.tv_sec;
			startTimeuSec = tv.tv_usec;	

			// Update time values to use to stop looping
			currentTimeSec = startTimeSec;
			currentTimeuSec = startTimeuSec;

			// Start datastreaming with new values.
			//sph->setDataStreaming(freq, 1, sensorArr[i], 0, 0);
			//sensorsStreaming = sph->getNumberOfStreaming();

			
			// Loop for 30 sec. 
			while (currentTimeSec- startTimeSec < 30)
			{

				
				//Used when Random send		
				
				//usleep((rand() % 10 + 1) * 10000); // Wait a random time between 0.01sec. and 0.1sec.
			    // Send a command
				//sph->roll(20, (rand() % 100 + 1), 1); // Random direction
				

				
				// Update time
				gettimeofday(&randomSend, NULL); 
				currentTimeSec = randomSend.tv_sec;
				currentTimeuSec = randomSend.tv_usec;

			}			
	

			//cout << "j: " << j << " i: " << i << endl;
			
		//}
		
		
	//}
	
	fclose(pFile);
	return 0;
}