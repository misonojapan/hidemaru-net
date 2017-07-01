#include <jni.h>
#include <windows.h>
#include <string>
#include <iostream>

#include "convert_string.h"
#include "hidemaruexe_export.h"
#include "output_debugstream.h"
#include "self_dll_info.h"
#include "dllfunc_interface.h"
#include "dllfunc_interface_internal.h"

// jstring �� utf16��
std::wstring jstring_to_utf16(JNIEnv *env, jstring& jtext) {
	const char *utf8_text = env->GetStringUTFChars(jtext, 0);
	wstring utf16_text = utf8_to_utf16(utf8_text);
	env->ReleaseStringUTFChars(jtext, utf8_text);
	return utf16_text;
}

// jstring �� utf16��
jstring utf16_to_jstring(JNIEnv *env, wstring& utf16_text) {
	string utf8_return_value = utf16_to_utf8(utf16_text);
	return env->NewStringUTF(utf8_return_value.c_str());
}

JNIEXPORT jdouble JNICALL Java_hidemaru_Hm_GetVersion() {
	return CHidemaruExeExport::hm_version;
}


// EvalMacro
JNIEXPORT jint JNICALL Java_hidemaru_Hm_EvalMacro(JNIEnv *env, jobject obj, jstring value) {

	wstring utf16_value = jstring_to_utf16(env, value);

	jboolean success = CHidemaruExeExport::EvalMacro(utf16_value);
	if (success) {
		return true;
	}

	OutputDebugStream(L"�}�N���̎��s�Ɏ��s���܂����B\n");
	OutputDebugStream(L"�}�N�����e:\n");
	OutputDebugStream(utf16_value);
	return false;
}

// �G�ۃ}�N���̃V���{����ϐ��̒l�̎擾�B
JNIEXPORT jstring JNICALL Java_hidemaru_Hm_GetMacroVarObj(JNIEnv *env, jobject obj, jstring key_name) {

	TestDynamicVar.Clear();
	auto dll_invocant = CSelfDllInfo::GetInvocantString();

	wstring utf16_key_name = jstring_to_utf16(env, key_name);

	wstring cmd = L"##_tmp_dll_id_ret = dllfuncw( " + dll_invocant + L"\"SetDynamicVar\", " + utf16_key_name + L");\n"
		L"##_tmp_dll_id_ret = 0;\n";
	jint success = CHidemaruExeExport::EvalMacro(cmd);

	// ���l�Ȃ�
	if (TestDynamicVar.type == CDynamicValue::TDynamicType::TypeInteger)
	{
		wstring return_value = L"";
		if (sizeof(intptr_t) == sizeof(int)) {
			return_value = wstring(L"HmJavaVMType_Int32<<>>") + std::to_wstring(TestDynamicVar.num);
		}
		else {
			return_value = wstring(L"HmJavaVMType_Int64<<>>") + std::to_wstring(TestDynamicVar.num);
		}
		return utf16_to_jstring(env, return_value);
	}
	// ������Ȃ�
	else {
		wstring return_value = wstring(L"HmJavaVMType_String<<>>") + TestDynamicVar.wstr;
		return utf16_to_jstring(env, return_value);
	}
}

// �G�ۃ}�N���̃V���{����ϐ��֒l��ݒ�B���l�n�B
JNIEXPORT jint JNICALL Java_hidemaru_Hm_SetMacroVarNum(JNIEnv *env, jobject obj, jstring key_name, jlong value) {
	jint success = false;

	auto dll_invocant = CSelfDllInfo::GetInvocantString();

	wstring utf16_key_name = jstring_to_utf16(env, key_name);

	wchar_t start = utf16_key_name[0];

	if (start == L'#') {

		PushNumVar((intHM_t)value);
		wstring cmd = L" = dllfuncw( " + dll_invocant + L"\"PopNumVar\" );\n";
		cmd = utf16_key_name + cmd;
		success = CHidemaruExeExport::EvalMacro(cmd);
	}
	else {
		success = false;
	}

	return success;
}

// �G�ۃ}�N���̃V���{����ϐ��֒l��ݒ�B������n�B
JNIEXPORT jint JNICALL Java_hidemaru_Hm_SetMacroVarStr(JNIEnv *env, jobject obj, jstring key_name, jstring value) {
	jint success = false;

	auto dll_invocant = CSelfDllInfo::GetInvocantString();

	wstring utf16_key_name = jstring_to_utf16(env, key_name);

	wstring utf16_value = jstring_to_utf16(env, value);

	wchar_t start = utf16_key_name[0];

	if (start == L'$') {

		PushStrVar(utf16_value.data());
		wstring cmd = L" = dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n";
		cmd = utf16_key_name + cmd;
		success = CHidemaruExeExport::EvalMacro(cmd);
	}
	else {
		success = false;
	}

	return success;
}

// �G�ۃ}�N���̃V���{����ϐ��֒l��ݒ�B������n�B
JNIEXPORT void JNICALL Java_hidemaru_Hm_DebugInfo(JNIEnv *env, jobject obj, jstring value) {

	wstring utf16_value = jstring_to_utf16(env, value);
	OutputDebugStream(utf16_value);
}

// GetTotalText�B
JNIEXPORT jstring JNICALL Java_hidemaru_Hm_GetTotalText(JNIEnv *env, jobject obj) {

	return utf16_to_jstring(env, CHidemaruExeExport::GetTotalText());
}

// SetTotalText
JNIEXPORT jint JNICALL Java_hidemaru_Hm_SetTotalText(JNIEnv *env, jobject obj, jstring value) {
	jint success = false;

	auto dll_invocant = CSelfDllInfo::GetInvocantString();

	wstring utf16_value = jstring_to_utf16(env, value);

	PushStrVar(utf16_value.data());
	wstring cmd =
		L"begingroupundo;\n"
		L"selectall;\n"
		L"insert dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n"
		L"endgroupundo;\n";
	success = CHidemaruExeExport::EvalMacro(cmd);

	return success;
}

// GetSelectedText�B
JNIEXPORT jstring JNICALL Java_hidemaru_Hm_GetSelectedText(JNIEnv *env, jobject obj) {

	return utf16_to_jstring(env, CHidemaruExeExport::GetSelectedText());
}

// SetSelectedText
JNIEXPORT jint JNICALL Java_hidemaru_Hm_SetSelectedText(JNIEnv *env, jobject obj, jstring value) {
	jint success = false;

	auto dll_invocant = CSelfDllInfo::GetInvocantString();

	wstring utf16_value = jstring_to_utf16(env, value);

	PushStrVar(utf16_value.data());
	wstring cmd =
		L"if (selecting) {\n"
		L"insert dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n"
		L"};\n";
	success = CHidemaruExeExport::EvalMacro(cmd);

	return success;
}

// GetLineText�B
JNIEXPORT jstring JNICALL Java_hidemaru_Hm_GetLineText(JNIEnv *env, jobject obj) {

	return utf16_to_jstring(env, CHidemaruExeExport::GetLineText());
}

// SetLineText
JNIEXPORT jint JNICALL Java_hidemaru_Hm_SetLineText(JNIEnv *env, jobject obj, jstring value) {
	jint success = false;

	auto dll_invocant = CSelfDllInfo::GetInvocantString();

	auto pos = CHidemaruExeExport::GetCursorPos();

	wstring utf16_value = jstring_to_utf16(env, value);
	PushStrVar(utf16_value.data());

	wstring cmd =
		L"begingroupundo;\n"
		L"selectline;\n"
		L"insert dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n"
		L"moveto2 " + std::to_wstring(pos.column) + L", " + std::to_wstring(pos.lineno) + L";\n" +
		L"endgroupundo;\n";
	success = CHidemaruExeExport::EvalMacro(cmd);

	return success;
}

// GetCursorPos
JNIEXPORT jstring JNICALL Java_hidemaru_Hm_GetCursorPos(JNIEnv *env, jobject obj) {

	auto pos = CHidemaruExeExport::GetCursorPos();
	wstring str_pos = std::to_wstring(pos.lineno) + L"," + std::to_wstring(pos.column);

	return utf16_to_jstring(env, str_pos);
}

// GetCursorPosFromMousePos
JNIEXPORT jstring JNICALL Java_hidemaru_Hm_GetCursorPosFromMousePos(JNIEnv *env, jobject obj) {

	auto pos = CHidemaruExeExport::GetCursorPosFromMousePos();
	wstring str_pos = std::to_wstring(pos.lineno) + L"," + std::to_wstring(pos.column);

	return utf16_to_jstring(env, str_pos);
}
