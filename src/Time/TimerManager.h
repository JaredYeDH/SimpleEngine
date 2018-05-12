#pragma once

#include "../defines.h"

#include <map>
#include <functional>
#include "Singleton.h"


using tstring=std::string;
class Timer;

class TimerManager final: public Singleton<TimerManager>
{
public:
	TimerManager();
	~TimerManager();

	void Update();

	bool CreateTimer(const tstring & name, float32 targetTime, bool countingDown,
		bool loop, std::function<void()> func, bool paused = false);
	bool RemoveTimer(const tstring & name);
	void PauseTimer(const tstring & name, bool paused);
	void SetCountingDownTimer(const tstring & name, bool countingDown);
	void SetLoopTimer(const tstring & name, bool looping);
	void ResetTimer(const tstring & name, bool paused = false);
	void SetTargetTimeTimer(const tstring & name, float32 targetTime, bool reset = true, bool paused = false);
	void SetFunctionTimer(const tstring & name, const std::function<void()> & func);
	float64 ForceEndTimer(const tstring & name);
	void ForwardTimer(const tstring & name, float64 time);
	void ForwardAllTimers(float64 time);

	int32 GetTimerMinutes(const tstring & name) const;
	int32 GetTimerSeconds(const tstring & name) const;
	int32 GetTimerTotalSeconds(const tstring & name) const;
	float64 GetTimerTargetTime(const tstring & name) const;
	float64 GetTimerAccurateTime(const tstring & name) const;

private:
	std::map<const tstring, Timer> m_TimerContainer;
	std::map<const tstring, Timer> m_GarbageContainer;

	TimerManager(const TimerManager& yRef);
	TimerManager(TimerManager&& yRef);
	TimerManager& operator=(const TimerManager& yRef);
	TimerManager& operator=(TimerManager&& yRef);
};

#define TIMER_MANAGER_INTANCE TimerManager::GetInstance()