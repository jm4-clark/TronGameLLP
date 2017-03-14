#include "Client.h"
#include <SFML/Graphics.hpp>

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;

const sf::IpAddress SERVER_IP("127.0.0.1");
constexpr int SERVER_TCP_PORT(53000);



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
			}
	
	
		}
		Client cPtr;
		cPtr.client();
		window.clear();
		window.draw(shape);
		window.draw(player);
		window.display();
		
	
	}
	
	return 0;
}
	
void Client::client()
{
	if (!connect())
	{
		return;
	}

	auto handle = std::async(std::launch::async, [&]
	{
//keep track of the socket status
		sf::Socket::Status status;
			sf::Packet packet;
			status = socket.receive(packet);
			if (status != sf::Socket::Disconnected)
			{
				if (status == sf::Socket::Done)
				{
					// Recieve message from server
					std::string message;
					packet >> message;
				}
			}
});

	//std::thread inputThread(&input, socket); 

	return input();
}

bool Client::connect()
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
	
Client::Client()
{
}

Client::~Client()
{
}

void Client::input()
{
	
	auto& sender_ref = socket;

	//send message to sever
	sf::Packet packet;
	std::string message = "HelloWorld";
	packet << message;
	socket.send(packet);

}