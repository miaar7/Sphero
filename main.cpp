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

	string address = "CD:72:26:BA:E7:32";


	Sphero* sph = new Sphero(address.c_str(), new bluez_adaptor());

	DataBuffer* sensors = sph->getDataBuffer();

	sph->onConnect([]() {
		cout << "Here I come, honourable people !" << endl;
	});

	sph->onData([sensors]() {
		uint16_t value;
		sensors->getValue(dataTypes::FILTERED_ROLL_IMU, value);
		printf("%u:\n ", value);
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
	sph->setStabilization(0);
	int i = 0;
	while (1)
	{
		usleep(10000000);
	}
	return 0;
}