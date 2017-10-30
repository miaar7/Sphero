
#include <iostream>
#include <deque>
#include <unistd.h>
#include <pthread.h>
#include <cstdint>

using namespace std;

#include "DataBuffer.h"

DataBuffer::DataBuffer()
{
	_dataValues = new uint16_t[27];
	for(int i = 0; i < 27; ++i)
		_dataValues[i] = 0;

	pthread_mutex_init(&lock, NULL);
}

DataBuffer::~DataBuffer()
{
	delete[] _dataValues;
}


/**
 * @brief waitForNext : Waits for the next value in the list
 * @param valueType : The value type
 * @param returnValue : a reference to the variable in which will be placed the value
 * @param wait : the wait time (in µs) between each try. If -1, the function isn't blocking
 * @param timeout : The trying timeout time (in µs)
 * @return true if the value has been found, false if the timeout came before
 */
bool DataBuffer::waitForNext(dataTypes valueType, uint16_t &returnValue, int wait, int timeout)
{
	int remaining = timeout / wait + 1;

	do
	{
		pthread_mutex_lock(&lock);
		returnValue = _dataValues[valueType];

		pthread_mutex_unlock(&lock);
		return true;
			
		/*pthread_mutex_unlock(&lock);

		if(wait < 0)
			return false;
		usleep(wait);

		--remaining;*/
	}while(remaining > 0);

	return false;
}

bool DataBuffer::getValue(dataTypes valuetype, uint16_t &returnValue)
{
	pthread_mutex_lock(&lock);
	returnValue = _dataValues[valuetype];
	pthread_mutex_unlock(&lock);
	return true;
}


/**
 * @brief flush : Flushes the specified queue (can be used when configuring locator, to avoid meaningless)
 * @param valueType
 */
void DataBuffer::flush(dataTypes valueType)
{
	pthread_mutex_lock(&lock);
	_dataValues[valueType] = 0;
	pthread_mutex_unlock(&lock);
}


/**
 * @brief addValue : Adds a new value to the type queue
 * @param valueType : The type of the value
 * @param value : The new value
 */
void DataBuffer::addValue(dataTypes valueType, uint16_t value)
{
	pthread_mutex_lock(&lock);

	_dataValues[valueType] = value;

	pthread_mutex_unlock(&lock);
}

