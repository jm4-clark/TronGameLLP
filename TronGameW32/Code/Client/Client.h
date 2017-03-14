#pragma once
#include <SFML/Network.hpp>

//#include "stdafx.h"
#include <future>
#include <iostream>
#include <string>

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;


class Client
{
public:
	Client();
	~Client();

	void input();
	void client();
	bool connect();

	
protected:
	TcpClient socket;


};
