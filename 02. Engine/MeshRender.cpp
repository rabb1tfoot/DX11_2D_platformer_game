#include "stdafx.h"
#include "MeshRender.h"
#include "GameObj.h"
#include "Transform.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Animation2D.h"

#include "Collider2D.h"
#include "Animator2D.h"
#include "ResMgr.h"


CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_pMesh(nullptr)
	, m_pMeterial(nullptr)
{
}


CMeshRender::~CMeshRender()
{
}


CResPtr<CMaterial> CMeshRender::GetCloneMaterial()
{
	if (nullptr == m_pMeterial)
		return nullptr;

	m_pMeterial = m_pMeterial->Clone();

	CResMgr::GetInst()->AddCloneRes(m_pMeterial);

	return m_pMeterial;
}

void CMeshRender::Update()
{
}

void CMeshRender::Render()
{
	Transform()->UpdateData();

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}
	else
	{
		CAnimation2D::ClearRegister();
	}

	if (nullptr == m_pMeterial->GetShader())
		return;

	m_pMeterial->UpdateData();
	m_pMesh->SetLayout(m_pMeterial->GetShader());
	m_pMesh->Render();

	// Collider 가 있으면 그려준다.
	//if (nullptr != Collider2D())
	//	Collider2D()->render();
}

void CMeshRender::SaveToScene(FILE * _pFile)
{
	bool bMesh = true; bool bMtrl = true;

	if (nullptr == m_pMesh)
		bMesh = false;
	if (nullptr == m_pMeterial)
		bMesh = false;

	fwrite(&bMesh, sizeof(bool), 1, _pFile);

	if (bMesh)
	{
		SaveWString(m_pMesh->GetName().c_str(), _pFile);
		SaveWString(m_pMesh->GetPath().c_str(), _pFile);
	}
	fwrite(&bMtrl, sizeof(bool), 1, _pFile);

	if (bMtrl)
	{
		SaveWString(m_pMeterial->GetName().c_str(), _pFile);
		SaveWString(m_pMeterial->GetPath().c_str(), _pFile);
	}

	const tShaderParam tParam =  m_pMeterial->GetData();
	CResPtr<CTexture>* pArrTex =  m_pMeterial->GetTexData();
	const tTileParam&  tTileParams =  m_pMeterial->GetTileData();

	//해당 쉐이더 파라미터 저장
	for (UINT i = 0; i < 4; ++i)
	{
		fwrite(&tParam.arrFloat[i], sizeof(float), 1, _pFile);
		fwrite(&tParam.arrInt[i], sizeof(int), 1, _pFile);
		fwrite(&tParam.arrVec2[i], sizeof(Vec2), 1, _pFile);
		fwrite(&tParam.arrVec4[i], sizeof(Vec4), 1, _pFile);
	}
	//택스처 파라미터 저장
	for (int i = 0; i < 4; ++i)
	{
		if (nullptr != pArrTex[i])
		{
			bool HaveTexture = true;
			fwrite(&HaveTexture, sizeof(bool), 1, _pFile);

			SaveWString(pArrTex[i]->GetName().c_str(), _pFile);
			SaveWString(pArrTex[i]->GetPath().c_str(), _pFile);
		}
		else
		{
			bool None = false;
			fwrite(&None, sizeof(bool), 1, _pFile);
			break;
		}
	}
	// 타일일경우 타일 파라미터 저장
	if (Object()->GetLayerIdx() == 4)
	{
		bool bTileHave = true;
		int a = 4;
		fwrite(&bTileHave, sizeof(bool), 1, _pFile);
		fwrite(&tTileParams, sizeof(tTileParam), 1, _pFile);
	}
	else
	{
		bool None = false;
		fwrite(&None, sizeof(bool), 1, _pFile);
	}
}

CComponent* CMeshRender::LoadFromScene(FILE * _pFile)
{
	bool bMesh = true; bool bMtrl = true;
	wstring strKey, strPath;

	fread(&bMesh, sizeof(bool), 1, _pFile);

	if (bMesh)
	{
		strKey = LoadWString(_pFile);
		strPath = LoadWString(_pFile);

		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(strKey);
		if (nullptr == m_pMesh)
		{
			m_pMesh = CResMgr::GetInst()->Load<CMesh>(strKey, strPath);
		}
	}

	fread(&bMtrl, sizeof(bool), 1, _pFile);

	if (bMtrl)
	{
		strKey = LoadWString(_pFile);
		strPath = LoadWString(_pFile);

		m_pMeterial = CResMgr::GetInst()->FindRes<CMaterial>(strKey);
		if (nullptr == m_pMeterial)
		{
			m_pMeterial = CResMgr::GetInst()->Load<CMaterial>(strKey, strPath);
		}
	}
	//해당 쉐이더 파라미터 불러오기

	tShaderParam tLoadParam = {};
	CResPtr<CTexture> ArrTex[4] = {};
	tTileParam tLoadTileParam = {};

	for (UINT i = 0; i < 4; ++i)
	{
		fread(&tLoadParam.arrFloat[i], sizeof(float), 1, _pFile);
		fread(&tLoadParam.arrInt[i], sizeof(int), 1, _pFile);
		fread(&tLoadParam.arrVec2[i], sizeof(Vec2), 1, _pFile);
		fread(&tLoadParam.arrVec4[i], sizeof(Vec4), 1, _pFile);
	}
	//택스처 파라미터 불러오기
	for (int i = 0; i < 4; ++i)
	{
		bool IsExist;
		fread(&IsExist, sizeof(bool), 1, _pFile);
		if (IsExist)
		{
			wstring strName = LoadWString(_pFile);
			wstring strPath = LoadWString(_pFile);
			ArrTex[i] = CResMgr::GetInst()->FindRes<CTexture>(strName.c_str());
		}
		else
			break;
	}

	// 타일일경우 타일 파라미터 불러오기
	bool bTileIsExit = false;
	int a = 0;
	fread(&bTileIsExit, sizeof(bool), 1, _pFile);
	if (bTileIsExit)
	{
		fread(&tLoadTileParam, sizeof(tTileParam), 1, _pFile);
	}

	//파라미터 데이터 세팅하기
	GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_0, &tLoadParam.arrFloat[0]);
	GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_1, &tLoadParam.arrFloat[1]);
	GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_2, &tLoadParam.arrFloat[2]);
	GetCloneMaterial()->SetData(SHADER_PARAM::FLOAT_3, &tLoadParam.arrFloat[3]);
	GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &tLoadParam.arrInt[0]);
	GetCloneMaterial()->SetData(SHADER_PARAM::INT_1, &tLoadParam.arrInt[1]);
	GetCloneMaterial()->SetData(SHADER_PARAM::INT_2, &tLoadParam.arrInt[2]);
	GetCloneMaterial()->SetData(SHADER_PARAM::INT_3, &tLoadParam.arrInt[3]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_0, &tLoadParam.arrVec2[0]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_1, &tLoadParam.arrVec2[1]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_2, &tLoadParam.arrVec2[2]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_3, &tLoadParam.arrVec2[3]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC4_0, &tLoadParam.arrVec4[0]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC4_1, &tLoadParam.arrVec4[1]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC4_2, &tLoadParam.arrVec4[2]);
	GetCloneMaterial()->SetData(SHADER_PARAM::VEC4_3, &tLoadParam.arrVec4[3]);
	for (int i = 0; i < 4; ++i)
	{
		if (nullptr != ArrTex[i])
			GetCloneMaterial()->SetData((SHADER_PARAM)((UINT)SHADER_PARAM::TEX_0 + i), &ArrTex[i]);
	}
	if (bTileIsExit)
	{
		Vec2 TileParam[900] = {};
		Vec2 TileTexParam[900] = {};
		for (int i = 0; i < 900; ++i)
		{
			TileParam[i] = Vec2(tLoadTileParam.arrVec2TileX[i], tLoadTileParam.arrVec2TileY[i]);
			TileTexParam[i] = Vec2(tLoadTileParam.arrVec2TileTexX[i], tLoadTileParam.arrVec2TileTexY[i]);
		}
		GetCloneMaterial()->SetData(SHADER_PARAM::INT_ARRAY, &tLoadTileParam.arrIntTile);
		GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_ARRAY, &TileParam);
		GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_1ARRAY, &TileTexParam);
	}
	return nullptr;
}
