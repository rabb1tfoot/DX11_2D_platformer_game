#include "stdafx.h"
#include "ResMgr.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderMgr.h"
#include "Collider2D.h"


CResMgr::CResMgr()
{
}


CResMgr::~CResMgr()
{
	CCollider2D::ClearMaterial();

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Delete_Map(m_mapRes[i]);

		Safe_Delete_Vec(m_vecCloneRes[i]);
	}
}

void CResMgr::Init()
{
	CreateDefaultMesh();

	CreateDefaultShader();

	CreateDefaultMaterial();

	InitSound();
}

void CResMgr::CreateDefaultMesh()
{

	// ==========
	// Rect Mesh
	// ==========
	vector<VTX> vecVtx;	VTX v;
	vector<WORD> vecIdx;

	// 0 --- 1
	// |  \  |
	// 2-----3

	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.5f, 0.5f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	// Create Index Buffer
	vecIdx.push_back(0);
	vecIdx.push_back(3);
	vecIdx.push_back(2);
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(3);

	CMesh* pMesh = new CMesh(RES_TYPE::MESH);
	pMesh->CreateMesh(sizeof(VTX), vecVtx.size(), D3D11_USAGE_DEFAULT, &vecVtx[0]
		, sizeof(WORD), vecIdx.size(), &vecIdx[0]);

	pMesh->SetName(L"RectMesh");
	m_mapRes[(UINT)RES_TYPE::MESH].insert(make_pair(L"RectMesh", pMesh));

	vecVtx.clear();
	vecIdx.clear();

	// ===================
	// Collider Rect Mesh
	// ===================
	// 0 --- 1
	// |	 |
	// 2-----3

	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.5f, 0.5f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	// Create Index Buffer
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(3);
	vecIdx.push_back(2);
	vecIdx.push_back(0);

	pMesh = new CMesh(RES_TYPE::MESH);
	pMesh->CreateMesh(sizeof(VTX), vecVtx.size(), D3D11_USAGE_DEFAULT, &vecVtx[0]
		, sizeof(WORD), vecIdx.size(), &vecIdx[0], D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pMesh->SetName(L"ColliderRectMesh");
	m_mapRes[(UINT)RES_TYPE::MESH].insert(make_pair(L"ColliderRectMesh", pMesh));

	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Circle Mesh
	// ===========
	UINT iSliceCount = 40;
	float fThetaStride = XM_2PI / (float)iSliceCount;
	float fTheta = 0.f;

	// 원의 중심
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	// 테두리 정점
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos.x = cosf(fTheta);
		v.vPos.y = sinf(fTheta);
		v.vPos.z = 0.f;

		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		vecVtx.push_back(v);

		fTheta += fThetaStride;
	}

	// 인덱스
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(RES_TYPE::MESH);
	pMesh->CreateMesh(sizeof(VTX), vecVtx.size(), D3D11_USAGE_DEFAULT, &vecVtx[0]
		, sizeof(WORD), vecIdx.size(), &vecIdx[0]);

	pMesh->SetName(L"CircleMesh");
	m_mapRes[(UINT)RES_TYPE::MESH].insert(make_pair(L"CircleMesh", pMesh));

	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Circle Collision Mesh
	// ===========
	iSliceCount = 40;
	fThetaStride = XM_2PI / (float)iSliceCount;
	fTheta = 0.f;

	// 원의 중심
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	// 테두리 정점
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos.x = cosf(fTheta);
		v.vPos.y = sinf(fTheta);
		v.vPos.z = 0.f;

		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		vecVtx.push_back(v);

		fTheta += fThetaStride;
	}

	// 인덱스
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(RES_TYPE::MESH);
	pMesh->CreateMesh(sizeof(VTX), vecVtx.size(), D3D11_USAGE_DEFAULT, &vecVtx[0]
		, sizeof(WORD), vecIdx.size(), &vecIdx[0], D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pMesh->SetName(L"ColliderCircleMesh");
	m_mapRes[(UINT)RES_TYPE::MESH].insert(make_pair(L"ColliderCircleMesh", pMesh));

	vecVtx.clear();
	vecIdx.clear();

}

void CResMgr::CreateDefaultShader()
{
	wstring strKey;

	// ==============
	// VtxColorShader
	// ==============
	CShader* pShader = nullptr;

	pShader = new CShader(RES_TYPE::SHADER);
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS");

	strKey = L"VtxColorShader";
	pShader->SetName(strKey);
	m_mapRes[(UINT)RES_TYPE::SHADER].insert(make_pair(strKey, pShader));

	// ===================
	// Collider2D Shader
	// ===================

	pShader = new CShader(RES_TYPE::SHADER);
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Collider2D");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Collider2D");

	strKey = L"Collider2DShader";
	pShader->SetName(strKey);
	m_mapRes[(UINT)RES_TYPE::SHADER].insert(make_pair(strKey, pShader));


	// ==============
	// Texture Shader
	// ==============

	pShader = new CShader(RES_TYPE::SHADER);
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Tex");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Tex");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddParam(SHADER_PARAM::TEX_0, L"Output Image");

	strKey = L"TextureShader";
	pShader->SetName(strKey);
	m_mapRes[(UINT)RES_TYPE::SHADER].insert(make_pair(strKey, pShader));

	// ==============
	// Std2D Shader
	// ==============

	pShader = new CShader(RES_TYPE::SHADER);
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Std2D");

	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddParam(SHADER_PARAM::TEX_0, L"Output Image");

	strKey = L"Std2DShader";
	pShader->SetName(strKey);
	m_mapRes[(UINT)RES_TYPE::SHADER].insert(make_pair(strKey, pShader));

}

void CResMgr::CreateDefaultMaterial()
{
	CResPtr<CMaterial> pMtrl = nullptr;

	pMtrl = new CMaterial(RES_TYPE::MATERIAL);
	pMtrl->SetName(L"VtxColorMaterial");
	pMtrl->SetShader(FindRes<CShader>(L"VtxColorShader"));
	AddRes<CMaterial>(pMtrl->GetName(), pMtrl);

	pMtrl = new CMaterial(RES_TYPE::MATERIAL);
	pMtrl->SetName(L"Collider2DMaterial");
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	AddRes<CMaterial>(pMtrl->GetName(), pMtrl);

	pMtrl = new CMaterial(RES_TYPE::MATERIAL);
	pMtrl->SetName(L"TextureMaterial");
	pMtrl->SetShader(FindRes<CShader>(L"TextureShader"));
	AddRes<CMaterial>(pMtrl->GetName(), pMtrl);

	pMtrl = new CMaterial(RES_TYPE::MATERIAL);
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindRes<CShader>(L"Std2DShader"));
	AddRes<CMaterial>(pMtrl->GetName(), pMtrl);

	CCollider2D::CreateMaterial();
}

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::AddRes(RES_TYPE _eType, CReSource * _pResource)
{
	map<wstring, CReSource*>::iterator iter = m_mapRes[(UINT)_eType].find(_pResource->GetName());

	if (iter != m_mapRes[(UINT)_eType].end())
		return;

	m_mapRes[(UINT)_eType].insert(make_pair(_pResource->GetName(), _pResource));
}
