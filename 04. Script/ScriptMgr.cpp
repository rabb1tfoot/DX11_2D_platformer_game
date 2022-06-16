#include "stdafx.h"
#include "ScriptMgr.h"

#include "BlackoutScript.h"
#include "BlockScript.h"
#include "BossBulletPara.h"
#include "BossBulletRand.h"
#include "BossScript.h"
#include "BugBulletScript.h"
#include "BugScript.h"
#include "BulletScript.h"
#include "CameraScipt.h"
#include "DoorScript.h"
#include "FollowScript.h"
#include "GunItemScript.h"
#include "HilightScript.h"
#include "HiveScript.h"
#include "InfoScript.h"
#include "MonsterScript.h"
#include "PanelScript.h"
#include "SnailScript.h"
#include "TitleGround.h"
#include "TraceScript.h"
#include "WaitCam.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"BlackoutScript");
	_vec.push_back(L"BlockScript");
	_vec.push_back(L"BossBulletPara");
	_vec.push_back(L"BossBulletRand");
	_vec.push_back(L"BossScript");
	_vec.push_back(L"BugBulletScript");
	_vec.push_back(L"BugScript");
	_vec.push_back(L"BulletScript");
	_vec.push_back(L"CameraScipt");
	_vec.push_back(L"DoorScript");
	_vec.push_back(L"FollowScript");
	_vec.push_back(L"GunItemScript");
	_vec.push_back(L"HilightScript");
	_vec.push_back(L"HiveScript");
	_vec.push_back(L"InfoScript");
	_vec.push_back(L"MonsterScript");
	_vec.push_back(L"PanelScript");
	_vec.push_back(L"SnailScript");
	_vec.push_back(L"TitleGround");
	_vec.push_back(L"TraceScript");
	_vec.push_back(L"WaitCam");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"BlackoutScript" == _strScriptName)
		return new CBlackoutScript;
	if (L"BlockScript" == _strScriptName)
		return new CBlockScript;
	if (L"BossBulletPara" == _strScriptName)
		return new CBossBulletPara;
	if (L"BossBulletRand" == _strScriptName)
		return new CBossBulletRand;
	if (L"BossScript" == _strScriptName)
		return new CBossScript;
	if (L"BugBulletScript" == _strScriptName)
		return new CBugBulletScript;
	if (L"BugScript" == _strScriptName)
		return new CBugScript;
	if (L"BulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CameraScipt" == _strScriptName)
		return new CCameraScipt;
	if (L"DoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"FollowScript" == _strScriptName)
		return new CFollowScript;
	if (L"GunItemScript" == _strScriptName)
		return new CGunItemScript;
	if (L"HilightScript" == _strScriptName)
		return new CHilightScript;
	if (L"HiveScript" == _strScriptName)
		return new CHiveScript;
	if (L"InfoScript" == _strScriptName)
		return new CInfoScript;
	if (L"MonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"PanelScript" == _strScriptName)
		return new CPanelScript;
	if (L"SnailScript" == _strScriptName)
		return new CSnailScript;
	if (L"TitleGround" == _strScriptName)
		return new CTitleGround;
	if (L"TraceScript" == _strScriptName)
		return new CTraceScript;
	if (L"WaitCam" == _strScriptName)
		return new CWaitCam;
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BLACKOUTSCRIPT:
		return L"BlackoutScript";
		break;

	case SCRIPT_TYPE::BLOCKSCRIPT:
		return L"BlockScript";
		break;

	case SCRIPT_TYPE::BOSSBULLETPARA:
		return L"BossBulletPara";
		break;

	case SCRIPT_TYPE::BOSSBULLETRAND:
		return L"BossBulletRand";
		break;

	case SCRIPT_TYPE::BOSSSCRIPT:
		return L"BossScript";
		break;

	case SCRIPT_TYPE::BUGBULLETSCRIPT:
		return L"BugBulletScript";
		break;

	case SCRIPT_TYPE::BUGSCRIPT:
		return L"BugScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"BulletScript";
		break;

	case SCRIPT_TYPE::CAMERASCIPT:
		return L"CameraScipt";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"DoorScript";
		break;

	case SCRIPT_TYPE::FOLLOWSCRIPT:
		return L"FollowScript";
		break;

	case SCRIPT_TYPE::GUNITEMSCRIPT:
		return L"GunItemScript";
		break;

	case SCRIPT_TYPE::HILIGHTSCRIPT:
		return L"HilightScript";
		break;

	case SCRIPT_TYPE::HIVESCRIPT:
		return L"HiveScript";
		break;

	case SCRIPT_TYPE::INFOSCRIPT:
		return L"InfoScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"MonsterScript";
		break;

	case SCRIPT_TYPE::PANELSCRIPT:
		return L"PanelScript";
		break;

	case SCRIPT_TYPE::SNAILSCRIPT:
		return L"SnailScript";
		break;

	case SCRIPT_TYPE::TITLEGROUND:
		return L"TitleGround";
		break;

	case SCRIPT_TYPE::TRACESCRIPT:
		return L"TraceScript";
		break;

	case SCRIPT_TYPE::WAITCAM:
		return L"WaitCam";
		break;

	}
	return nullptr;
}