#include "stdafx.h"
#include "Client.h"

int main()
{
	boost::asio::io_service ioService;
	Client client(ioService);
	std::cout << "이름을 입력하세요 (최대" << (MAX_NAME_LEN - 1) << "글자) : ";
	char name[MAX_NAME_LEN];
	std::cin >> name;
	client.SetName(name);
	std::cin.ignore();
	std::cin.clear();
	client.Connect();

	boost::thread thread(boost::bind(&boost::asio::io_service::run, &ioService));

	char inputMessage[MAX_TEXT_LEN] = { 0, };
	while (std::cin.getline(inputMessage, MAX_TEXT_LEN))
	{
		//if (strnlen_s(inputMessage, MAX_TEXT_LEN) == 0)
		//{
		//	break;
		//}

		if (false == client.IsConnected())
		{
			std::cout << "서버와 연결되지 않았습니다" << std::endl;
			continue;
		}

		MsgRoomText msg;
		sprintf_s(msg.Text, MAX_TEXT_LEN - 1, "%s", inputMessage);
		sprintf_s(msg.Name, MAX_NAME_LEN - 1, "%s", name);
		client.PostSend(false, msg.Size, (char*)&msg);
	}

	client.Close();

	getchar();

	return 0;
}