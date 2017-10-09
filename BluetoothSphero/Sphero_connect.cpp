#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <time.h>

int main(int argc, char **argv)
{
	struct sockaddr_l2 addr = { 0 };
	struct sockaddr_l2 loc_addr = { 0 };
	int s, status;
	char dest[18] = "CD:72:26:BA:E7:32";

	//                        SOP1	SOP2   DID	CID	  SEQ	DLEN  dat1   dat2  dat3  flag CHK
	// 0x52 = Write_cmd

	char gattmessage[8] = {0x52, 0x2a, 0x00, 0x30, 0x31, 0x31 ,0x69 ,0x33 };
	char message1[10] = { 0x52, 0x17, 0x00, 0x28, 0x44, 0x00, 0x00, 0x20, 0x00, 0x02 };
	char message2[4] = { 0x52, 0x2f, 0x00, 0x00 };
	char message3[14] = { 0x52, 0x0e, 0x00, 0xff, 0xfe, 0x02, 0x30, 0x00, 0x05, 0x96, 0x00, 0x14, 0x01, 0x1d };
	//0x000e value  0xff 0xff 0x02 0x30 0x00 0x05 0x96 0x00 0x14 0x01 0x1d
	//40D = 1037 % 256
	//13 = 00001101
	// 11110010 = F2
	// Open a socket
	s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (s < 0) {
		perror("Unable to open socket");
	}

	// Connection parameters 
	addr.l2_family = AF_BLUETOOTH;
	addr.l2_cid = htobs(4);
	str2ba(dest, &addr.l2_bdaddr);
	addr.l2_bdaddr_type = BDADDR_LE_RANDOM;

	// Setings for binding to socket.
	loc_addr.l2_family = AF_BLUETOOTH;
	loc_addr.l2_bdaddr = (bdaddr_t) { {0, 0, 0, 0, 0, 0} };
	loc_addr.l2_cid = htobs(4);
	loc_addr.l2_bdaddr_type = BDADDR_LE_RANDOM;
	

	if (bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0)
	{
		perror("Unable to bind");
	}

	printf("Connecting...\n");
	// Connect to server
	if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Unable to connect");
		return -1;
	}
	
	printf("Connected to: %s\n", dest);
	//while (1) {

		status = write(s, gattmessage, 8);
		if (status < 0)
		{
			perror("Unable to write");
			return -1;
		}
		printf("Message Send  \n");
		
		status = write(s, message1, 10);
		if (status < 0)
		{
			perror("Unable to write");
			return -1;
		}
		printf("Message1 Send  \n");
		status = write(s, message2, 4);
		if (status < 0)
		{
			perror("Unable to write");
			return -1;
		}
		printf("Message2 Send  \n");

		while (1)
		{
			status = write(s, message3, 14);
			if (status < 0)
			{
				perror("Unable to write");
				return -1;
			}
			printf("Message3 Send  \n");
			usleep(1000000);
		}


	//}

	close(s);
	return 0;
}