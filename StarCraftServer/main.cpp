#include "stdafx.h"
#include "Server.h"

int main()
{
	boost::asio::io_service ioService;
	Server server(ioService);
	server.Start();
	ioService.run();

	std::wcout << "네트워크 접속 종료" << std::endl;

	getchar();

	return 0;
}