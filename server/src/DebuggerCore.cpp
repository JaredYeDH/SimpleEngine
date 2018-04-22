/*author by: oceancx*/
#include "DebuggerCore.h"
#include <functional>
#include "lobject.h"
#include "lstate.h"

#define CALLINFO_MAX_DEPTH 256

int DebuggerCore::m_StepDepth = 0;
bool DebuggerCore::m_StepInto = false;
bool DebuggerCore::m_TrapWait = false;
lua_State* DebuggerCore::L = nullptr;

DebuggerCore::DebuggerCore()
{
	lua_Debug* ar = nullptr;
	lua_sethook(L, DebuggerCore::DebugHookLoop, LUA_MASKLINE | LUA_MASKCALL | LUA_MASKRET, 4096);
	
}

DebuggerCore::~DebuggerCore()
{
	
}

bool DebuggerCore::breaked()
{
	m_StepInto = m_StepDepth = 0;
	m_TrapWait = true;
	// while handle debug update
	while (m_TrapWait)
	{

	}
	return false;
}

bool DebuggerCore::hasBreakPoint(const char* source)
{
	return false;
}

void DebuggerCore::OnHookLine(lua_State* L, lua_Debug* ar)
{
	LClosure* active_closure;
	CallInfo* active_callinfo= ar->i_ci;
	if (!ttisLclosure(active_callinfo->func))
		return;
	active_closure = clLvalue( active_callinfo->func);
	lua_getinfo(L, "nSlf", ar);
	if (!hasBreakPoint(ar->source))
	{
		return;
	}

	if (ar->what[0] == 'C')
		return;
	assert(lua_isfunction(L, -1));

	if (m_StepInto) {
		breaked();
	}
	else if (m_StepDepth
		&&m_StepDepth <= (size_t)L 
		&&m_StepDepth >=(size_t)L - CALLINFO_MAX_DEPTH)
	{
		m_StepDepth = 0;
		breaked();
	}

	if (hitBreakPointTest(ar, ar->currentline))
	{
		breaked();
	}
}

void DebuggerCore::SetHook()
{

}

void DebuggerCore::DebugHookLoop(lua_State* L,lua_Debug* ar)
{
	switch (ar->event)
	{
	case LUA_HOOKLINE:
		OnHookLine(L,ar);
		break;
	case LUA_HOOKCALL:
		if (m_StepDepth && (size_t)L + CALLINFO_MAX_DEPTH > m_StepDepth && (size_t)L - CALLINFO_MAX_DEPTH < m_StepDepth)
			m_StepDepth++;
		break;
	case LUA_HOOKRET:
		if (m_StepDepth && (size_t)L + CALLINFO_MAX_DEPTH > m_StepDepth && (size_t)L - CALLINFO_MAX_DEPTH < m_StepDepth)
			m_StepDepth--;
		break;
	default:
		break;
	}
}

bool DebuggerCore::hitBreakPointTest(lua_Debug* ar, int currentline)
{
	return false;
}
