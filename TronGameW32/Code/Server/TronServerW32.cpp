// TronServerW32.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <SFML\Network.hpp>

constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);
using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;

bool bindServerPort(sf::TcpListener& listener);
void listen(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients);
void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients);
void receiveMsg(TcpClients& tcp_clients, sf::SocketSelector&selector);
void runServer();

int main()
{
	std::cout << "Searching for life signs...";
	runServer();

	while (true)
	{
		std::cin.get();
	}

	return 0;
}

bool bindServerPort(sf::TcpListener&listener)
{
	if (listener.listen(SERVER_TCP_PORT) != sf::Socket::Done)
	{
		std::cout << "Could not bind server port" << std::endl;
		std::cout << std::endl << "Port: " << SERVER_TCP_PORT;
		std::cout << std::endl;
		return false;
	}
	return true;
}

void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	auto client_ptr = std::make_unique<sf::TcpSocket>();
	auto& client_ref = *client_ptr;
	if (tcp_listener.accept(client_ref) == sf::Socket::Done)
	{
		selector.add(client_ref);
		tcp_clients.push_back(std::move(client_ptr));
	}
}

void receiveMsg(TcpClients& tcp_clients, sf::SocketSelector&selector)
{	// loop through all tcp clients to find sender

	for (auto& sender : tcp_clients)
	{
		auto& sender_ref = *sender.get();
		if (selector.isReady(sender_ref))
		{
			sf::Packet packet;
			std::string string = "Test";
			packet << string;


			// print received string to console
			sf::Packet RecivedPacket;
			std::string clientMessage;
			for (auto& sender : tcp_clients)
			{
				(sender)->receive(RecivedPacket);
			}
			RecivedPacket >> clientMessage;
			std::cout << clientMessage << std::endl;

			//loop through all connected clients
			for (auto& sender : tcp_clients)
			{
				(sender)->send(packet);
			}
		}
	}
}

void listen(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	while (true)
	{
		if (selector.wait())
		{
			// this is a new connection request
			if (selector.isReady(tcp_listener))
			{
				connect(tcp_listener, selector, tcp_clients);
			}
			// this is a message being received
			else
			{
				receiveMsg(tcp_clients, selector);
			}
		}
	}
}

void runServer()
{
	sf::TcpListener tcp_listener;
	if (!bindServerPort(tcp_listener))
	{
		return;
	}

	sf::SocketSelector selector;
	selector.add(tcp_listener);
	TcpClients tcp_clients;

	return listen(tcp_listener, selector, tcp_clients);
}