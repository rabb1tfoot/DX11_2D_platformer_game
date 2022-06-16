#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "Camera.h"
#include "Core.h"
#include "GameObj.h"
#include "Transform.h"
#include "MeshRender.h"
#include "ResMgr.h"
#include "Prefab.h"
#include "SceneMgr.h"

CScene::CScene()
	: m_iSpawnStake(0)
	, m_arrLayer{}
{
}


CScene::~CScene()
{
	Safe_Delete_Array(m_arrLayer);
}

void CScene::Awake()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			continue;

		m_arrLayer[i]->awake();
	}
}

void CScene::Start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			continue;

		m_arrLayer[i]->start();
	}
}

void CScene::Update()
{
	//모든 레이어 업데이트
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i] || wcscmp(m_arrLayer[i]->GetName().c_str(), L"Tile") == 0)
			continue;
		m_arrLayer[i]->Update();
	}
}

void CScene::Lateupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i] || wcscmp(m_arrLayer[i]->GetName().c_str(), L"Tile") == 0)
			continue;
		m_arrLayer[i]->Lateupdate();
	}


}

void CScene::Finalupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			continue;
		m_arrLayer[i]->Finalupdate();
	}
}

void CScene::Render()
{
	for (UINT i = 0; i < m_vecCam.size(); ++i)
	{
		if (false == m_vecCam[i]->GetRender())
			continue;
		g_transform.matView = m_vecCam[i]->GetViewMat();
		g_transform.matProj = m_vecCam[i]->GetProjMat();
		for (UINT j = 0; j < MAX_LAYER; ++j)
		{
			if (nullptr == m_arrLayer[j])
				continue;
			if(m_vecCam[i]->IsValiedLayer(j))
				m_arrLayer[j]->Render();
			//렌더링이 되든 안되든 오브젝트들은 삭제되어야한다.
		}
	}
	m_vecCam.clear();
}

void CScene::AddLayer(const wstring & _strLayerName, UINT _iLayerIdx)
{
	if (nullptr != m_arrLayer[_iLayerIdx])
		return;
	m_arrLayer[_iLayerIdx] = new CLayer;
	m_arrLayer[_iLayerIdx]->SetName(_strLayerName);
	m_arrLayer[_iLayerIdx]->SetLayerIdx(_iLayerIdx);
}

void CScene::AddLayer(CLayer * _pLayer, UINT _iLayerIdx)
{
	assert(!m_arrLayer[_iLayerIdx]);

	m_arrLayer[_iLayerIdx] = _pLayer;
	m_arrLayer[_iLayerIdx]->SetLayerIdx(_iLayerIdx);

	//const vector<CGameObj*> vObj = _pLayer->GetParentObject();
	
}

void CScene::AddObject(const wstring & _strLayerName, CGameObj * _pObj, bool _bMoveAll)
{
	CLayer* pFindLayer = FindLayer(_strLayerName);
	assert(pFindLayer);
	pFindLayer->AddObject(_pObj, _bMoveAll);
}

void CScene::AddObject(UINT _iLayer, CGameObj * _pObj, bool _bMoveAll)
{
	assert(m_arrLayer[_iLayer]);
	m_arrLayer[_iLayer]->AddObject(_pObj, _bMoveAll);
}

CLayer * CScene::FindLayer(const wstring & _strLayerName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			continue;

		if (m_arrLayer[i]->GetName() == _strLayerName)
		{
			return m_arrLayer[i];
		}
	}
	return nullptr;
}

CGameObj * CScene::FindGameObject(const wstring & _strName, vector<CGameObj*>& _vecOut)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			continue;

		for (UINT j = 0; j < m_arrLayer[i]->m_vecParentObj.size(); ++j)
		{
			FindObject(m_arrLayer[i]->m_vecParentObj[j], _strName, _vecOut);
		}
	}
	return nullptr;
}

void CScene::Instantiate(CResPtr<CPrefab>& _pPrefab, const Vec3 & _vPos, const wstring & _strLayerName)
{
	CGameObj* pClone = _pPrefab->Instantiate();
	pClone->Transform()->SetLocalPos(_vPos);
	CSceneMgr::GetInst()->GetCurScene()->AddObject(_strLayerName, pClone);
}

void CScene::CreateMonster(CResPtr<CPrefab>& _pPrefab, const Vec3 & _vPos, const wstring & _strLayerName, int _stack)
{
	//CGameObj* pClone = _pPrefab->Instantiate();
	//pClone->Transform()->SetLocalPos(_vPos);
	//CMonsterScript* pScript = (CMonsterScript*)pClone->GetScript<CMonsterScript>();
	//pScript->SetSpeed(float(50 + _stack));
	//int iTime = CCore::GetInst()->GetPassTime();
	//static int iKind = 0;
	//if(iKind % 4 == 0)
	//	pClone->MeshRender()->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Ghost1"));
	//else if (iKind % 4 == 1)
	//	pClone->MeshRender()->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Ghost2"));
	//else if (iKind % 4 == 2)
	//	pClone->MeshRender()->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Ghost3"));
	//else
	//	pClone->MeshRender()->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Ghost4"));
	//
	//++iKind;
	//CSceneMgr::GetInst()->GetCurScene()->AddObject(_strLayerName, pClone);
}

void CScene::FindObject(CGameObj * _pParent, const wstring & _strName, vector<CGameObj*>& _vecOut)
{
	vector<CGameObj*> stack;

	stack.push_back(_pParent);

	while (!stack.empty())
	{
		CGameObj* pObject = stack.back();
		stack.pop_back();

		if (pObject->GetName() == _strName)
		{
			_vecOut.push_back(pObject);
		}

		const vector<CGameObj*>& vecChild = pObject->GetChild();

		for (UINT i = 0; i < vecChild.size(); ++i)
		{
			stack.push_back(vecChild[i]);
		}
	}
}
