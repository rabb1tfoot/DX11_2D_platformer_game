#pragma once
#include "Component.h"

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

class CCamera :
	public CComponent
{
private:
	Matrix m_matView;
	Matrix m_matProj;

	PROJ_TYPE m_eType;
	float m_fScale;
	float m_fFOV;

	float m_fNear;
	float m_fFar;

	UINT m_iLayerCheck;
	bool m_bRender;

public:
	virtual void Update();
	void Finalupdate();

public:
	void SetProjType(PROJ_TYPE _eType) { m_eType = _eType; }
	void SetScale(float _fScale) { if (_fScale == 0.f) m_fScale = 0.01f; else m_fScale = _fScale; }
	void SetNear(float _fNear) { m_fNear = _fNear; }
	void SetFar(float _fFar) { m_fFar = _fFar; }
	void SetFOV(float _fFov) { m_fFOV = _fFov; }
	void SetRender(bool _bool) { m_bRender = _bool; }

	PROJ_TYPE GetProjType() { return m_eType; }
	float GetScale() { return m_fScale; }
	float GetNear() { return m_fNear; }
	float GetFar() { return m_fFar; }
	float GetFOV() { return m_fFOV; }
	bool  GetRender() { return m_bRender; }

	const Matrix& GetViewMat() { return m_matView; }
	const Matrix& GetProjMat() { return m_matProj; }

	void CheckLayer(UINT _iLayerIdx);
	bool IsValiedLayer(UINT _iLayerIdx) { return m_iLayerCheck & (1 << _iLayerIdx); }

	virtual void SaveToScene(FILE* _pFile);
	virtual CComponent* LoadFromScene(FILE* _pFile);

	CLONE(CCamera);

public:
	CCamera();
	virtual ~CCamera();
};

