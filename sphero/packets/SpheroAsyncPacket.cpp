/*************************************************************************
	SpheroAsyncPacket - Defines asynchronous packets behavior received by Sphero
							 -------------------
	début                : mar. 17 mars 2015
*************************************************************************/


//-------------------------------------------------------- System includes
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>



//--------------------------------------------------------- Local includes
#include "SpheroAsyncPacket.hpp"
#include "async/SpheroCollisionPacket.hpp"
#include "async/SpheroStreamingPacket.hpp"

//-------------------------------------------------------- Class variables
extractorMap_t SpheroAsyncPacket::_extractorMap = {
	{COLLISION_DETECTED, SpheroCollisionPacket::extractPacket},
	{SENSOR_DATA_STREAMING, SpheroStreamingPacket::extractPacket}
};

//------------------------------------------------ Constructors/Destructor

/**
 * @brief SpheroAsyncPacket : Constructor
 * @param sphero : The Sphero instance that receives the asynchronous answer packet
 */
SpheroAsyncPacket::SpheroAsyncPacket(Sphero* sphero) : SpheroPacket(sphero)
{}

SpheroAsyncPacket::~SpheroAsyncPacket()
{}


//--------------------------------------------------------- Public methods

/**
 * @brief extractPacket : extracts the packet from a socket
 * @param fd : The socket file descriptor
 * @param sphero : The Sphero sending the packet
 * @param packet_ptr : A pointer to a SpheroPacket pointer
 * @return true if the packet was successfully extracted from the socket, false otherwise
 *
 * Contract: the socket has to be in blocking read
 */
bool SpheroAsyncPacket::extractPacket(Sphero* sphero, SpheroPacket** packet_ptr, vector<uint8_t>* data)
{

#ifdef MAP
	fprintf(stdout, "Asynchronous packet reception\n");
#endif
	
	if (data->size() > 2) 
	{
		extractorMap_t::iterator mapIt = _extractorMap.find((*data)[2]);
		if (mapIt != _extractorMap.end())
		{
			return mapIt->second(sphero, packet_ptr, data);
		}
	}	
	return false;
}



