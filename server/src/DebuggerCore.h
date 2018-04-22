/*author by: oceancx*/
#pragma once
#include "defines.h"

class DebuggerCore
{
public:
	DebuggerCore();
	~DebuggerCore();
	static bool breaked();
	static bool hasBreakPoint(const char* source);
	static void OnHookLine(lua_State* L, lua_Debug* ar);
	void SetHook();
	static void DebugHookLoop(lua_State* L, lua_Debug* ar);
	static bool hitBreakPointTest(lua_Debug* ar, int currentline);
private:
	static bool m_StepInto;
	static int m_StepDepth;
	static bool m_TrapWait;
	static lua_State* L;
};
