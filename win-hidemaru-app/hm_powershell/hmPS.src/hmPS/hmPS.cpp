#include <windows.h>
#include <string>

#include "string_converter.h"
#include "hidemaruexe_export.h"
#include "hmPS.h"
#include "hmPSStatlcLib.h"


using namespace std;
using namespace System;


// ��̎蓮��BindDllHandle�������ōs���B�G��8.66�ȏ�
// �P�񂾂����s����Ηǂ��킯�ł͂Ȃ��Bdll���ǂݍ��܂�Ă���Ԃɂ�dll�l���ς���Ă��܂���������Ȃ����߁B(�����̎����ł�)
// ����āADoString��DoFile�̎����_�@�ɍX�V����B
static bool BindDllHandle() {
	// �G��8.66�ȏ�
	if (Hidemaru_GetDllFuncCalledType) {
		int dll = Hidemaru_GetDllFuncCalledType(-1); // ������dll�̌Ă΂�����`�F�b�N
		IPSStaticLib::BindDllHandle((IntPtr)dll);
		return true;
	}
	return false;
}


MACRO_DLL intHM_t SetCodePage(intHM_t cp) {
	IPSStaticLib::SetCodePage((IntPtr)cp);
	return TRUE;
}

// �G�ۂ̕ϐ��������񂩐��l���̔���p
MACRO_DLL intHM_t SetTmpVar(const void* dynamic_value) {
	int param_type = Hidemaru_GetDllFuncCalledType(1); // 1�Ԗڂ̈����̌^�B
	if (param_type == DLLFUNCPARAM_WCHAR_PTR) {
		return (intHM_t)IPSStaticLib::SetTmpVar(gcnew String((wchar_t *)dynamic_value));
	}
	else {
		return (intHM_t)IPSStaticLib::SetTmpVar((intHM_t)dynamic_value);
	}
}

MACRO_DLL intHM_t PopNumVar() {
	intHM_t num = (intHM_t)IPSStaticLib::PopTmpVar();
	return num;
}

static wstring strvarsopop;
MACRO_DLL const TCHAR * PopStrVar() {
	auto var = (String ^)IPSStaticLib::PopTmpVar();
	strvarsopop = String_to_tstring(var);
	return strvarsopop.data();
}

//------------------------------------------------------------------------------------
MACRO_DLL intHM_t GetNumVar(const TCHAR *sz_var_name) {
	BindDllHandle();

	return (intHM_t)IPSStaticLib::GetNumVar(gcnew String(sz_var_name));
}

MACRO_DLL intHM_t SetNumVar(const TCHAR *sz_var_name, intHM_t value) {
	BindDllHandle();

	return (intHM_t)IPSStaticLib::SetNumVar(gcnew String(sz_var_name), (IntPtr)value);
}

// �G�ۂ̃L���b�V���̂���
static wstring strvars;
MACRO_DLL const TCHAR * GetStrVar(const TCHAR *sz_var_name) {
	BindDllHandle();

	auto var = IPSStaticLib::GetStrVar(gcnew String(sz_var_name));
	strvars = String_to_tstring(var->ToString());
	return strvars.data();
}

MACRO_DLL intHM_t SetStrVar(const TCHAR *sz_var_name, const TCHAR *value) {
	BindDllHandle();

	return (intHM_t)IPSStaticLib::SetStrVar(gcnew String(sz_var_name), gcnew String(value));
}
//------------------------------------------------------------------------------------



MACRO_DLL intHM_t DoString(const TCHAR *szexpression) {
	BindDllHandle();

	// �����͂悭�ԈႦ��̂ł��������`�F�b�N�B���͏G��8.66�ȍ~�ł͂قƂ�Ǘ��p���Ȃ��̂Ŗ���
	if (Hidemaru_GetDllFuncCalledType) {
		auto rtn_type = Hidemaru_GetDllFuncCalledType(0); // 0�͕Ԃ�l�̌^
		if (rtn_type == DLLFUNCRETURN_CHAR_PTR || rtn_type == DLLFUNCRETURN_WCHAR_PTR) {
			MessageBox(NULL, L"�Ԃ�l�̌^���قȂ�܂��B\ndllfuncstr�ł͂Ȃ��Adllfuncw���𗘗p���Ă��������B", L"�Ԃ�l�̌^���قȂ�܂�", MB_ICONERROR);
		}

		auto arg_type = Hidemaru_GetDllFuncCalledType(1); // 1��1�Ԗڂ̈���
		if (arg_type != DLLFUNCPARAM_WCHAR_PTR) {
			MessageBox(NULL, L"�����̌^���قȂ�܂��B\ndllfunc�ł͂Ȃ��Adllfuncw���𗘗p���Ă��������B", L"�����̌^���قȂ�܂�", MB_ICONERROR);
		}
	}

	return (intHM_t)IPSStaticLib::DoString(gcnew String(szexpression));
}

MACRO_DLL intHM_t DoFile(const TCHAR *szfilename) {
	BindDllHandle();

	// �����͂悭�ԈႦ��̂ł��������`�F�b�N�B���͏G��8.66�ȍ~�ł͂قƂ�Ǘ��p���Ȃ��̂Ŗ���
	if (Hidemaru_GetDllFuncCalledType) {
		auto rtn_type = Hidemaru_GetDllFuncCalledType(0); // 0�͕Ԃ�l�̌^
		if (rtn_type == DLLFUNCRETURN_CHAR_PTR || rtn_type == DLLFUNCRETURN_WCHAR_PTR) {
			MessageBox(NULL, L"�Ԃ�l�̌^���قȂ�܂��B\ndllfuncstr�ł͂Ȃ��Adllfuncw���𗘗p���Ă��������B", L"�Ԃ�l�̌^���قȂ�܂�", MB_ICONERROR);
		}

		auto arg_type = Hidemaru_GetDllFuncCalledType(1); // 1��1�Ԗڂ̈���
		if (arg_type != DLLFUNCPARAM_WCHAR_PTR) {
			MessageBox(NULL, L"�����̌^���قȂ�܂��B\ndllfunc�ł͂Ȃ��Adllfuncw���𗘗p���Ă��������B", L"�����̌^���قȂ�܂�", MB_ICONERROR);
		}
	}

	return (intHM_t)IPSStaticLib::DoFile(gcnew String(szfilename));
}

MACRO_DLL intHM_t DestroyScope() {
	return (intHM_t)IPSStaticLib::DestroyScope();
}

MACRO_DLL intHM_t DllDetachFunc_After_Hm866() {
	return DestroyScope();
}