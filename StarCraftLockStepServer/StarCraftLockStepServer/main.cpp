#include "stdafx.h"
#include "Server.h"

int main()
{
	boost::asio::io_service ioService;
	Server server(ioService);
	server.Start();
	ioService.run();

	std::wcout << "��Ʈ��ũ ���� ����\n";

	getchar();

	return 0;
}