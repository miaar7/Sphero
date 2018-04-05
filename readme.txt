To intall bluetooth package:
	sudo apt-get update
	sudo apt-get upgrade -y 
	sudo apt-get install bluez
	sudo apt-get install blueman
	sudo apt-get install libbluetooth-dev

//Check that the adpater is running
	systemctl status bluetooth

If not: 
	sudo systemctl start bluetooth

// If you want to crosscompile from visual studio

Follow this guide: https://blogs.msdn.microsoft.com/vcblog/2016/03/30/visual-c-for-linux-development/

// If problems ocurre when debugging from visual studio, it might be because the debugger isn´t run as root.
// To solve this, create a user with root permisions!  - Not recomended for anything else than testing!!
	
	sudo useradd -ou 0 -g 0 DesiredUsername
	sudo passwd DesiredUsername
Create default home directory
	mkhomedir_helper Desiredusername
	
Allow root login on ssh:

	Edit this file: sudo nano /etc/ssh/sshd_config
	Find this line: #PermitRootLogin without-password
	Edit: PermitRootLogin yes
	Close and save file
	reboot or restart sshd service using: /etc/init.d/ssh restart

// You can now login as the root-user from E.G putty or Visual studio.








usefull links

https://stackoverflow.com/questions/29767053/bluez-programming
https://stackoverflow.com/questions/29309533/how-to-program-a-bluetooth-le-device-using-c-on-linux-x86

https://github.com/jdelacroix/sphero-bluecove/blob/master/src/com/jpdelacroix/sphero/packets/SpheroDataStreamingOptions.java



/// Connect to Sphero with gatttool: 
					gatttool -t random -b CD:72:26:BA:E7:32 -I --sec-level=high
get primary services: 			 primary
get characteristics    			characteristics <handle> <handle>

Sphero UUIDs: 

Adaptor.BLEService = "22bb746f2bb075542d6f726568705327";
Adaptor.WakeCharacteristic = "22bb746f2bbf75542d6f726568705327"; handle 0x00
Adaptor.TXPowerCharacteristic = "22bb746f2bb275542d6f726568705327";
Adaptor.AntiDosCharacteristic = "22bb746f2bbd75542d6f726568705327";
Adaptor.RobotControlService = "22bb746f2ba075542d6f726568705327";
Adaptor.CommandsCharacteristic = "22bb746f2ba175542d6f726568705327";
Adaptor.ResponseCharacteristic = "22bb746f2ba675542d6f726568705327";


