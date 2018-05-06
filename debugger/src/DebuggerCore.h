/*author by: oceancx*/
#pragma once
#include "defines.h"
#include "DebugServer.h"
class DebuggerCore
{
public:
	DebuggerCore();
	~DebuggerCore();
	bool breaked();
	bool hasBreakPoint(const char* source);
	static void OnHookLine(lua_State* L, lua_Debug* ar);
	void SetHook(lua_State*L);
	static void DebugHookLoop(lua_State* L, lua_Debug* ar);
	bool hitBreakPointTest(lua_Debug* ar, int currentline);
	void SetBreaked(bool isBreak) {
		m_Breaked = isBreak;
	};
	static DebuggerCore* GetInstance()
	{
		static DebuggerCore* instance = new DebuggerCore();
		return instance;
	}

	static void SetHandler(MessageHandler* handler)
	{
		m_handler = handler;
	}

	void SetAllBreakPoints(String path, Json bps)
	{
	//	std::cout << bps << std::endl;
		assert(bps.is_array());
		format_path(path);

		m_AllBreakPoints[path] = Json::array();
		Json& new_bps = m_AllBreakPoints[path];
		for (Json::iterator bp_it = bps.begin(); bp_it != bps.end(); ++bp_it)
		{
			Json new_bp = Json::object();
			new_bp["id"] = 0;
			new_bp["verified"] = false;
			new_bp["line"] = bp_it.value()["line"];
			new_bps.push_back(new_bp);
		}
	}

	const auto& GetAllBreakPoints()
	{
		return m_AllBreakPoints;
	}

	const Json& GetAllBreakPoints(String path)
	{
		format_path(path);

		assert(m_AllBreakPoints[path].is_array());
		return m_AllBreakPoints[path];
	}


	void VerifyAllBreakPoints()
	{
		int id = 1;
		for (auto& pb_pair : m_AllBreakPoints)
		{
			auto& bp_ary = pb_pair.second;
			for (Json::iterator bp_it = bp_ary.begin(); bp_it != bp_ary.end(); ++bp_it)
			{
				bp_it.value()["id"] = id++;
				bp_it.value()["verified"] = true;
			}
		}
	//	std::cout << m_AllBreakPoints << std::endl;
	}


	void SetSources(String path, Json source)
	{
		format_path(path);
		m_Sources.push_back(source);
	}
	bool has_bp(String path)
	{
		format_path(path);
		return m_AllBreakPoints.find(path) != m_AllBreakPoints.end();
	};

	bool hit_bp(String path,int line)
	{
		format_path(path); 
		if (!has_bp(path))return false;
		const Json& bp_ary= m_AllBreakPoints[path];
		for (Json::const_iterator bp_it = bp_ary.cbegin(); bp_it != bp_ary.cend(); ++bp_it)
		{
			if (bp_it.value()["line"] == line)
			{
				return true;
			}
		}
		return false;
	}
	Json get_bp(String path, int line)
	{
		format_path(path);
		const Json& bp_ary = m_AllBreakPoints[path];
		for (Json::const_iterator bp_it = bp_ary.cbegin(); bp_it != bp_ary.cend(); ++bp_it)
		{
			if (bp_it.value()["line"] == line)
			{
				return bp_it.value();
			}
		}
		return{};
	}
	inline static void format_path(String& path)
	{
		if (path[0] == '@')
			path = path.substr(1);
		std::replace(path.begin(), path.end(), '\\', '/');
	}
private:
	
	bool m_StepInto;
	int m_StepDepth;
	bool m_Breaked;
	bool m_TrapWait;
	static MessageHandler* m_handler;
	std::vector<Json> m_Sources;
	std::map<String, Json> m_AllBreakPoints;
};
