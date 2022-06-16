#pragma once
#include <afxdialogex.h>
#include <resource.h>
class CResInfoDlg :
	public CDialogEx
{
private:
	CReSource* m_pResource;

public:
	virtual void SetResource(CReSource* _pRes) { m_pResource = _pRes; }
	CReSource* GetResource() { return m_pResource; }
	CReSource* GetRes() { return m_pResource; }

public:
	CResInfoDlg(UINT _iResID, CWnd* pParent);
	virtual ~CResInfoDlg();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
};

