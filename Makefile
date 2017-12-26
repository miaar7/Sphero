#Makefile for making the sphero project
#this makefile must be: chmod 755 Makefile
#call with this: ./Makefile

g++ \
main.cpp \
sphero/Sphero.cpp \
sphero/bluetooth/bluez_adaptor.cpp \
sphero/packets/ChecksumComputer.cpp \
sphero/packets/ClientCommandPacket.cpp \
sphero/packets/SpheroAnswerPacket.cpp \
sphero/packets/SpheroAsyncPacket.cpp \
sphero/packets/SpheroPacket.cpp \
sphero/packets/Toolbox.cpp \
sphero/packets/async/CollisionStruct.cpp \
sphero/packets/async/DataBuffer.cpp \
sphero/packets/async/SpheroCollisionPacket.cpp \
sphero/packets/async/SpheroStreamingPacket.cpp \
-lpthread -lbluetooth