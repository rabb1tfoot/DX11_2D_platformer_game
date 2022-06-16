// GameView.cpp: 구현 파일
//

#include "stdafx.h"
#include "GameView.h"
#include "ComponentView.h"
#include "MainFrm.h"
#include "BtnView.h"

#include <KeyMgr.h>
#include <SceneMgr.h>
#include <Scene.h>
#include <Layer.h>
#include <GameObj.h>
#include <Transform.h>
#include <MeshRender.h>
#include <Collider2D.h>
#include <Material.h>
#include <ResPtr.h>
#include <ResMgr.h>
#include <Texture.h>
#include <MeshRender.h>
#include <EventMgr.h>

// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)
CGameView::CGameView()
{
}

CGameView::~CGameView()
{
}


void CGameView::init()
{
}

void CGameView::update()
{
	//게임뷰 클릭만 잡는다.
	if (this != m_pframe->GetActiveView())
		return;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer** ppAllLayer = pCurScene->GetAllLayer();

	if (TOOL_TYPE::TOOL_MAIN == m_pframe->GetType())
	{
		if (KEYENTER(KEY_TYPE::MOUSE_LBTN))
		{
			ChangeGameObjTarget(ppAllLayer);

			//메인툴은 타깃 변경후 종료
			return;
		}
	}
	else if (TOOL_TYPE::TOOL_MAP == m_pframe->GetType())
	{
		//맵 타일 바꾸기
		ChangeTileUI();

		if (KEYENTER(KEY_TYPE::MOUSE_LBTN))
		{
			if (nullptr == m_pMapCamObj)
				return;

			Vec3 vCamPos = m_pMapCamObj->Transform()->GetLocalPos();
			ChangeTileTarget(ppAllLayer, vCamPos);

			if (m_bSetCollider)
			{
				Vec2 vMapSize = ((CBtnView*)(m_pframe->GetBtnView()))->GetMapSize();
				Vec3 vCamPos = m_pMapCamObj->Transform()->GetLocalPos();
				DrawCOllTile(ppAllLayer, vCamPos, vMapSize);
			}
		}
		//브러쉬 같이 타일을 찍고싶을 경우
		if (KEYHOLD(KEY_TYPE::MOUSE_LBTN))
		{
			if (nullptr == m_pMapCamObj)
				return;
			if (false == m_bSetCollider)
			{
				Vec2 vMapSize = ((CBtnView*)(m_pframe->GetBtnView()))->GetMapSize();
				Vec3 vCamPos = m_pMapCamObj->Transform()->GetLocalPos();
				DrawTile(ppAllLayer, vCamPos, vMapSize);
			}
		}

		//if (KEYAWAY(KEY_TYPE::MOUSE_LBTN))
		//{
		//	//큰 타일을 타깃으로 정할 경우
		//	if (TOOL_TYPE::TOOL_MAP == m_pframe->GetType())
		//	{
		//		ChangeBigTileTarget(ppAllLayer);
		//	}
		//}

		if (nullptr != m_pTargetTileObj)
		{
			////16x16보다 크면 마우스위치에 LT타일을 위치시킨다.
			//if (m_vTileSize.x > 16.f || m_vTileSize.y > 16.f)
			//{
			//	Vec3 TilePos = Vec3(m_vTileSize.x / 2.f, m_vTileSize.y / 2.f, 0.f);
			//	m_pTargetTileObj->Transform()->SetLocalPos(Vec3(m_pMousePos.x + TilePos.x - 8.f, m_pMousePos.y - TilePos.y + 8.f, 500.f));
			//	return;
			//}
			m_pTargetTileObj->Transform()->SetLocalPos(Vec3(m_pMousePos.x, m_pMousePos.y, 500.f));
		}
	}
}


void CGameView::ChangeTileUI()
{
	if (KEYENTER(KEY_TYPE::KEY_1))
	{
		m_pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Area1(clear)2.png");
		m_pTargetTileObj->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &m_pTex);
		m_pTileUIObj->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &m_pTex);
		m_iSetTexNum = 0;
	}
	if (KEYENTER(KEY_TYPE::KEY_2))
	{
		m_pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Area2(clear)2.png");
		m_pTargetTileObj->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &m_pTex);
		m_pTileUIObj->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &m_pTex);
		m_iSetTexNum = 1;
	}
}

void CGameView::ChangeGameObjTarget(CLayer ** ppAllLayer)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		//레이어가 없는경우 넘어간다.
		if (nullptr == ppAllLayer[i])
			continue;

		const vector<CGameObj*>& pObj = ppAllLayer[i]->GetParentObject();

		for (UINT j = 0; j < pObj.size(); ++j)
		{
			CTransform* pTrans = pObj[j]->Transform();
			Vec3 vObjPos = pTrans->GetLocalPos();
			Vec3 vObjSize = pTrans->GetLocalScale();
			Vec3 vObjLT = Vec3(vObjPos.x - vObjSize.x / 2
				, vObjPos.y - vObjSize.y / 2, vObjPos.z - vObjSize.z / 2);
			Vec3 vObjRB = Vec3(vObjLT.x + vObjSize.x
				, vObjLT.y + vObjSize.y, vObjLT.z + vObjSize.z);

			//오브젝트 눌렸는지 확인
			if (m_pMousePos.x > vObjLT.x && m_pMousePos.x < vObjRB.x
				&&m_pMousePos.y > vObjLT.y && m_pMousePos.y < vObjRB.y)
			{
				//타일은 타깃이 되지 않는다.
				if (0 == StrCmpW(L"Tile", ppAllLayer[i]->GetName().c_str()))
				{
					continue;
				}
				CComponentView* pCom = (CComponentView*)m_pframe->GetComView();
				pCom->SetTarget(pObj[j]);
				return;
			}
		}
	}
}

void CGameView::ChangeTileTarget(CLayer ** ppAllLayer, Vec3 vCamPos)
{
	//타일과 UI 레이어만 검사한다.
	for (UINT i = 4; i < 6; ++i)
	{
		//레이어가 없는경우 넘어간다.
		if (nullptr == ppAllLayer[i])
			continue;

		const vector<CGameObj*>& pObj = ppAllLayer[i]->GetParentObject();

		for (UINT j = 0; j < pObj.size(); ++j)
		{
			CTransform* pTrans = pObj[j]->Transform();
			Vec3 vObjPos;

			//타일맵은 카메라에 영향을 받는다.
			if (4 == i)
			{
				vObjPos = Vec3(pTrans->GetLocalPos().x - vCamPos.x,
					pTrans->GetLocalPos().y - vCamPos.y, pTrans->GetLocalPos().z);
			}
			else
				vObjPos = pTrans->GetLocalPos();

			Vec3 vObjSize = pTrans->GetLocalScale();
			Vec3 vObjLT = Vec3(vObjPos.x - vObjSize.x / 2
				, vObjPos.y - vObjSize.y / 2, vObjPos.z - vObjSize.z / 2);
			Vec3 vObjRB = Vec3(vObjLT.x + vObjSize.x
				, vObjLT.y + vObjSize.y, vObjLT.z + vObjSize.z);

			Vec2 vTileLT;

			//오브젝트 눌렸는지 확인
			if (m_pMousePos.x > vObjLT.x && m_pMousePos.x < vObjRB.x
				&&m_pMousePos.y > vObjLT.y && m_pMousePos.y < vObjRB.y)
			{

				//타일타깃프리셋을 변경한다.
				if (0 == StrCmpW(L"UI", ppAllLayer[i]->GetName().c_str()))
				{
					float fMouseTilePosX = m_pMousePos.x - vObjLT.x;
					float fMouseTilePosY = m_pMousePos.y - vObjLT.y;

					for (UINT k = 0; k < m_pTex->GetWidth(); k += TILE_SIZE)
					{
						for (UINT w = 0; w < m_pTex->GetHeight(); w += TILE_SIZE)
						{
							//어느 타일에 처음 눌렸는지 기록 확인
							if (fMouseTilePosX > k && fMouseTilePosX <= k + TILE_SIZE
								&& fMouseTilePosY > w && fMouseTilePosY <= w + TILE_SIZE)
							{
								//collider설정 타일
								if (k == 48 && w == 64)
									m_bSetCollider = true;
								else 
									m_bSetCollider = false;

								m_vTileLT.x = k;
								m_vTileLT.y = (240 - w);
								vTileLT = Vec2(m_vTileLT.x / m_pTex->GetWidth(), m_vTileLT.y / m_pTex->GetHeight());
							}
						}
					}
					m_pTargetTileObj->Transform()->SetLocalScale(Vec3(TILE_SIZE * 2, TILE_SIZE * 2, 1));

					m_pTargetTileObj->MeshRender()->GetCloneMaterial()->SetData(
						SHADER_PARAM::VEC2_0, &vTileLT);
				}
			}
		}
	}
}

void CGameView::ChangeBigTileTarget(CLayer ** ppAllLayer)
{
	return;
	
}

void CGameView::DrawTile(CLayer ** ppAllLayer, Vec3 vCamPos, Vec2 vMapSize)
{
	Vec3 vObjPos = m_pMapTileObj->Transform()->GetLocalPos();
	Vec3 vObjSize = m_pMapTileObj->Transform()->GetLocalScale();

	//타일맵은 카메라에 영향을 받는다.
	vObjPos = Vec3(vObjPos.x - vCamPos.x,
		vObjPos.y + vCamPos.y, vObjPos.z);

	Vec3 vObjLT = Vec3(vObjPos.x - vObjSize.x / 2, vObjPos.y - vObjSize.y / 2, vObjPos.z);
	Vec3 vObjRB = Vec3(vObjPos.x + vObjSize.x / 2, vObjPos.y + vObjSize.y / 2, vObjPos.z);

	//마우스 피킹
	if (m_pMousePos.x > vObjLT.x && m_pMousePos.x < vObjRB.x
		&&m_pMousePos.y > vObjLT.y && m_pMousePos.y < vObjRB.y)
	{
		Vec2 TilePos = Vec2(m_pMousePos.x - vObjLT.x, (-1 * vObjLT.y) - m_pMousePos.y);
		Vec2 vTileLT = Vec2(m_vTileLT.x / m_pTex->GetWidth(), m_vTileLT.y / m_pTex->GetHeight());

		//디폴트 타일 찍기
		int TileNum = 0;
		for (UINT i = 0; i < vMapSize.y; ++i)
		{
			for (UINT j = 0; j < vMapSize.x; ++j)
			{
				if (TilePos.x >= 32.f * j && TilePos.x < 32.f * (j + 1) &&
					TilePos.y >= 32.f * i && TilePos.y < 32.f * (i + 1))
				{
					//비율로 변경후 넣기
					m_vTileParam[TileNum] = Vec2(int(TilePos.x / m_pTex->GetWidth()), int(TilePos.x / m_pTex->GetHeight()));
					m_vTileTexParam[TileNum] = vTileLT;
					if (m_iSetTexNum == 0)
					{
						m_iTexIdx[TileNum] = 0;
					}
					else if (m_iSetTexNum == 1)
					{
						m_iTexIdx[TileNum] = 1;
					}
				}
				++TileNum;
			}
		}
		CResPtr<CTexture> ptempTex1 = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Area1(clear)2.png");
		CResPtr<CTexture> ptempTex2 = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Area2(clear)2.png");

		m_pMapTileObj->MeshRender()->GetCloneMaterial()->SetData(
			SHADER_PARAM::TEX_0, &ptempTex1);
		m_pMapTileObj->MeshRender()->GetCloneMaterial()->SetData(
			SHADER_PARAM::TEX_1, &ptempTex2);
		m_pMapTileObj->MeshRender()->GetCloneMaterial()->SetData(
			SHADER_PARAM::INT_ARRAY, m_iTexIdx);
		m_pMapTileObj->MeshRender()->GetCloneMaterial()->SetData(
			SHADER_PARAM::VEC2_ARRAY, m_vTileParam);
		m_pMapTileObj->MeshRender()->GetCloneMaterial()->SetData(
			SHADER_PARAM::VEC2_1ARRAY, m_vTileTexParam);
	}
}

void CGameView::DrawCOllTile(CLayer ** ppAllLayer, Vec3 vCamPos, Vec2 vMapSize)
{
	Vec3 vObjPos = m_pMapTileObj->Transform()->GetLocalPos();
	Vec3 vObjSize = m_pMapTileObj->Transform()->GetLocalScale();

	//타일맵은 카메라에 영향을 받는다.
	vObjPos = Vec3(vObjPos.x - vCamPos.x,
		vObjPos.y + vCamPos.y, vObjPos.z);

	Vec3 vObjLT = Vec3(vObjPos.x - vObjSize.x / 2, vObjPos.y - vObjSize.y / 2, vObjPos.z);
	Vec3 vObjRB = Vec3(vObjPos.x + vObjSize.x / 2, vObjPos.y + vObjSize.y / 2, vObjPos.z);

	Vec2 TilePos = Vec2(m_pMousePos.x - vObjLT.x, (-1 * vObjLT.y) - m_pMousePos.y);
	Vec2 vTileLT = Vec2(m_vTileLT.x / m_pTex->GetWidth(), m_vTileLT.y / m_pTex->GetHeight());

	//마우스 피킹
	if (m_pMousePos.x > vObjLT.x && m_pMousePos.x < vObjRB.x
		&&m_pMousePos.y > vObjLT.y && m_pMousePos.y < vObjRB.y)
	{
		//콜라이더 타일이면 콜라이더 오브젝트를 생성한다.
		for (UINT i = 0; i < vMapSize.y; ++i)
		{
			for (UINT j = 0; j < vMapSize.x; ++j)
			{
				if (TilePos.x >= 32.f * j && TilePos.x < 32.f * (j + 1) &&
					TilePos.y >= 32.f * i && TilePos.y < 32.f * (i + 1))
				{
					//이미 콜라이더 타일이 있는지 검사
					Vec3 vNewPos = (Vec3(32.f * (j + 1) - TILE_SIZE * 2 + vObjLT.x + 16.f + vCamPos.x,
						((32.f * i)* -1) - TILE_SIZE * 2 - vObjLT.y + 16.f + vCamPos.y, 400.f));
					for (int k = 0; k < m_vCollObj.size(); ++k)
					{
						Vec3 vOldPos = m_vCollObj[k]->Transform()->GetLocalPos();
						if (vOldPos == vNewPos)
						{
							return;
						}
					}
				}
			}
		}

		CGameObj* pCollObj = new CGameObj;
		int RenderTile = 1;
		pCollObj->SetName(L"CollTile");
		Vec2 TileLT = Vec2(COLL_TILE_X / m_pTex->GetWidth(), COLL_TILE_Y / m_pTex->GetHeight());
		Vec2 TileSize = Vec2(TILE_SIZE / m_pTex->GetWidth(), TILE_SIZE / m_pTex->GetHeight());
		CTransform* pTransform = new CTransform;
		CMeshRender* pMeshRender = new CMeshRender;
		CCollider2D* pColl =  new CCollider2D;
		pCollObj->AddComponent(pTransform);
		pCollObj->AddComponent(pMeshRender);
		pCollObj->AddComponent(pColl);
		for (UINT i = 0; i < vMapSize.y; ++i)
		{
			for (UINT j = 0; j < vMapSize.x; ++j)
			{
				if (TilePos.x >= 32.f * j && TilePos.x < 32.f * (j + 1) &&
					TilePos.y >= 32.f * i && TilePos.y < 32.f * (i + 1))
				{
					pTransform->SetLocalPos(Vec3(32.f * (j + 1) - TILE_SIZE * 2 + vObjLT.x + 16.f + vCamPos.x,
						((32.f * i)* -1) - TILE_SIZE * 2 - vObjLT.y + 16.f + vCamPos.y, 400.f));
				}
			}
		}

		pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
		pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, &m_pTex);
		pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::INT_2, &RenderTile);
		pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_0, &TileLT);
		pMeshRender->GetCloneMaterial()->SetData(SHADER_PARAM::VEC2_1, &TileSize);

		pTransform->SetLocalScale(Vec3(TILE_SIZE * 2, TILE_SIZE * 2, 1.f));

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(L"Tile", pCollObj);

		m_vCollObj.push_back(pCollObj);
		return;
	}
}


BEGIN_MESSAGE_MAP(CGameView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_COMMAND(ID_32777, &CGameView::OnSaveMap)
END_MESSAGE_MAP()


// CGameView 그리기

void CGameView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CGameView 진단

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGameView 메시지 처리기


void CGameView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pMousePos = point;
	m_pMousePos.x -= GAMEVIEW_X / 2;
	m_pMousePos.y -= (GAMEVIEW_Y / 2);
	m_pMousePos.y *= -1;
	CView::OnMouseMove(nFlags, point);
}


int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pTargetTileObj = nullptr;
	m_pTileUIObj = nullptr;
	m_pMapCamObj = nullptr;
	m_pMapTileObj = nullptr;
	m_vTileLT = {};
	m_vTileSize = {};
	m_bSetCollider = false;
	m_iSetTexNum = 0;
	m_vCollObj = {};

	for (int i = 0; i < 900; ++i)
	{
		m_vTileParam[i] = Vec2( 256.f / 272.f, 0.f / 256.f);
		m_vTileTexParam[i] = Vec2(0.f, 0.f);
		m_iTexIdx[i] = 0;
	}

	m_pframe = (CMainFrame*)AfxGetMainWnd();
	return 0;
}


void CGameView::OnSaveMap()
{
	CScene* pMainScene = CSceneMgr::GetInst()->GetToolScene();
	//콜라이더 타일 저장
	for (UINT i = 0; i < m_vCollObj.size(); ++i)
	{
		pMainScene->AddObject(L"Tile", m_vCollObj[i]->Clone());
	}
	m_pMapTileObj->SetName(L"MapTile");
	pMainScene->AddObject(L"Tile", m_pMapTileObj->Clone());
}
