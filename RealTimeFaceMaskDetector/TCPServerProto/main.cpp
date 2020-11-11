#pragma once
#include "SocketServer.h"

int  main(void)
{
	SocketServer SS;
	SS.InitSocketServer();
	SS.CreateListeningSocket();
	SS.StartListening();
	SS.AcceptConnection();
	SS.ReceiveMessage();
	SS.ShutdownServer();
	return 0;
}