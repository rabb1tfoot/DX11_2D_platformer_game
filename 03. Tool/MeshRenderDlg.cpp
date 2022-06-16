// MeshRenderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MeshRenderDlg.h"
#include "afxdialogex.h"

#include "ListDlg.h"

#include <GameObj.h>
#include <ResMgr.h>
#include <MeshRender.h>

// CMeshRenderDlg 대화 상자

IMPLEMENT_DYNAMIC(CMeshRenderDlg, CDialogEx)

CMeshRenderDlg::CMeshRenderDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_MESHRENDERDLG, pParent)
{

}

CMeshRenderDlg::~CMeshRenderDlg()
{
}

void CMeshRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editMesh);
	DDX_Control(pDX, IDC_EDIT2, m_editMtrl);
}

void CMeshRenderDlg::update(CGameObj * _pTarget)
{
	if (IsEditActive())
		return;

	CString strName;

	SetTarget(_pTarget);

	CMeshRender* pMeshRender = GetTarget()->MeshRender();

	CResPtr<CMesh> pMesh = pMeshRender->GetMesh();
	CResPtr<CMaterial> pMtrl = pMeshRender->GetSharedMaterial();

	if (nullptr == pMesh)
		m_editMesh.SetWindowTextW(L"");
	else
	{
		strName = CPathMgr::GetFileName(pMesh->GetName().c_str());
		m_editMesh.SetWindowTextW(strName);
	}

	if (nullptr == pMtrl)
		m_editMesh.SetWindowTextW(L"");
	else
	{
		strName = CPathMgr::GetFileName(pMtrl->GetName().c_str());
		m_editMtrl.SetWindowTextW(strName);
	}
}



BEGIN_MESSAGE_MAP(CMeshRenderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshRenderDlg::OnBtnMesh)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshRenderDlg::OnBtnMtrl)
END_MESSAGE_MAP()


// CMeshRenderDlg 메시지 처리기

void CMeshRenderDlg::OnBtnMesh()
{
	const map<wstring, CReSource*>& mapRes = CResMgr::GetInst()->GetResources(RES_TYPE::MESH);

	vector<CString> vecFileName;
	vector<CString> vecResName;

	for (const auto& pair : mapRes)
	{
		vecFileName.push_back(CPathMgr::GetFileName(pair.second->GetName().c_str()));
		vecResName.push_back(pair.second->GetName().c_str());
	}

	CListDlg dlg(L"Mesh", vecFileName, vecResName);
	int iRet = dlg.DoModal();

	if (IDOK == iRet)
	{
		CString strSelect = dlg.GetSelectSecondItem();

		CResPtr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(strSelect.GetBuffer());

		GetTarget()->MeshRender()->SetMesh(pMesh);
	}


}

void CMeshRenderDlg::OnBtnMtrl()
{
	const map<wstring, CReSource*>& mapRes = CResMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

	vector<CString> vecFileName;
	vector<CString> vecResName;

	for (const auto& pair : mapRes)
	{
		vecFileName.push_back(CPathMgr::GetFileName(pair.second->GetName().c_str()));
		vecResName.push_back(pair.second->GetName().c_str());
	}

	CListDlg dlg(L"Material", vecFileName, vecResName);
	int iRet = dlg.DoModal();

	if (IDOK == iRet)
	{
		CString strSelect = dlg.GetSelectSecondItem();
		CResPtr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strSelect.GetBuffer());
		GetTarget()->MeshRender()->SetMaterial(pMtrl);
	}
}