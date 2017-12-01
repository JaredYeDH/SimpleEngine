#pragma once

#include "defines.h"
#include "Pos.h"

#include <asio.hpp>

using asio::ip::tcp;


class Message
{
public:
	enum
	{
		HEADER_LEN = 4,
		MAX_BODY_LEN = 1024
	};

	Message()
		:m_BodyLength(0)
	{
	};

	~Message() {};

	const char* Data() const
	{
		return m_Data;
	}

	char* Data()
	{
		return m_Data;
	}

	int Length() const
	{
		return HEADER_LEN + m_BodyLength;
	}
	const char* Body() const
	{
		return m_Data + HEADER_LEN;
	}
	char* Body()
	{
		return m_Data + HEADER_LEN;
	}

	int BodyLength() const
	{
		return m_BodyLength;
	}

	void BodyLength(int new_length)
	{
		m_BodyLength = new_length;
		if (m_BodyLength > MAX_BODY_LEN)
			m_BodyLength = MAX_BODY_LEN;
	}

	bool DecodeHeader()
	{
		char header[HEADER_LEN + 1] = "";
		std::strncat(header, m_Data, HEADER_LEN);
		m_BodyLength = std::atoi(header);
		if (m_BodyLength > MAX_BODY_LEN)
		{
			m_BodyLength = 0;
			return false;
		}
		return true;
	}

	void EncodeHeader()
	{
		char header[HEADER_LEN + 1] = "";
		std::sprintf(header, "%4d", static_cast<int>(m_BodyLength));
		std::memcpy(m_Data, header, HEADER_LEN);
	}


	virtual void EncodeBody() = 0;
	virtual void DecodeBody() = 0;
	int m_BodyLength;
private:
	
	char m_Data[MAX_BODY_LEN+HEADER_LEN];
};


class MoveMessage : public Message
{
public:
	friend std::istream & operator>>(std::istream &in, MoveMessage &obj);
	friend std::ostream & operator<<(std::ostream &out, MoveMessage &obj);

	MoveMessage()
	{
		m_Pid= 0;
		m_Src.x = 12;
		m_Src.y = 123;
		m_Dest.x = 33;
		m_Dest.y = 90;
		m_Info = "";
	}

	MoveMessage(int pid ,IntPos src,IntPos dest,std::string info)
	{
		m_Pid = pid;
		m_Src.x = src.x;
		m_Src.y = src.y;
		m_Dest.x = dest.x;
		m_Dest.y = dest.y;
		m_Info = info;
	}
	
	~MoveMessage()
	{

	}
	

	void DecodeBody() override
	{
		int pos = 0;
		GetInt(m_Pid, pos);
		GetInt(m_Src.x, pos);
		GetInt(m_Src.y, pos);
		GetInt(m_Dest.x, pos);
		GetInt(m_Dest.y, pos);
		m_Info = GetString(pos);
	}

	void EncodeBody() override
	{
		m_BodyLength = 0;
		PutInt(m_Pid);
		PutInt(m_Src.x);
		PutInt(m_Src.y);
		PutInt(m_Dest.x);
		PutInt(m_Dest.y);
		PutString(m_Info);
	}

private:

	void PutString(std::string s)
	{
		int len = strlen(s.c_str());
		char* body = Body();
		std::memcpy(body + m_BodyLength, s.c_str(), len);
		m_BodyLength += len;
	}

	void PutInt(int x)
	{
		char int_holder[5] = "";
		std::sprintf(int_holder, "%4d", x);
		char* body = Body();
		std::memcpy(body + m_BodyLength, int_holder, sizeof(int));
		m_BodyLength += sizeof(int);
	}

	void GetInt(int & out, int& pos)
	{
		char int_holder[5] = "";
		char* body = Body();
		std::strncat(int_holder, body + pos, sizeof(int));
		out = std::atoi(int_holder);
		pos += sizeof(int);
	}

	std::string GetString(int & pos)
	{
		char string_holder[256] = "";
		char* body = Body();
		std::strncat(string_holder, body + pos, m_BodyLength - pos);
		return std::string(string_holder);
	}

public:
	int m_Pid;
	IntPos m_Src;
	IntPos m_Dest;
	std::string m_Info;

};


