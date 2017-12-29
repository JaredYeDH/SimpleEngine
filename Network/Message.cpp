#include "Message.h"

std::ostream & operator<<(std::ostream &out, MoveMessage &obj)
{
	//MoveInfo &obj = *this;
	out << "pid:" << obj.m_Pid << std::endl
		<< "src x:" << obj.m_Src.x << std::endl
		<< "src y:" << obj.m_Src.y << std::endl
		<< "dest x:" << obj.m_Dest.x << std::endl
		<< "dest y:" << obj.m_Dest.y << std::endl
		<< "info:" << obj.m_Info;
	return out;
}

std::istream & operator >> (std::istream &in, MoveMessage& obj)
{
	//MoveInfo &obj = *this;
	in >> obj.m_Pid
		>> obj.m_Src.x >> obj.m_Src.y
		>> obj.m_Dest.x >> obj.m_Dest.y
		>> obj.m_Info;
	if (!in)
	{
		obj = MoveMessage();
	}
	return in;
}
