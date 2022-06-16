#include "stdafx.h"
#include "Animation2D.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "TimeMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "Texture.h"

tAnim2D g_anim2d;

CAnimation2D::CAnimation2D()
	:m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}


CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::Update()
{
}

void CAnimation2D::Lateupdate()
{
}

void CAnimation2D::Finalupdate()
{
	if (!m_bPlay)
		return;

	m_fAccTime += DT;

	if (m_vecFrm[m_iCurFrm].vTerm < m_fAccTime)
	{
		float fDiff = m_fAccTime - m_vecFrm[m_iCurFrm].vTerm;

		int iNextFrm = m_iCurFrm + 1;

		if (iNextFrm >= m_vecFrm.size())
		{
			iNextFrm = m_vecFrm.size() - 1; // 마지막 프레임 유지
			m_fAccTime = 0.f;
			m_bFinish = true;
			m_bPlay = false;
		}
		else
		{
			while (true)
			{
				fDiff -= m_vecFrm[iNextFrm].vTerm;
				if (fDiff < 0.f)
				{
					m_fAccTime = m_vecFrm[iNextFrm].vTerm + fDiff;
					break;
				}

				++iNextFrm;
				if (m_vecFrm.size() <= iNextFrm) // Animation 종료
				{
					iNextFrm = m_vecFrm.size() - 1; // 마지막 프레임 유지
					m_fAccTime = 0.f;
					m_bFinish = true;
					m_bPlay = false;
					break;
				}
			}
		}

		m_iCurFrm = iNextFrm;
	}
}

void CAnimation2D::UpdateData()
{
	static CConstBuffer* pCB = CDevice::GetInst()->FindConstBuffer(L"Anim2D");

	g_anim2d.iAnimCheck = 1;
	g_anim2d.vLT = m_vecFrm[m_iCurFrm].vLT;
	g_anim2d.vSize = m_vecFrm[m_iCurFrm].vSize;
	
	pCB->AddData(&g_anim2d, sizeof(tAnim2D));
	pCB->UpdateData();
	pCB->SetRegister((UINT)SHADER_TYPE::PIXEL_SHADER);

	m_vecFrm[m_iCurFrm].pTex->SetRegister(17, (UINT)SHADER_TYPE::PIXEL_SHADER);
}

void CAnimation2D::Create(const wstring & _strName, CResPtr<CTexture> _pTex
	, Vec2 _vLT, Vec2 _vChopSize
	, int _iFrmCount, float _fTerm)
{
	SetName(_strName);

	float fWidth = _pTex->GetWidth();
	float fHeight = _pTex->GetHeight();

	tAnim2DFrm tFrm = {};

	Vec2 vCropUV = Vec2(_vChopSize.x / fWidth, _vChopSize.y / fHeight);
	Vec2 vLTUV = Vec2(_vLT.x / fWidth, _vLT.y / fHeight);

	for (int i = 0; i < _iFrmCount; ++i)
	{
		tFrm.pTex = _pTex;
		tFrm.vLT = vLTUV;
		tFrm.vLT.x += (i * vCropUV.x);

		tFrm.vSize = vCropUV;
		tFrm.vTerm = _fTerm;

		m_vecFrm.push_back(tFrm);
	}
}

void CAnimation2D::Create(const wstring & _strName, const wstring & _strFolderPath, float _fTerm)
{
	SetName(_strName);

	wstring strFolderPath = CPathMgr::GetResPath();
	strFolderPath += _strFolderPath;
	strFolderPath += L"\\*.*";

	WIN32_FIND_DATA tData = {};
	HANDLE h = (HANDLE)FindFirstFile(strFolderPath.c_str(), &tData);
	UINT err = GetLastError();
	CResPtr<CTexture> pTex = nullptr;

	tAnim2DFrm tFrm = {};

	while (true)
	{
		const wchar_t* pExt = CPathMgr::GetExt(tData.cFileName);

		if (!wcscmp(pExt, L".png")
			|| !wcscmp(pExt, L".bmp")
			|| !wcscmp(pExt, L".jpg")
			|| !wcscmp(pExt, L".jpeg"))
		{
			wstring strKey = _strFolderPath + tData.cFileName;

			pTex = CResMgr::GetInst()->Load<CTexture>(strKey, strKey);
			tFrm.pTex = pTex;
			tFrm.vTerm = _fTerm;
			tFrm.vLT = Vec2(0.f, 0.f);
			tFrm.vSize = Vec2(1.f, 1.f);

			m_vecFrm.push_back(tFrm);
		}

		if (false == FindNextFile(h, &tData))
		{
			err = GetLastError();
			break;
		}
	}
}

void CAnimation2D::Play()
{
	if (m_bFinish)
	{
		m_iCurFrm = 0;
		m_bFinish = false;
	}

	m_bPlay = true;
}

void CAnimation2D::SaveToScene(FILE * _pFile)
{
	SaveWString(GetName().c_str(), _pFile);

	UINT iFrmCount = (UINT)m_vecFrm.size();
	fwrite(&iFrmCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < m_vecFrm.size(); ++i)
	{
		SaveWString(m_vecFrm[i].pTex->GetName().c_str(), _pFile);

		fwrite(&m_vecFrm[i].vLT, sizeof(Vec2), 1, _pFile);
		fwrite(&m_vecFrm[i].vSize, sizeof(Vec2), 1, _pFile);
		fwrite(&m_vecFrm[i].vTerm, sizeof(float), 1, _pFile);
	}


}

void CAnimation2D::LoadFromScene(FILE * _pFile)
{
	SetName(LoadWString(_pFile));

	UINT iFrmCount = 0;
	fread(&iFrmCount, sizeof(UINT), 1, _pFile);

	wstring strKey;
	tAnim2DFrm frm = {};
	for (UINT i = 0; i < iFrmCount; ++i)
	{
		strKey = LoadWString(_pFile);
		frm.pTex = CResMgr::GetInst()->FindRes<CTexture>(strKey);

		fread(&frm.vLT, sizeof(Vec2), 1, _pFile);
		fread(&frm.vSize, sizeof(Vec2), 1, _pFile);
		fread(&frm.vTerm, sizeof(float), 1, _pFile);

		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::ClearRegister()
{
	static CConstBuffer* pCB = CDevice::GetInst()->FindConstBuffer(L"Anim2D");

	g_anim2d.iAnimCheck = 0;
	g_anim2d.vLT = Vec2();
	g_anim2d.vSize = Vec2();

	pCB->AddData(&g_anim2d, sizeof(tAnim2D));
	pCB->UpdateData();
	pCB->SetRegister((UINT)SHADER_TYPE::PIXEL_SHADER);

	CTexture::ClearRegister(17, (UINT)SHADER_TYPE::PIXEL_SHADER);
}
