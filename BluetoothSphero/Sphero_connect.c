#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <time.h>

// ------- Defines ------//

// RobotControlService  handle 0x000c to 0x0011
#define CommandsCharacteristic 0x0e //0x000e
#define ResponseCharacteristic 0x10 //0x0010
// BLEService  handle: 0x0012 to 0x0033
#define WakeCharacteristic   0x2f //0x002f
#define TXPowerCharacteristic  0x14 //0x0014
#define AntiDosCharacteristic   0x2a // 0x002a
// ATT 
#define WRITE 0x52

int connectSphero(int* s, char* dest, struct sockaddr_l2 *addr, struct sockaddr_l2 *loc_addr);
int send_message(int* bl_sock, char* message, int length);
int wakeSphero(int*bl_sock);

int main(int argc, char **argv)
{
	struct sockaddr_l2 addr = { 0 };
	struct sockaddr_l2 loc_addr = { 0 };
	int s;
	int status;
	char dest[18] = "CD:72:26:BA:E7:32"; // Address of Sphero

	char command[14] = { WRITE, CommandsCharacteristic, 0x00, 0xff, 0xfe, 0x02, 0x30, 0x00, 0x05, 0x96, 0x00, 0x14, 0x01, 0x1d };
	
	// Open a socket	
	if (connectSphero(&s, &dest, &addr, &loc_addr) < 0) {
		return -1;
	}

	if (wakeSphero(&s) < 0) {
		return -1;
	}
	while (1) 
	{
		if (send_message(&s, &command, sizeof(command)) < 0)
		{
			return -1;
		}
		usleep(1000000);
	}
	close(s);
	return 0;
}

int connectSphero(int* s,char* dest, struct sockaddr_l2 *addr, struct sockaddr_l2 *loc_addr) {

	// Connection parameters 
	addr->l2_family = AF_BLUETOOTH;
	addr->l2_cid = htobs(4);
	str2ba(dest, &(addr->l2_bdaddr));
	addr->l2_bdaddr_type = BDADDR_LE_RANDOM;

	// Setings for binding to socket.
	loc_addr->l2_family = AF_BLUETOOTH;
	loc_addr->l2_bdaddr = (bdaddr_t) { {0, 0, 0, 0, 0, 0} };
	loc_addr->l2_cid = htobs(4);
	loc_addr->l2_bdaddr_type = BDADDR_LE_RANDOM;
	int sock = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	*s = sock;
	if (*s < 0) {
		perror("Unable to open socket");
		return -1;
	}

	if (bind(*s, (struct sockaddr *)loc_addr, sizeof(*loc_addr)) < 0)
	{
		perror("Unable to bind");
		return -1;
	}

	printf("Connecting...\n");
	// Connect to server
	if (connect(*s, (struct sockaddr *)addr, sizeof(*addr)) < 0)
	{
		perror("Unable to connect");
		return -1;
	}
	
	return 0;
	
}
int send_message(int *bl_sock, char* message, int length) {
	printf("Sending message  \n");
	int status = write((*bl_sock), message, length);
	return status;
}
int wakeSphero(int*bl_sock) {
	
	char message1[8] = { WRITE, AntiDosCharacteristic, 0x00, 0x30, 0x31, 0x31 ,0x69 ,0x33 };
	if (send_message(bl_sock, &message1, sizeof(message1)) < 0)
	{
		return -1;
	}
	char message2[10] = { WRITE, TXPowerCharacteristic, 0x00, 0x28, 0x44, 0x00, 0x00, 0x20, 0x00, 0x02 };
	if (send_message(bl_sock, &message2, sizeof(message2)) < 0)
	{
		return -1;
	}
	char message3[4] = { WRITE, WakeCharacteristic, 0x00, 0x00 };
	if (send_message(bl_sock, &message3, sizeof(message3)) < 0)
	{
		return -1;
	}
	return 0;

}