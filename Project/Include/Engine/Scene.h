#pragma once
#include "Entity.h"

class CLayer;
class CGameObj;
class CCamera;

class CPrefab;
#include "ResPtr.h"

class CScene :
	public CEntity
{
private:
	CLayer *		 m_arrLayer[MAX_LAYER];
	vector<CCamera*> m_vecCam;
	int m_iSpawnStake;
	CResPtr<CPrefab>	m_MonsterPrefab;

public:
	void Awake();
	void Start();
	
	void Update();
	void Lateupdate();
	void Finalupdate();

	void Render();

public:
	void AddLayer(const wstring& _strLayerName, UINT _iLayerIdx);
	void AddLayer(CLayer* _pLayer, UINT _iLayerIdx);

	void AddObject(const wstring & _strLayerName, CGameObj * _pObj, bool _bMoveAll = true);
	void AddObject(UINT _iLayer, CGameObj * _pObj, bool _bMoveAll = true);

	void AddCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	CLayer* FindLayer(const wstring& _strLayerName);
	CLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
	CLayer** GetAllLayer() {return m_arrLayer;}
	CGameObj* FindGameObject(const wstring& _strName, vector<CGameObj*>& _vecOut);

	void Instantiate(CResPtr<CPrefab>& _pPrefab, const Vec3& _vPos, const wstring& _strLayerName);

private:
	void CreateMonster(CResPtr<CPrefab>& _pPrefab, const Vec3& _vPos, const wstring& _strLayerName, int _stack);
	void FindObject(CGameObj* _pParent, const wstring& _strName, vector<CGameObj*>& _vecOut);

public:
	CLONE_DISABLE(CScene);

public:
	CScene();
	virtual ~CScene();

	friend class CPanelScript;
};

