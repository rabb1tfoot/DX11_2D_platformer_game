#include "stdafx.h"
#include "WaitCam.h"
#include <ResPtr.h>
#include <Sound.h>

CWaitCam::CWaitCam()
	:CScript((UINT)SCRIPT_TYPE::WAITCAM)
{
}

void CWaitCam::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	vPos.y -= 10.f * DT;

	Transform()->SetLocalPos(vPos);
}


CWaitCam::~CWaitCam()
{
}
