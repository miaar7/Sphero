/*************************************************************************
                           SpheroAnswerPacket  -  description
                             -------------------
    début                : mar. 17 mars 2015
*************************************************************************/

//------ Réalisation de la classe <SpheroAnswerPacket> (fichier SpheroAnswerPacket.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- System includes
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>


//--------------------------------------------------------- Local includes
#include "../Sphero.hpp"
#include "SpheroAnswerPacket.hpp"
#include "ChecksumComputer.hpp"
#include "answer/BTInfoStruct.hpp"

//------------------------------------------------ Constructors/Destructor

/**
 * @brief SpheroAnswerPacket : Constructor
 * @param sphero : The Sphero instance that receives the answer packet
 */
SpheroAnswerPacket::SpheroAnswerPacket(Sphero* sphero) : SpheroPacket(sphero)	
{}


SpheroAnswerPacket::~SpheroAnswerPacket()
{}


//--------------------------------------------------------- Public methods

/**
 * @brief extractPacket : extracts the packet from a socket
 * @param fd : The socket file descriptor
 * @param sphero : The Sphero sending the packet
 * @param packet_ptr : A pointer to a SpheroPacket pointer
 * @return true if the packet was successfully extracted from the socket, false otherwise
 *
 * Contract: the socket has to be in blocking read mode
 */
bool SpheroAnswerPacket::extractPacket(Sphero* sphero, SpheroPacket** packet_ptr, vector<uint8_t>* data)
{
	uint8_t msgrsp;
	uint8_t seq;
	uint8_t* dataPayload = NULL;
	uint8_t dlen;
	uint8_t checksum;

	if (data->size() > 5) // Checck that we have at least: msgrasp, seq, dlen and chk. 
	{
		msgrsp = (*data)[2];
		seq = (*data)[3];
		dlen = (*data)[4];		
	} else
	{
		return false;
	}

	if(dlen > 1) // If there is any payload
	{
		if (data->size() > 5 + dlen - 1) // Check that we recieved the payload and checksum
		{
			dataPayload = (uint8_t*)malloc((dlen - 1) * sizeof(uint8_t));

			std::copy(data->begin() + 5, data->begin() + 5 + dlen - 1, dataPayload);
			checksum = data->back();
		} else
		{
			free(dataPayload);
			return false;
		}
	}
	else
	{
		checksum = checksum = data->back();
	}
	
	ChecksumComputer cc;
	cc.addField(seq);
	cc.addField(msgrsp);
	cc.addField(dlen);

	if(dataPayload != NULL)
	{
		cc.addField(dataPayload, dlen - 1);
	}

	if(cc() != checksum)
	{
#ifdef MAP
		fprintf(stderr, "Erreur de checksum\n");
#endif
		if(dataPayload != NULL)
		{
			free(dataPayload);
			return false;
		}
	}
	
#ifdef MAP
	fprintf(stdout, "msgrsp : %u ;\nseq : %u;\n", msgrsp, seq);
#endif

	sphero->lockSeqnum(seq);
	pendingCommandType requested = sphero->getTodo(seq);
	packetFormatter pf = getPacketFromTodo(requested);
	void* retour = pf(dlen, dataPayload);

	sphero->notifyPacket(seq, msgrsp, retour);
	
	sphero->unlockSeqnum(seq);
	
	if(dataPayload != NULL)
	{
		free(dataPayload);
	}

	return false;
}

//------------------------------------------------------------- Private methods

packetFormatter SpheroAnswerPacket::getPacketFromTodo(pendingCommandType todo)
{
	switch(todo){
		case pendingCommandType::GETCOLOR:
			return [](uint8_t dlen, uint8_t* dataPayload){
				if (dlen != 0x04)
				{
					return (void*) NULL;
				}
				ColorStruct* color = new ColorStruct;
				color->red = dataPayload[0];
				color->green = dataPayload[1];
				color->blue = dataPayload[2];
				return (void*) color;
			};
			break;
		case pendingCommandType::GETBTINFO:
			return [](uint8_t dlen, uint8_t* dataPayload){
				if (dlen != 0x21)
				{
					return (void*) NULL;
				}
				BTInfoStruct* btinfo = new BTInfoStruct;
				
				btinfo->bt_name = std::string((const char*)dataPayload);
				btinfo->bt_adress = std::string((const char*)(dataPayload + 16));
				return (void*) btinfo;

			};
			break;
		case pendingCommandType::NONE:
		case pendingCommandType::SIMPLE_RESPONSE:
		default:
			return [](uint8_t, uint8_t* ){ return (void*) NULL; };
			break;
	}
}
