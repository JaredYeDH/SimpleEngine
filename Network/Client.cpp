#include "Client.h"
#include "../Engine.h"
void Client::DoReadBody()
{
	asio::async_read(m_Socket,
		asio::buffer(m_ReadMessage.Body(), m_ReadMessage.BodyLength()),
		[this](std::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			m_ReadMessage.DecodeBody();
			Engine::GetInstance()->DispatchMove(m_ReadMessage);
			std::cout.write(m_ReadMessage.Body(), m_ReadMessage.BodyLength());
			std::cout << "\n";
			DoReadHeader();
		}
		else
		{
			m_Socket.close();
		}
	});
}