#pragma once

struct Message;
class INetworkReceiver abstract
{
public:
	virtual void ReceiveMessage(Message* pMsg) abstract;
};