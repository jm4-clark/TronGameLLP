#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
//#include "stdafx.h"

#include <future>
#include <iostream>
#include <string>

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;

const sf::IpAddress SERVER_IP("127.0.0.1");
constexpr int SERVER_TCP_PORT(53000);

void client();
bool connect(TcpClient&);
void input(TcpClient&);

int main()
{	
	sf::RenderWindow window(sf::VideoMode(400, 400), "TRON!", sf::Style::Close | sf::Style::Resize);
	sf::RectangleShape player(sf::Vector2f(100.0f, 100.0f));
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	player.setFillColor(sf::Color::Blue);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				/*std::cout<<"new window width:" << event.size.width <<" new window Height:"<< event.size.height<<std::endl;
				printf("New window width: %i New window Height: %i", event.size.width, event.size.height);*/
	
				break;
			case sf::Event::TextEntered:
				if (event.text.unicode<128)
				{
					//printf("%c", event.text.unicode);
				}
	
				break;
			}
	
	
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			//player.move()
		}
	
		window.clear();
		window.draw(shape);
		window.draw(player);
		window.display();
		client();
	
	}
	
	return 0;
}
	
void client()
{
	TcpClient socket;
	if (!connect(socket))
	{
		return;
	}

	auto handle = std::async(std::launch::async, [&]
	{
		// keep track of the socket status
		sf::Socket::Status status;
		do
		{
			sf::Packet packet;
			status = socket.receive(packet);
			if (status == sf::Socket::Done)
				{
				// Recvie message from sever
				std::string message;
				packet >> message;
			}
		} while (status != sf::Socket::Disconnected);
});

	//std::thread inputThread(&input, socket); 
	return input(socket);
}

bool connect(TcpClient &socket)
{
	sf::Clock ping;
	sf::Time t1 = ping.getElapsedTime();

	auto status = socket.connect(SERVER_IP, SERVER_TCP_PORT);
	if (status != sf::Socket::Done)
	{
		return false;
	}
	return true;

}
	
void input(TcpClient &_client)
{
	while (true)
	{
		auto& sender_ref = _client;
	
		//send message to sever
		sf::Packet packet;
		std::string message = "HelloWorld";
		packet << message;

		_client.send(packet);
	}
}