#pragma once

#include "defines.h"
#include "Pos.h"
#include "../scene/GameMap.h"
#include "tsv.h"
#include "Logger.h"
#include "../animation/animation_database.h"
#include "../animation/FrameAnimation.h"
#include "../State.h"

class Player  : public BaseGameEntity
{
public:
	Player(int roleID);
	Player();
	~Player();
	void OnUpdate(double dt);
	void ReloadFrames();
	void ResetDirAll(int dir);
	void ResetDir(int dir);
	void SetDir(int dir);
	void ReverseDir();
	double GetCombatDistSquare();
	double GetCombatAngle();
	double GetMoveDestDistSquare(Pos dest);
	double GetMoveDestAngle(Pos dest);
	void SetActionID(int state);
	int GetActionID(){ return m_ActionID; }

	void OnDraw(int px,int py);
	void SetPos(double x, double y);
    void SetPos(Pos p){ m_Pos = p;};
	Pos GetPos(){ return m_Pos;};

	void SetCombatPos(double x, double y) {m_CombatPos.x=x;m_CombatPos.y=y;};
	void SetCombatPos(Pos pos){m_CombatPos = pos;};
	Pos GetCombatPos(){ return m_CombatPos;};

    Pos GetCombatTargetPos(){ return m_CombatTargetPos;};

	void SetBox();
	void SetBoxX(int x){ m_Box.x = x;};
	void SetBoxY(int y){m_Box.y = y;}
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
	
	std::map<int,FrameAnimation>& GetPlayerFrames(){ return m_PlayerFrames;};

	FrameAnimation* GetCurrentPlayerFrame();
	FrameAnimation* GetCurrentWeaponFrame();
	
	bool IsMove() { return m_IsMove; }
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

	void SetSkillFrame(FrameAnimation* anim);
	FrameAnimation& GetSkillFrame(){return m_SkillFrame;}
	void SetTargetID(int id){m_TargetID = id;}
	int GetTargetID(){return m_TargetID;}
	
	StateMachine<Player>* GetFSM(){ return m_pFSM;};
 	
	bool HandleMessage(const Telegram& msg) override;
	void SetGameMap(GameMap* map){ m_GameMapPtr = map;}
	GameMap* GetGameMap(){return m_GameMapPtr;}
	std::list<Pos>& GetMoveList() { return m_MoveList;} ;
    std::list<Pos>& GetBackupMoveList(){ return m_BackupMoveList; };
private:
	GameMap* m_GameMapPtr;
	StateMachine<Player>* m_pFSM;
	std::map<int,FrameAnimation> m_PlayerFrames;
	std::map<int,FrameAnimation> m_WeaponFrames;
	FrameAnimation m_SkillFrame;
	bool m_bSkillFrameShow;
	std::wstring m_NickName;
	int m_AnimationState;
	int m_Dir;
	Pos m_Pos;
	Pos m_CombatPos;
	Pos m_CombatTargetPos;
    BoxPos m_Box;
	std::list<Pos> m_MoveList;
    std::list<Pos> m_BackupMoveList;
	bool m_IsMove;
	double m_MoveVelocity;
	bool m_bInCombat;

	int m_RoleID;				//current role
	int m_ActionID;				//current action
	int m_WeaponID;				//current weapon
	bool m_HasWeapon;
	int m_TargetID;

	utils::AnimDB m_AnimDB;

	void LogInfo();
	
};


