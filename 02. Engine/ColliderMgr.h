#pragma once

union COL_ID
{
	struct
	{
		DWORD left;
		DWORD right;
 	};
	unsigned long long ID;
};

class CCollider2D;
class CCollider3D;

class CColliderMgr
{
	SINGLE(CColliderMgr);

private:
	UINT			m_arrCheck[MAX_LAYER];
	map<unsigned long long, bool> m_mapID2D;
	map<unsigned long long, bool> m_mapID3D;

public:
	void Update();

public:
	void CollisionCheck(const wstring& _strLayer1, const wstring& _strLayer2);
	void CollisionCheck(int _iLayerIdx1, int _iLayerIdx2);
	void DelCollisionCheck(const wstring& _strLayer1, const wstring& _strLayer2);
	void DelCollisionCheck(int _iLayerIdx1, int _iLayerIdx2);

	void ClearCheck() { memset(m_arrCheck, 0, sizeof(UINT) * MAX_LAYER); }

	UINT* GetCollisionCheckArr() { return m_arrCheck; }

private:
	void CollisionGroup(int _leftIdx, int _rightIdx);

	//2차원 충돌 채크
	bool IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight);
	bool CollisionRect(CCollider2D* _pLeft, CCollider2D* _pRight);
	bool CollisionCircle(CCollider2D* _pLeft, CCollider2D* _pRight);

	//3차원 충돌 채크
	bool IsCollision(CCollider3D* _pLeft, CCollider3D* _pRight);
	bool CollisionRect(CCollider3D* _pLeft, CCollider3D* _pRight);
	bool CollisionCircle(CCollider3D* _pLeft, CCollider3D* _pRight);

};

