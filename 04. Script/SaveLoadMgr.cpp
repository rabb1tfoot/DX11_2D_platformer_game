#include "stdafx.h"
#include "SaveLoadMgr.h"

#include <ResMgr.h>
#include <SceneMgr.h>
#include <Scene.h>
#include <Layer.h>
#include <GameObj.h>
#include <func.h>
#include <Component.h>
#include <Transform.h>
#include <MeshRender.h>
#include <Camera.h>
#include <Collider2D.h>
#include <Animator2D.h>

#include <Script.h>
#include "ScriptMgr.h"

CSaveLoadMgr::CSaveLoadMgr()
{
}


CSaveLoadMgr::~CSaveLoadMgr()
{
}

void CSaveLoadMgr::SaveScene(const wstring & _strPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strPath.c_str(), L"wb");

	//리소스 저장
	SaveResource(pFile);

	//씬저장
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pCurLayer = pCurScene->GetLayer(i);
		SaveLayer(pCurLayer, pFile);
	}

	fclose(pFile);
}

void CSaveLoadMgr::SaveResource(FILE * _pFile)
{
	UINT i = 0;
	for (;i < (UINT)RES_TYPE::END; ++i)
	{
		//쉐이더는 넘어간다.
		if (i == (UINT)RES_TYPE::SHADER || i == (UINT)RES_TYPE::PREFAB)
			continue;
		

		const map<wstring, CReSource*>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		//리소스 타입 저장
		fwrite(&i, sizeof(UINT), 1, _pFile);

		//리소스 갯수 저장
		UINT iResCount = mapRes.size();
		fwrite(&iResCount, sizeof(UINT), 1, _pFile);

		//리소스 정보 저장
		for (const auto& pair : mapRes)
		{
			pair.second->SaveToScene(_pFile);
		}
	}
	//리소스 END 정보 (파일종료) 
	fwrite(&i, sizeof(UINT), 1, _pFile);
}

void CSaveLoadMgr::SaveLayer(CLayer * _pLayer, FILE * _pFile)
{
	bool bExist = _pLayer;
	fwrite(&bExist, sizeof(bool), 1, _pFile);

	if (nullptr == _pLayer)
		return;

	SaveWString(_pLayer->GetName().c_str(), _pFile);

	const vector<CGameObj*>& vecObject = _pLayer->GetParentObject();
	UINT iSize = (UINT)vecObject.size();
	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], _pFile);
	}
}

void CSaveLoadMgr::SaveGameObject(CGameObj * _pObj, FILE * _pFile)
{
	SaveWString(_pObj->GetName().c_str(), _pFile);

	_pObj->SaveToScene(_pFile);

	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = _pObj->GetComponent((COMPONENT_TYPE)i);

		if (nullptr == pCom)
			continue;

		fwrite(&i, sizeof(UINT), 1, _pFile);

		pCom->SaveToScene(_pFile);
	}
	//Component end저장
	fwrite(&i, sizeof(UINT), 1, _pFile);

	//스크립트 저장
	vector<CScript*>& vecScripts = _pObj->GetScripts();

	UINT iScriptCount = (UINT)vecScripts.size();
	fwrite(&iScriptCount, sizeof(UINT), 1, _pFile);

	wstring strScriptName;
	for (UINT i = 0; i < iScriptCount; ++i)
	{
		strScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		SaveWString(strScriptName.c_str(), _pFile);

		vecScripts[i]->SaveToScene(_pFile);
	}


	vector<CGameObj*> vecChild = _pObj->GetChild();
	UINT iSize = vecChild.size();
	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		SaveGameObject(vecChild[i], _pFile);
	}
}


void CSaveLoadMgr::LoadScene(const wstring & _strPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strPath.c_str(), L"rb");

	//리소스 로드
	LoadResource(pFile);

	//씬 불러오기
	CScene* pScene = new CScene;
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = LoadLayer(pFile);
		if (nullptr != pLayer)
			pScene->AddLayer(pLayer, i);
	}

	//현재 씬으로 지정 + 툴씬으로(게임돌릴때는 지워야함)
	CSceneMgr::GetInst()->SetToolScene(pScene);
	CSceneMgr::GetInst()->ChangeCurScene(pScene);

	fclose(pFile);
}

void CSaveLoadMgr::LoadResource(FILE * _pFile)
{
	UINT iResType = (UINT)RES_TYPE::END;
	UINT iResCount = 0;

	while (true)
	{
		fread(&iResType, sizeof(UINT), 1, _pFile);

		//파일종료
		if ((UINT)RES_TYPE::END == iResType)
			break;

		fread(&iResCount, sizeof(UINT), 1, _pFile);

		CReSource* pResource = nullptr;
		bool bRet = true;

		for (UINT i = 0; i < iResCount; ++i)
		{
			switch ((RES_TYPE)iResType)
			{
			case RES_TYPE::MATERIAL:
				pResource = new CMaterial(RES_TYPE::MATERIAL);
				break;
			case RES_TYPE::PREFAB:
				//pResource = new CPrefab(RES_TYPE::PREFAB);
				break;
			case RES_TYPE::MESH:
				pResource = new CMesh(RES_TYPE::MESH);
				break;
			case RES_TYPE::TEXTURE:
				pResource = new CTexture(RES_TYPE::TEXTURE);
				break;
			case RES_TYPE::SOUND:
				 pResource = new CSound(RES_TYPE::SOUND);
				break;
			}
			bRet = pResource->LoadFromScene(_pFile);
			if (false == bRet)
			{
				delete pResource;
				continue;
			}

			CResMgr::GetInst()->AddRes((RES_TYPE)iResType, pResource);
		}
	}
}

CLayer * CSaveLoadMgr::LoadLayer(FILE * _pFile)
{
	bool bExit = true;
	fread(&bExit, sizeof(bool), 1, _pFile);

	if (false == bExit)
		return nullptr;

	CLayer* pLayer = new CLayer;

	//layer 이름
	wstring strName = LoadWString(_pFile);
	pLayer->SetName(strName);

	//gameObject 로딩
	UINT iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		pLayer->AddObject(LoadGameObject(_pFile), false, true);
	}

	return pLayer;
}

CGameObj * CSaveLoadMgr::LoadGameObject(FILE * _pFile)
{
	CGameObj* pObject = new CGameObj;
	wstring strName = LoadWString(_pFile);
	pObject->SetName(strName);

	pObject->LoadFromScene(_pFile);

	UINT iComType = 0;
	while (true)
	{
		fread(&iComType, sizeof(UINT), 1, _pFile);

		if ((UINT)COMPONENT_TYPE::END == iComType)
			break;

		CComponent* pCom = nullptr;
		switch ((COMPONENT_TYPE)iComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pCom = new CTransform;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pCom = new CMeshRender;
			break;
		case COMPONENT_TYPE::CAMERA:
			pCom = new CCamera;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pCom = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			break;
		case COMPONENT_TYPE::ANIMATION2D:
			pCom = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATION3D:
			break;


		}
		pCom->LoadFromScene(_pFile);
		pObject->AddComponent(pCom);
	}

	//스크립트 불러오기
	UINT iScriptCount = 0;
	fread(&iScriptCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iScriptCount; ++i)
	{
		strName = LoadWString(_pFile);
		CScript* pScript = CScriptMgr::GetScript(strName);
		pScript->LoadFromScene(_pFile);
		pObject->AddComponent(pScript);
	}
	UINT iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);
	for (UINT i = 0; i < iSize; ++i)
	{
		CGameObj* pChild = LoadGameObject(_pFile);
		pObject->AddChild(pChild);
	}
	return pObject;
}