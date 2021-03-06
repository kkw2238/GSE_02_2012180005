#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Renderer.h"
#include "Collision.h"
#include "Screen.h"
#include "Time.h"
#include <map>

const int MAX_OBJECTS_COUNT = 5000;

const float LEVEL_GOD			= 0.0f;
const float LEVEL_SKY			= 0.1f;
const float LEVEL_GROUND		= 0.2f;
const float LEVEL_UNDERGROUND	= 0.4f;

const float LEVEL_BUILDING = 0.1f;
const float LEVEL_CHARACTER = 0.2f;
const float LEVEL_BULLET = 0.3f;
const float LEVEL_ARROW = LEVEL_BULLET;

enum ObjectType { OBJECT_BUILDING, OBJECT_CHARACTER, OBJECT_BULLET, OBJECT_ARROW};
enum TeamType { TEAM_1, TEAM_2 };

class Object
{
private:
	using RenderingLEVEL = float;
private:
	Vector3										m_vPos;
	Vector3										m_vDirection;
	Vector4										m_vColor;

	bool										m_bLive;

	float										m_fSize;
	float										m_fValocity;
	float										m_fLife;
	float										m_fMaxLife;
	float										m_fActionTime;
	float										m_fActionDelay;

	int											m_itexID;
	int											m_iMaxSpriteCount;
	int											m_iCurSpirteCount;
	int											m_iSpriteXCount;
	int											m_iSpriteYCount;

	Renderer*									m_pRenderer;
	Collision									m_colAABB;
	ObjectType									m_eObjectType;
	TeamType									m_eTeamType;
	RenderingLEVEL								m_fRenderingLevel;
	char										m_pHPbuf[50];
	
	std::list<std::shared_ptr<Object>>			m_lpChildObject;
	
public:
	Object();

	Object(const Vector3& pos, float size, const Vector4& color, Renderer* rend, const  Vector3& vDirection, float fValocity, ObjectType type, TeamType team, float Life, int texID, RenderingLEVEL flevel);
	
	~Object();

public:
	virtual void Render();
	virtual void Update(float fElpsedtime);

public:
	void Move(float Elpsedtime);

	void SetValocity(const float fValocity) { m_fValocity = fValocity; }
	void SetDirection(Vector3& vDirection) { m_vDirection = vDirection.Normalize(); }
	void SetColor(Vector4& vColor) { m_vColor = vColor; }
	void SetLive(bool isDead) { m_bLive = isDead; }

	void AddChildObject(std::shared_ptr<Object>& child) {
		m_lpChildObject.push_back(child);
		m_fActionTime = 0.0f;
	}

	void Damage(const float fDamage);

	bool isLive() const { return m_bLive; }
	bool CoolOn() const { return m_fActionTime > m_fActionDelay; }

	float GetLife() const { return m_fLife; }

	Vector3 GetDirection() const { return m_vDirection; }
	Vector3 GetPosition() const { return m_vPos; }
	Collision GetCollision() const { return m_colAABB; }
	ObjectType GetType() const { return m_eObjectType; }
	TeamType GetTeam() const { return m_eTeamType; }

	bool CollisionObject(std::shared_ptr<Object>& other);

};

// 오브젝트 포인터를 넣어주고 비어 있는 공간에 새로운 오브젝트를 삽입.
class SceneManager
{
public:
	using VISP = std::vector<std::list<std::shared_ptr<Object>>>;

private:
	int m_iCurrentObjectCount;
	int* m_iTexID;
	int m_iBGMIndex;
	int m_iBoomIndex;

	float m_fTeam1CharacterCooldown;
	float m_fTeam2CharacterCooldown;
	float m_fSnowTime;

	Renderer* m_pRenderer;
	Screen m_sScreen;
	Time m_tTime;
	Sound* m_pSound;

	std::list<std::shared_ptr<Object>>	m_lBuildingObjects;
	std::list<std::shared_ptr<Object>>	m_lBulletObjects;
	std::list<std::shared_ptr<Object>>	m_lCharacterObjects;
	std::list<std::shared_ptr<Object>>	m_lArraowObjects;
public:
	bool IsFull() const { return m_iCurrentObjectCount >= MAX_OBJECTS_COUNT; }

public:
	SceneManager();
	~SceneManager() { Destroy(); }

	void Run();
	void Update(float fElpsedtime);
	void Render();
	void CheckObjectCollision();
	void InitSceneManager();
	
	int AddCharacter(Vector3& vec3Pos, TeamType type);
	std::shared_ptr<Object>* CreateNewObject(Vector3& pos, ObjectType type, TeamType team);

	void Destroy();

};

