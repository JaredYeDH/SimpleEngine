#pragma once
#include "defines.h"
using asio::ip::tcp;
class DebugSession
{
public:
	DebugSession();
	~DebugSession();

private:

};

class RawBuff
{
public:
	RawBuff()
		:m_Buffsize(1024)
	{
	};

	~RawBuff()
	{

	};
	
	char* buff(){ return m_Buff; };

	int buffsize(){ return m_Buffsize; };

	void print(){ std::cout <<std::string(m_Data)<< std::endl; };

	void clear() { m_ContentSize = 0; };

	void appendBuf() { 
		strncpy(&m_Data[m_ContentSize], m_Buff, m_Buffsize); 
		m_ContentSize += m_Buffsize; 
	};

	int size() { return m_ContentSize; };
private:
	char m_Buff[1024];
	int m_Buffsize;
	int m_ContentSize;
	char m_Data[4096];
};



class DebugServer
{
public:

	DebugServer(int port)

	{
		tcp::endpoint localhost(tcp::v4(), port);
		m_Acceptor = new tcp::acceptor(m_IOContext,localhost);
	};
	~DebugServer(){};
	void doRead()
	{
		asio::async_read(*m_Socket,
			asio::buffer(buffs2, 50),
			[this](std::error_code ec, std::size_t len)
		{
			if (!ec)
			{
				int i = 10;
				std::cout << std::string(buffs2, 0, len) << "\t len10:" << len << std::endl;
				//m_RawBuff.appendBuf();
			}
			else
			{
				int j = 20;
				std::cout << std::string(buffs2, 0, len) << "\t len20:" << len << std::endl;
			}
			doRead();
		});
	}
	void listen(){
		
		m_Acceptor->async_accept(
			[this](std::error_code ec, tcp::socket socket)
		{
			m_Socket = new  tcp::socket(std::move(socket));
			
			doRead();
				
			
		});
		
		m_IOContext.run();
	
	
	}
private:
	char buffs2[512];
	tcp::acceptor* m_Acceptor;
	RawBuff m_RawBuff;
	asio::io_context m_IOContext;
	tcp::socket* m_Socket;
};
