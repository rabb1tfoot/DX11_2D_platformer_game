// MaterialDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MaterialDlg.h"
#include "ResInfoDlg.h"
#include "afxdialogex.h"

#include "ListDlg.h"
#include <ResMgr.h>
#include <Shader.h>

#include "resource.h"


// CMaterialDlg 대화 상자

IMPLEMENT_DYNAMIC(CMaterialDlg, CDialogEx)

CMaterialDlg::CMaterialDlg(CWnd* pParent /*=nullptr*/)
	: CResInfoDlg(IDD_CMaterialDlg, pParent)
	, m_pFont(nullptr)
	, m_arrInt{}
	, m_arrFloat{}
	, m_arrVec2{}
	, m_arrVec4{}
	, m_arrTex{}
{
}

CMaterialDlg::~CMaterialDlg()
{
	SAFE_DELETE(m_pFont);
}

void CMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editShader);
}


void CMaterialDlg::SetResource(CReSource * _pResource)
{
	CResInfoDlg::SetResource(_pResource);

	CResPtr<CShader> pShader = ((CMaterial*)_pResource)->GetShader();
	if (nullptr == pShader)
		m_editShader.SetWindowTextW(L"");
	else
	{
		m_editShader.SetWindowTextW(pShader->GetName().c_str());

		RenewControl();
	}
}

void CMaterialDlg::RenewControl()
{
	m_vecParam.clear();
	CResPtr<CShader> pShader = ((CMaterial*)GetResource())->GetShader();

	m_vecParam = pShader->GetParamInfo();

	m_iHeight = 60;

	for (UINT i = 0; i < (UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0; ++i)
		m_arrInt[i].ShowCtrl(false);
	for (UINT i = 0; i < (UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0; ++i)
		m_arrFloat[i].ShowCtrl(false);
	for (UINT i = 0; i < (UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0; ++i)
		m_arrVec2[i].ShowCtrl(false);
	for (UINT i = 0; i < (UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0; ++i)
		m_arrVec4[i].ShowCtrl(false);
	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
		m_arrTex[i].ShowCtrl(false);

	tShaderParam shaderparam = ((CMaterial*)GetResource())->GetData();
	CResPtr<CTexture>* m_arrTexParam = ((CMaterial*)GetResource())->GetTexData();

	for (UINT i = 0; i < m_vecParam.size(); ++i)
	{
		if (false == m_vecParam[i].m_bAccess)
			continue;

		int iIdx = 0;
		switch (m_vecParam[i].eType)
		{
		case SHADER_PARAM::INT_0:
		case SHADER_PARAM::INT_1:
		case SHADER_PARAM::INT_2:
		case SHADER_PARAM::INT_3:
			iIdx = (UINT)m_vecParam[i].eType - (UINT)SHADER_PARAM::INT_0;
			m_arrInt[iIdx].SetHeight(m_iHeight);
			m_arrInt[iIdx].SetText(m_vecParam[i].szName);
			m_arrInt[iIdx].ShowCtrl(true);
			m_iHeight += 50;
			break;

		case SHADER_PARAM::FLOAT_0:
		case SHADER_PARAM::FLOAT_1:
		case SHADER_PARAM::FLOAT_2:
		case SHADER_PARAM::FLOAT_3:
			iIdx = (UINT)m_vecParam[i].eType - (UINT)SHADER_PARAM::FLOAT_0;
			m_arrFloat[iIdx].SetHeight(m_iHeight);
			m_arrFloat[iIdx].SetText(m_vecParam[i].szName);
			m_arrFloat[iIdx].ShowCtrl(true);
			m_iHeight += 50;
			break;
		case SHADER_PARAM::VEC2_0:
		case SHADER_PARAM::VEC2_1:
		case SHADER_PARAM::VEC2_2:
		case SHADER_PARAM::VEC2_3:
			iIdx = (UINT)m_vecParam[i].eType - (UINT)SHADER_PARAM::VEC2_0;
			m_arrVec2[iIdx].SetHeight(m_iHeight);
			m_arrVec2[iIdx].SetText(m_vecParam[i].szName);
			m_arrVec2[iIdx].ShowCtrl(true);
			m_iHeight += 50;
			break;
		case SHADER_PARAM::VEC4_0:
		case SHADER_PARAM::VEC4_1:
		case SHADER_PARAM::VEC4_2:
		case SHADER_PARAM::VEC4_3:
			iIdx = (UINT)m_vecParam[i].eType - (UINT)SHADER_PARAM::VEC4_0;
			m_arrVec4[iIdx].SetHeight(m_iHeight);
			m_arrVec4[iIdx].SetText(m_vecParam[i].szName);
			m_arrVec4[iIdx].ShowCtrl(true);
			m_iHeight += 50;
			break;
		case SHADER_PARAM::MAT_0:
		case SHADER_PARAM::MAT_1:
		case SHADER_PARAM::MAT_2:
		case SHADER_PARAM::MAT_3:
			break;
		case SHADER_PARAM::TEX_0:
		case SHADER_PARAM::TEX_1:
		case SHADER_PARAM::TEX_2:
		case SHADER_PARAM::TEX_3:
			iIdx = (UINT)m_vecParam[i].eType - (UINT)SHADER_PARAM::TEX_0;
			m_arrTex[iIdx].SetHeight(m_iHeight);
			m_arrTex[iIdx].SetText(m_vecParam[i].szName);
			if (nullptr != m_arrTexParam[iIdx])
				m_arrTex[iIdx].SetData(m_arrTexParam[iIdx]->GetName().c_str());
			m_arrTex[iIdx].ShowCtrl(true);
			m_iHeight += 50;

			break;
		}
	}
}

const int g_iCtrlMax = 2 * ((UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0) +
					2 * ((UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0) +
					3 * ((UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0) +
					5 * ((UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0) +
					3 * ((UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0);

BEGIN_MESSAGE_MAP(CMaterialDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMaterialDlg::OnBnClickShader)
	ON_CONTROL_RANGE(EN_CHANGE, 0, g_iCtrlMax -1, OnChangeContrl)
	ON_CONTROL_RANGE(BN_CLICKED, 0, g_iCtrlMax - 1, OnTexSelBtnClicked)
END_MESSAGE_MAP()


// CMaterialDlg 메시지 처리기


void CMaterialDlg::OnBnClickShader()
{
	const map<wstring, CReSource*>& mapRes = CResMgr::GetInst()->GetResources(RES_TYPE::SHADER);

	vector<CString> vecFileName;
	vector<CString> vecResName;

	for (const auto& pair : mapRes)
	{
		vecFileName.push_back(CPathMgr::GetFileName(pair.second->GetName().c_str()));
		vecResName.push_back(pair.second->GetName().c_str());
	}

	CListDlg dlg(L"Shader", vecFileName, vecResName);
	int iRet = dlg.DoModal();

	if (IDOK == iRet)
	{
		CString strSelect = dlg.GetSelectSecondItem();
		CResPtr<CShader> pShader = CResMgr::GetInst()->FindRes<CShader>(strSelect.GetBuffer());

		CResPtr<CMaterial> pMtrl = (CMaterial*)GetRes();
		pMtrl->SetShader(pShader);

		if (nullptr == pShader)
			m_editShader.SetWindowTextW(L"");
		else
		{
			m_editShader.SetWindowTextW(pShader->GetName().c_str());
			RenewControl(); // 변경된 쉐이더에 맞추어서 파라미터 목록 화면 갱신
		}
	}
}


BOOL CMaterialDlg::OnInitDialog()
{
	CResInfoDlg::OnInitDialog();

	m_pFont = new CFont;
	m_pFont->CreateFont(17, 0, 0, 0, 50, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"arial");

	for (UINT i = 0; i < (UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0; ++i)
	{
		m_arrInt[i].Create(m_pFont, L"", 0, this, (SHADER_PARAM)(i + (UINT)SHADER_PARAM::INT_0));
		m_arrInt[i].ShowCtrl(false);
	}

	for (UINT i = 0; i < (UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0; ++i)
	{
		m_arrFloat[i].Create(m_pFont, L"", 0, this, (SHADER_PARAM)(i + (UINT)SHADER_PARAM::FLOAT_0));
		m_arrFloat[i].ShowCtrl(false);
	}

	for (UINT i = 0; i < (UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0; ++i)
	{
		m_arrVec2[i].Create(m_pFont, L"", 0, this, (SHADER_PARAM)(i + (UINT)SHADER_PARAM::VEC2_0));
		m_arrVec2[i].ShowCtrl(false);
	}

	for (UINT i = 0; i < (UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0; ++i)
	{
		m_arrVec4[i].Create(m_pFont, L"", 0, this, (SHADER_PARAM)(i + (UINT)SHADER_PARAM::VEC4_0));
		m_arrVec4[i].ShowCtrl(false);
	}
	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
	{
		m_arrTex[i].Create(m_pFont, L"", 0, this, (SHADER_PARAM)(i + (UINT)SHADER_PARAM::TEX_0));
		m_arrTex[i].ShowCtrl(false);
	}

	return TRUE;
}

void CMaterialDlg::OnChangeContrl(UINT _id)
{
	SHADER_PARAM eType = tCtrl::GetParamType(_id);

	switch (eType)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
	{
		int iData = m_arrInt[(UINT)eType - (UINT)SHADER_PARAM::INT_0].GetData();
		((CMaterial*)GetResource())->SetData(eType, &iData);
	}
	break;

	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
	{
		float fData = m_arrFloat[(UINT)eType - (UINT)SHADER_PARAM::FLOAT_0].GetData();
		((CMaterial*)GetResource())->SetData(eType, &fData);
	}
	break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
	{
		Vec2 vData = m_arrVec2[(UINT)eType - (UINT)SHADER_PARAM::VEC2_0].GetData();
		((CMaterial*)GetResource())->SetData(eType, &vData);
	}
	break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
	{
		Vec4 vData = m_arrVec4[(UINT)eType - (UINT)SHADER_PARAM::VEC4_0].GetData();
		((CMaterial*)GetResource())->SetData(eType, &vData);
	}
	break;
	case SHADER_PARAM::MAT_0:
	case SHADER_PARAM::MAT_1:
	case SHADER_PARAM::MAT_2:
	case SHADER_PARAM::MAT_3:
		break;
	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:

		break;
	}
}

void CMaterialDlg::OnTexSelBtnClicked(UINT _id)
{
	const map<wstring, CReSource*>& mapRes = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

	vector<CString> vecFileName;
	vector<CString> vecResName;

	for (const auto& pair : mapRes)
	{
		vecFileName.push_back(CPathMgr::GetFileName(pair.second->GetName().c_str()));
		vecResName.push_back(pair.second->GetName().c_str());
	}

	CListDlg dlg(L"Texture", vecFileName, vecResName);
	int iRet = dlg.DoModal();

	if (IDOK == iRet)
	{
		CString strSelect = dlg.GetSelectSecondItem();
		CResPtr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(strSelect.GetBuffer());

		SHADER_PARAM eType = tCtrl::GetParamType(_id);
		((CMaterial*)GetResource())->SetData(eType, &pTexture);
		RenewControl();
	}
}
