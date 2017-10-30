/*************************************************************************
	SpheroPacket  -  Abstract class. Defines the behavior of
												packets received by Sphero
							 -------------------
	started                : 06/05/2015
*************************************************************************/


//-------------------------------------------------------- System includes
#include <iostream>
#include <limits>
#include <sys/socket.h>
#include <unistd.h>


//--------------------------------------------------------- Local includes
#include "../Sphero.hpp"
#include "SpheroPacket.hpp"
#include "SpheroAsyncPacket.hpp"
#include "SpheroAnswerPacket.hpp"

//-------------------------------------------------------- Class variables
extractorMap_t SpheroPacket::_extractorMap = {
	{ASYNC_FLAG, SpheroAsyncPacket::extractPacket},
	{ANSWER_FLAG, SpheroAnswerPacket::extractPacket}
};


//------------------------------------------------ Constructors/Destructor

/**
 * @brief SpheroPacket::SpheroPacket
 * @param sphero : The Sphero instance that receives the packet
 */
SpheroPacket::SpheroPacket(Sphero* sphero):_sphero(sphero)
{}


SpheroPacket::~SpheroPacket ( )
{}


//--------------------------------------------------------- Public methods

/**
 * @brief extractPacket : extracts informations from the file descriptor to build a well made packet
 * @param fd : The socket file descriptor
 * @param sphero : The Sphero sending the packet
 * @param packet_ptr : A pointer to a SpheroPacket pointer
 * @return true if the packet was successfully extracted from the socket, false otherwise
 *
 * Contract: the socket has to be in blocking read
 */
bool SpheroPacket::extractPacket(Sphero* sphero, SpheroPacket** packet_ptr, vector<uint8_t>* data)
{
	
	int rcvVal = 0;
	if (data->size() > 1)
	{
		if ((*data)[0] == START_OF_PACKET_FLAG)
		{

			extractorMap_t::iterator mapIt = _extractorMap.find((*data)[1]);

			if (mapIt == _extractorMap.end())
			{
				return false;
			}
			else
			{
				// Second is a pointer tp either SpheroAsyncPacket::extractPacket or SpheroAnswerPacket::extractPacket depending on the flag
				return mapIt->second(sphero, packet_ptr, data);
			}
		}
	}
}
