#pragma once
#include "Message.h"
class Client : public std::enable_shared_from_this<Client>
{
public:
	Client(asio::io_context& io_context,
		const tcp::resolver::results_type& endpoints)
		:m_IOContext(io_context),
		m_Socket(io_context)
	{
		DoConnect(endpoints);
	};
	

	~Client()
	{
	};	

	void Write(const MoveMessage& msg)
	{
		asio::post(m_IOContext,
			[this, msg]()
		{
			bool write_in_progress = !m_WriteMessages.empty();
			m_WriteMessages.push_back(msg);
			if (!write_in_progress)
			{
				
				DoWrite();
			}
		});
	}

	void Close()
	{
		asio::post(m_IOContext, [this]() { m_Socket.close(); });
	}

private:

	void DoConnect(const tcp::resolver::results_type& endpoints)
	{
		asio::async_connect(m_Socket, endpoints,
			[this](std::error_code ec, tcp::endpoint)
		{
			if (!ec)
			{
				DoReadHeader();
			}
		});
	}

	void DoReadHeader()
	{
		asio::async_read(m_Socket,
			asio::buffer(m_ReadMessage.Data(), MoveMessage::HEADER_LEN),
			[this](std::error_code ec, std::size_t /*length*/)
		{
			if (!ec && m_ReadMessage.DecodeHeader())
			{
				DoReadBody();
			}
			else
			{
				m_Socket.close();
			}
		});
	}

	void DoReadBody();

	void DoWrite()
	{
		asio::async_write(m_Socket,
			asio::buffer(m_WriteMessages.front().Data(),
				m_WriteMessages.front().Length()),
			[this](std::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				m_WriteMessages.pop_front();
				if (!m_WriteMessages.empty())
				{
					DoWrite();
				}
			}
			else
			{
				m_Socket.close();
			}
		});
	}


	asio::io_context& m_IOContext;
	tcp::socket m_Socket;
	MoveMessage m_ReadMessage;
	std::deque<MoveMessage> m_WriteMessages;
};
