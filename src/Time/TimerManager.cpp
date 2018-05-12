#include "TimerManager.h"
#include "Timer.h"
#include <algorithm>

#include "../Logger.h"


TimerManager::TimerManager()
{

}

TimerManager::~TimerManager()
{

}

void TimerManager::Update()
{
    //clean up the trash
    for (auto & it : m_GarbageContainer)
    {
        m_TimerContainer.erase(it.first);
    }
    m_GarbageContainer.clear();

    for (auto it = m_TimerContainer.begin(); it != m_TimerContainer.end();)
    {
        if (it->second.Update())
        {
            m_TimerContainer.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

bool TimerManager::CreateTimer(const std::string & name, float32 targetTime,
                               bool countingDown, bool loop,
                               std::function<void()> func, bool paused)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            return false;
        }
    }

    for (auto it = m_TimerContainer.begin(); it != m_TimerContainer.end(); ++it)
    {
        if (it->first == name)
        {
            m_GarbageContainer[it->first] = it->second;
            break;
        }
    }

    Timer newTimer(targetTime, countingDown, loop, func, paused);
    m_TimerContainer[name] = newTimer;
    return true;
}

bool TimerManager::RemoveTimer(const std::string & name)
{
    for (auto it = m_TimerContainer.begin(); it != m_TimerContainer.end(); ++it)
    {
        if (it->first == name)
        {
            m_GarbageContainer[it->first] = it->second;
            return true;
        }
    }

    LOG_WARN(_T("TimerManager::RemoveTimer: trying to remove unknown timer '")
             + name + _T("'."));
    return false;
}

void TimerManager::PauseTimer(const std::string & name, bool paused)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            it.second.SetPaused(paused);
            return;
        }
    }
    LOG_WARN(
             _T("TimerManager::PauseTimer: trying to pause unknown timer '")
             + name + _T("'."));
}

void TimerManager::SetCountingDownTimer(const std::string & name, bool countingDown)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            it.second.SetCountingDown(countingDown);
            return;
        }
    }
    LOG_WARN(
             _T("TimerManager::SetCountingDownTimer: trying to adjust unknown timer '")
             + name + _T("'."));
}

void TimerManager::SetLoopTimer(const std::string & name, bool looping)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            it.second.SetLoop(looping);
            return;
        }
    }
    LOG_WARN(
             _T("TimerManager::SetLoopTimer: trying to adjust unknown timer '")
             + name + _T("'."));
}

void TimerManager::ResetTimer(const std::string & name, bool paused)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            it.second.Reset(paused);
            return;
        }
    }
    LOG_WARN(
             _T("TimerManager::ResetTimer: trying to reset unknown timer '")
             + name + _T("'."));
}

void TimerManager::SetTargetTimeTimer(const std::string & name, float32 targetTime, bool reset, bool paused)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            it.second.SetTargetTime(targetTime, reset, paused);
            return;
        }
    }
    LOG_WARN(
             _T("TimerManager::SetTargetTimeTimer: trying to adjust unknown timer '")
             + name + _T("'."));
}

void TimerManager::SetFunctionTimer(const std::string & name, const std::function<void()> & func)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            it.second.SetFunction(func);
            return;
        }
    }
    LOG_WARN(
             _T("TimerManager::SetFunctionTimer: trying to adjust unknown timer '")
             + name + _T("'."));
}

float64 TimerManager::ForceEndTimer(const std::string & name)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            return it.second.ForceEnd();
        }
    }
    LOG_WARN(
             _T("TimerManager::ForceEndTimer: trying to end unknown timer '")
             + name + _T("'."));
    return 0;
}

void TimerManager::ForwardTimer(const std::string & name, float64 time)
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            it.second.Forward(time);
            return;
        }
    }
    LOG_WARN(
             _T("TimerManager::ForwardTimer: trying to forward unknown timer '")
             + name + _T("'."));
}

void TimerManager::ForwardAllTimers(float64 time)
{
    for (auto & it : m_TimerContainer)
    {
        it.second.Forward(time);
    }
}

int32 TimerManager::GetTimerMinutes(const std::string & name) const
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            return it.second.GetCurrentMinutes();
        }
    }
    LOG_WARN(
             _T("TimerManager::GetTimerMinutes: Couldn't find the timer '")
             + name + _T("'."));
    return 0;
}

int32 TimerManager::GetTimerSeconds(const std::string & name) const
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            return it.second.GetCurrentSeconds();
        }
    }
    LOG_WARN(
             _T("TimerManager::GetTimerSeconds: Couldn't find the timer '")
             + name + _T("'."));
    return 0;
}

int32 TimerManager::GetTimerTotalSeconds(const std::string & name) const
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            return it.second.GetCurrentTotalSeconds();
        }
    }
    LOG_WARN(
             _T("TimerManager::GetTimerTotalSeconds: Couldn't find the timer '")
             + name + _T("'."));
    return 0;
}

float64 TimerManager::GetTimerTargetTime(const std::string & name) const
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            return it.second.GetTargetTime();
        }
    }
    LOG_WARN(
             _T("TimerManager::GetTimerTargetTime: trying to access unknown timer '")
             + name + _T("'."));
    return 0;
}

float64 TimerManager::GetTimerAccurateTime(const std::string & name) const
{
    for (auto & it : m_TimerContainer)
    {
        if (it.first == name)
        {
            return it.second.GetCurrentAccurateTime();
        }
    }
    LOG_WARN(
             _T("TimerManager::GetTimerAccurateTime: Couldn't find the timer '")
             + name + _T("'."));
    return 0;
}
