#pragma once
#include "defines.h"

using json=nlohmann::json;
using String = std::string;
class DebugSession
{
public:
	DebugSession();
	~DebugSession();

private:

};

class VSPotocol
{
	int seq;
	String type;		//request response event
};

class VSRequest :public VSPotocol
{
	String command;
	json arguments;
};

class VSEvent : public VSPotocol
{
	String event;
	json body;
};

class VSResponse : public VSPotocol
{
	int request_seq;
	bool success;
	String command;
	String message;
	json body;
};

class DebugServer;

class Dispatcher
{
public:	
	void handleMessage(DebugServer* server, String msg);
};


class DebugServer
{
public:
	DebugServer(int port);

	~DebugServer();
	
	int _sequence = 1;
	String wrapMsg(String type, json message)
	{
		message["type"] = type;
		message["seq"] = _sequence++;
		String msg = message.dump();
		std::cout << "readyToSend:" << std::endl
			<< msg << std::endl;

		int len = msg.length();
		String wrapped("");
		wrapped = wrapped + "Content-Length: " + std::to_string(len)
			+ "\r\n\r\n" + msg;
		return wrapped;
	}

	bool sendEvent(json& msg);

	bool sendResponse(json& msg);

	bool sendRequest(json& msg);

	void doRead()
	{
		asio::async_read(*m_Socket, asio::buffer(c, 1),
			[this](std::error_code ec, std::size_t len)
		{
			if (!ec)
			{
				mReadBuff[mReadBuffCurrentIndex++] = c[0];
				std::string s(mReadBuff.begin(), mReadBuff.begin() + mReadBuffCurrentIndex);
				std::regex regex_match_txt("Content-Length: ([0-9]+)\r\n\r\n");
				std::smatch base_match;
				if (std::regex_match(s, base_match, regex_match_txt))
				{
					if (base_match.size() == 2)
					{
						std::ssub_match lenstr = base_match[1];
						int len = std::stoi(lenstr.str());
						asio::async_read(*m_Socket, asio::buffer(msg, len),
							[this](std::error_code ec, std::size_t len)
						{
							if (!ec)
							{
								mReadBuffCurrentIndex = 0;
								for (int i = 0; i < len; i++)
								{
									mReadBuff[mReadBuffCurrentIndex++] = msg[i];
								}
								auto msgstr = std::string(mReadBuff.begin(), mReadBuff.begin() + mReadBuffCurrentIndex);
								mReadBuffCurrentIndex = 0;
								mMsgQueue.push_back(msgstr);
							}
							else
							{
								std::cout << ec.message() << std::endl;
							}
						});
					}
				}
			}
			else
			{
				std::cout << ec.message() << std::endl;
			}
			doRead();
		});
	}

	void backThread()
	{
		m_Acceptor->async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			m_Socket = new  asio::ip::tcp::socket(std::move(socket));
			doRead();
		});
		m_IOContext.run();
	}

	void run();

	void doWrite(std::string msg)
	{
		asio::async_write(*m_Socket,
			asio::buffer(msg.c_str(),
				msg.length()),
			[this](std::error_code ec, std::size_t /*length*/)
		{

		});
	}

private:

	asio::ip::tcp::acceptor* m_Acceptor;
	asio::io_context m_IOContext;
	asio::ip::tcp::socket* m_Socket;
	std::vector<char> mReadBuff;
	int mReadBuffCurrentIndex;
	char c[1];
	std::mutex io_mutex;
	char msg[4096];
	std::deque<std::string> mMsgQueue;
	Dispatcher m_Dispatcher;
};

//
//class RuntimeSession
//{
//public:
//	RuntimeSession();
//	~RuntimeSession();
//
//private:
//
//};
//
//RuntimeSession::RuntimeSession()
//{
//
//}
//
//RuntimeSession::~RuntimeSession()
//{
//
//}


#define  RAWBUFF_LENGTH 1024
class RawBuff
{
public:

	RawBuff()
		:m_Buffsize(RAWBUFF_LENGTH)
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
	char m_Buff[RAWBUFF_LENGTH];
	int m_Buffsize;
	int m_ContentSize;
	char m_Data[4096];
};



