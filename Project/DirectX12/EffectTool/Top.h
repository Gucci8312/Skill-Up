#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

// �����N���C�u���� //
#pragma comment(lib, "winmm.lib")				// �����N�Ώۃ��C�u������winmm.lib��ǉ�

#define	CLASS_NAME		"�E�B���h�E"			// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"EffectTool"
#define WINDOW_WIDTH	1400
#define WINDOW_HEIGHT	900

#define DEBUG