/*************************************************************************
   SpheroStreamingPacket  - Represents an asynchronous collision packet
							 -------------------
	début                : jeu. 07 mai 2015
*************************************************************************/

//-------------------------------------------------------- System includes
//#ifdef MAP
	#include <iostream>
//#endif


#include <sys/socket.h>
#include <endian.h>



using namespace std;

//--------------------------------------------------------- Local includes
#include "SpheroStreamingPacket.hpp"
#include "../../Sphero.hpp"
#include "../Toolbox.hpp"
#include "../Constants.hpp"
#include "DataBuffer.h"

//------------------------------------------------ Constructors/Destructor

/**
 * @brief SpheroStreamingPacket : Constructor
 * @param sphero : The Sphero instance that sends the collision packet
 */
SpheroStreamingPacket::SpheroStreamingPacket(
		Sphero* sphero):	SpheroAsyncPacket(sphero)
{}


SpheroStreamingPacket::~SpheroStreamingPacket ( )
{
#ifdef MAP
	std::cout << "<SpheroStreamingPacket> destructor called" << std::endl;
#endif
}


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
bool SpheroStreamingPacket::extractPacket(Sphero* sphero, SpheroPacket** packet_ptr, vector<uint8_t>* data)
{
	uint16_t len;

	sphero->requestLock();
	if (data->size() > 4)
	{
		len = (*data)[3] << 8;
		len |= (*data)[4];
	}

	if(!sphero->checkValid(len))
	{
		sphero->requestLock(false);
		return false;
	}

	if (data->size() > 5 + len - 1)
	{
		for (int i = 0; i < (len - 1) / 2; i++)
		{
			uint16_t value;
			value = (*data)[5 + i] << 8;
			value |= (*data)[6 + i];

			sphero->getDataBuffer()->addValue(sphero->getTypesList()[i%sphero->getTypesList().size()], value);
		}
	}
	sphero->requestLock(false);

	*packet_ptr = new SpheroStreamingPacket(sphero);
	return true;
}


/**
 * @brief packetAction : Performs the action associated to the packet
 *			on the Sphero instance
 */
void SpheroStreamingPacket::packetAction()
{
	_sphero->reportData();
}
