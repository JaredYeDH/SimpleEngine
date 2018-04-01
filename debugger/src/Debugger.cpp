#include "Debugger.h"

DebugSession::DebugSession(){}

DebugSession::DebugSession(int port)
:m_Port(port)
{
    std::cout << port << std::endl;
	asio::ip::tcp::resolver resolver(m_IOContext);
	auto endpoints = resolver.resolve("127.0.0.1",std::to_string(port));
	m_Socket = new asio::ip::tcp::socket(m_IOContext);
	asio::async_connect(*m_Socket, endpoints,
		[this](std::error_code ec, asio::ip::tcp::endpoint)
	{
		if (!ec)
		{
			std::cout  << "Debug Session连接成功" << std::endl;
            while(true);
		}
        else
        {
            std::cout  << "error:" << ec.message() << std::endl;


        }
	});
	m_IOContext.run();
}

DebugSession::~DebugSession()
{

}

void DebugSession::read()
{

}

void DebugSession::write()
{


}

void DebugSession::run()
{

}

void Dispatcher::handleMessage(DebugSession* session, String msg)
{

}
