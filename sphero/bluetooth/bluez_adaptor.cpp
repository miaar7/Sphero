/*************************************************************************
	bluez_adaptor  -  Bluez stack adaptor for Sphero
                             -------------------
	started                : 17/03/2015
*************************************************************************/

//-------------------------------------------------------- System includes
#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>

using namespace std;

//--------------------------------------------------------- Local includes
#include "bluez_adaptor.h"


//-------------------------------------------------------------- Constants
static const size_t MAX_CONNECT_ATTEMPT_BUSY = 3;

//------------------------------------------------ Constructors/Destructor


bluez_adaptor::bluez_adaptor():bluetooth_connector(), _bt_socket(0), _connected(false)
{
#ifdef MAP
	cout << "<bluez_adaptor> constructor called" << endl;
#endif
} //----- Fin de bluez_adaptor


bluez_adaptor::~bluez_adaptor()
{
#ifdef MAP
	cout << "<bluez_adaptor> destructor called" << endl;
#endif
	if(_connected)
	{
		disconnect();
	}
}

//--------------------------------------------------------- Public methods

/**
 * @brief connection : Establishes the connection
 * @param address : Thedevice address (format : 'XX:XX:XX:XX:XX')
 * @return The socket ID, or -1 if an error occurred
 */
int bluez_adaptor::connection(const char* address)
{
	
	int errval = 0;
	bool continuer = true;
	size_t i;

	if(_connected)
	{
		fprintf(stderr, "Connection attempt was made but peripheral was already connected\n");
		return _bt_socket;
	}
	
	bdaddr_t bt_address;

	//Address conversion
	if(str2ba(address, &bt_address))
	{
		fprintf(stderr, "BT address conversion error");
		return -1;
	}

	// Connection parameters 
	struct sockaddr_l2 dest_addr = { 0 };
	dest_addr.l2_family = AF_BLUETOOTH;
	dest_addr.l2_cid = htobs(4);
	dest_addr.l2_bdaddr = bt_address;
	dest_addr.l2_bdaddr_type = BDADDR_LE_RANDOM;

	// Setings for binding to socket.
	struct sockaddr_l2 loc_addr = { 0 };
	loc_addr.l2_family = AF_BLUETOOTH;
	loc_addr.l2_bdaddr = (bdaddr_t) { {0, 0, 0, 0, 0, 0} };
	loc_addr.l2_cid = htobs(4);
	loc_addr.l2_bdaddr_type = BDADDR_LE_RANDOM;

	_bt_socket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (_bt_socket < 0) {
		perror("Unable to open socket");
		return -1;
	}

	if (bind(_bt_socket, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0)
	{
		perror("Unable to bind");
		return -1;
	}

	while(continuer)
	{
		errval = connect(_bt_socket, (sockaddr*) &dest_addr, sizeof(dest_addr));
		if(errval >= 0)
		{
			continuer = false;
		}
		
		else if(errno == EBUSY && i++ < MAX_CONNECT_ATTEMPT_BUSY)
		{
			usleep(500000);
		}

		else
		{
			perror("BT connection");
			return -1;
		}
	}

	_connected = true;
	return _bt_socket;
}


/**
 * @brief disconnect : Closes the connections
 * @return An error code
 */
int bluez_adaptor::disconnect(void)
{
	_connected = false;
	return close(_bt_socket);
}


/**
 * @brief isConnected : Checks the connection status
 * @return true if the connection is currently established
 */
bool bluez_adaptor::isConnected()
{
	return _connected;
}
