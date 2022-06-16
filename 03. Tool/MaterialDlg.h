#pragma once

#include "ResInfoDlg.h"

#include "mtrlctrl.h"
#include <Shader.h>

class CMaterialDlg : public CResInfoDlg
{
	DECLARE_DYNAMIC(CMaterialDlg)

private:
	vector<tPramInfo> m_vecParam;
	UINT m_iHeight;

	map<int, int> m_mapIDType; //key: id // data : shader param
	CFont*			  m_pFont;
	tIntCtrl		  m_arrInt[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	tFloatCtrl		  m_arrFloat[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	tVec2Ctrl		  m_arrVec2[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	tVec4Ctrl		  m_arrVec4[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	//tMatCtrl		  m_arrMat[(UINT)SHADER_PARAM::MAT_END - (UINT)SHADER_PARAM::MAT_0];
	tTexCtrl		  m_arrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];


public:
	virtual void SetResource(CReSource * _pResource);

private:
	void RenewControl();

public:
	CMaterialDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMaterialDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMaterialDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickShader();
	CEdit m_editShader;
	virtual BOOL OnInitDialog();
	void OnChangeContrl(UINT _id);
	void OnTexSelBtnClicked(UINT _id);
};
