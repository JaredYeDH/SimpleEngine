#pragma once

#include "defines.h"
#include "FrameAnimation.h"
#include "Pos.h"
#include "Config.h"
#include "GameMap.h"
#include "tsv.h"
#include "Logger.h"
#include "animation_database.h"
#include "../State.h"
#include "../Message.h"



class PlayerCombatIdleState;
class PlayerCombatMoveState;
class PlayerCombatBackState;
class PlayerCombatAttackState;
class PlayerCombatCastAttackState;
class PlayerCombatBeCastAttackedState;
class PlayerCombatBeAttackedState;
class PlayerCombatGlobalState;



class Player  : public BaseGameEntity
{
private:
	friend class PlayerCombatIdleState;
	friend class PlayerCombatMoveState;
	friend class PlayerCombatBackState;
	friend class PlayerCombatAttackState;
	friend class PlayerCombatBeAttackedState;
	friend class PlayerCombatCastAttackState;
	friend class PlayerCombatGlobalState;
	friend class PlayerCombatBeCastAttackedState;

	StateMachine<Player>* m_pFSM;
public:
	StateMachine<Player>* GetFSM(){ return m_pFSM;};
 	
	bool HandleMessage(const Telegram& msg) override;

public:
// collapse
// angry
// cry
// idle_combat
// sayhi
// attack
// conjure
// salute
// walk
// attacked
// runto
// dance
// runback
// defend
// idle
// wave
// sit
// bow
	enum ActionName : int 
	{
		Idle = 15,		
		Moving = 9,		
		Attack1 = 2,
		// Attack2 = 3,
		Caster1 = 3,
		// Caster2 = 5
	};
	
	Player(int roleID);
	Player();
	~Player();
	void OnUpdate(double dt);
	void ReloadFrames();
	void ResetDirAll(int dir);
	void ResetDir(int dir);
	void SetDir(int dir);
	void ReverseDir();
	
	void SetActionID(int state) { 
		m_ActionID = state; 
		if(m_PlayerFrames.find(m_ActionID)!= m_PlayerFrames.end() )
		{
			m_PlayerFrames[m_ActionID].ResetAnim(m_Dir);
			if(m_WeaponFrames.find(m_ActionID)!= m_WeaponFrames.end() )
			{
				m_WeaponFrames[m_ActionID].ResetAnim(m_Dir);
			}
		}
	};
	int GetActionID(){ return m_ActionID; }

	void OnDraw(int px,int py);
	void SetPos(double x, double y);
    void SetPos(Pos p){ m_Pos = p;};
	Pos GetPos(){ return m_Pos;};
	void SetCombatPos(double x, double y) {m_CombatPos.x=x;m_CombatPos.y=y;};
	void SetCombatPos(Pos pos){m_CombatPos = pos;};
	Pos GetCombatPos(){ return m_CombatPos;};

	void SetBox();
	double GetX() { return m_Pos.x; }
	double GetY() { return m_Pos.y; }

	int GetBoxX() { return m_Pos.x/20; }
	int GetBoxY() { return m_Pos.y/20; }

	void SetX(double x) { m_Pos.x = x; }
	void SetY(double y) { m_Pos.y = y; }

	void TranslateX(double x) { m_Pos.x += x; }
	void TranslateY(double y) { m_Pos.y += y; }
	
	void MoveTo(GameMap* gameMapPtr, int param2, int param3);
	void SetVelocity(int velocity) { m_MoveVelocity = velocity; };
	double GetVelocity() { return  m_MoveVelocity ;};
	
	

	FrameAnimation& GetCurrentPlayerFrame()
	{
		assert(m_PlayerFrames.find(m_ActionID) != m_PlayerFrames.end());
		return m_PlayerFrames[m_ActionID];
	};
	FrameAnimation& GetCurrentWeaponFrame()
	{
		assert(m_WeaponFrames.find(m_ActionID) != m_WeaponFrames.end());
		return m_WeaponFrames[m_ActionID];
	};
	

	bool IsMove() { return m_IsMove; }
	void HandleMoveToCalled();
	void SaveFrame(int index);
	void ChangeRole();
	void ChangeRole(int roleID);
	void ChangeWeapon();
	void ChangeWeapon(int WeaponID);
	void ChangeAction();
	void ChangeAction(int actionID);
	void SetCombatTargetPos(Pos pos) { m_CombatTargetPos = pos;};
	
	void SetNickName(std::wstring name) { m_NickName= name ;};
	void SetIsCombat(bool bcombat){ m_bInCombat = bcombat;}

	void SetSkillFrame(FrameAnimation* anim)
	{
        if(anim&&anim->GetSpritesCount() <= 0 ) return;
		m_bSkillFrameShow = true;
		m_SkillFrame =  *anim;
		m_SkillFrame.ResetAnim(0);
	}
	FrameAnimation& GetSkillFrame()
	{
		return m_SkillFrame;
	}
	void SetTargetID(int id)
	{
		m_TargetID = id;
	}
	int GetTargetID()
	{
		return m_TargetID;
	}
	
private:

	std::map<int,FrameAnimation> m_PlayerFrames;
	std::map<int,FrameAnimation> m_WeaponFrames;
	FrameAnimation m_SkillFrame;
	bool m_bSkillFrameShow;
	// std::map<int,FrameAnimation> m_SkillFrames;
	
	std::wstring m_NickName;
	int m_AnimationState;
	int m_Dir;
	Pos m_Pos;
	Pos m_CombatPos;
	Pos m_CombatTargetPos;
    BoxPos m_Box;
	std::list<Pos> m_MoveList;
    std::list<Pos> m_BackupMoveList;
    bool m_MoveToCalled;
	double m_UpdateDelta;
	bool m_IsMove;
	double m_MoveVelocity;
	bool m_bInCombat;

	int m_RoleID;				//current role
	int m_ActionID;				//current action
	int m_WeaponID;				//current weapon
	bool m_HasWeapon;
	int m_TargetID;

	utils::tsv m_ActionTSV;
	utils::tsv m_RoleTSV;
	utils::tsv m_WeaponTSV;
	utils::AnimDB m_AnimDB;
	
	

	void LogInfo()
	{
		Logger::Print("Role:%s Weapon:%s Action:%s\n",
		m_RoleTSV.query(m_RoleID)[1].c_str(),
		m_WeaponTSV.query(m_WeaponID)[1].c_str(),
		m_ActionTSV.query(m_ActionID)[1].c_str()
		);
	}
};


class BasePlayerCombatState :  public State<Player> 
{
public:
	virtual void Enter(Player* ){};
    virtual void Execute(Player* player){};
 	virtual void Exit(Player* ) {};
	virtual bool OnMessage(Player* , const Telegram& ) {};
};

class PlayerCombatIdleState : public BasePlayerCombatState, public Singleton<PlayerCombatIdleState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;
};


class PlayerCombatMoveState : public BasePlayerCombatState, public Singleton<PlayerCombatMoveState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;
	bool OnMessage(Player* , const Telegram& ) override;
private:
	bool m_bSent;
};
class PlayerCombatAttackState : public BasePlayerCombatState, public Singleton<PlayerCombatAttackState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;
};

class PlayerCombatCastAttackState : public BasePlayerCombatState, public Singleton<PlayerCombatCastAttackState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;
};


class PlayerCombatBackState : public BasePlayerCombatState, public Singleton<PlayerCombatBackState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;
};


class PlayerCombatBeAttackedState : public BasePlayerCombatState, public Singleton<PlayerCombatBeAttackedState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;

};

class PlayerCombatBeCastAttackedState : public BasePlayerCombatState, public Singleton<PlayerCombatBeCastAttackedState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;

};

class PlayerCombatGlobalState : public BasePlayerCombatState, public Singleton<PlayerCombatGlobalState>
{
public:	
    void Execute(Player* player) override;
	void Enter(Player* player) override;
	bool OnMessage(Player* , const Telegram& ) override;
};

