#include "main.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// �E�C���h�E�v���V�[�W��

int g_nFPS;
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpszArgs, int _nWinMode)
{
	HWND hWnd;
	LPCTSTR szClassName = TEXT(WINDOW_NAME);
	MSG msg;
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,						// �\������E�C���h�E�̃X�^�C����ݒ�
		WndProc,						// �E�B���h�E�v���V�[�W���̃A�h���X(�֐���)���w��
		0,								// �ʏ�͎g�p���Ȃ��̂�"0"���w��
		0,								// �ʏ�͎g�p���Ȃ��̂�"0"���w��
		_hInstance,						// WinMain�̃p�����[�^�̃C���X�^���X�n���h����ݒ�
		NULL,							// �g�p����A�C�R�����w��(Windows�������Ă���A�C�R�����g���Ȃ�NULL)
		LoadCursor(NULL, IDC_ARROW),	// �}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),		// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,							// Window�ɂ��郁�j���[��ݒ�
		CLASS_NAME,						// �E�C���h�E�N���X�̖��O
		NULL							// �������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	};

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,						// �g���E�B���h�E�X�^�C��
		CLASS_NAME,				// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,			// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,			// �E�B���h�E�̍���w���W
		CW_USEDEFAULT,			// �E�B���h�E�̍���x���W 
		(WINDOW_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2), // �E�B���h�E�̕�
		(WINDOW_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 +
			GetSystemMetrics(SM_CYCAPTION)), // �E�B���h�E�̍���
		NULL,					// �e�E�B���h�E�̃n���h��
		NULL,					// ���j���[�n���h���܂��͎q�E�B���h�EID
		_hInstance,				// �C���X�^���X�n���h��
		NULL);					// �E�B���h�E�쐬�f�[�^

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (!Init(_hInstance, hWnd, TRUE, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		return -1;
	}

	// �E�C���h�E�̕\��(�����������̌�ɍs��)
	ShowWindow(hWnd, _nWinMode);
	UpdateWindow(hWnd);
	timeBeginPeriod(1);
	timeBeginPeriod(1);	//����\(�����ׂ̍���)��F��
	int nLastTime = timeGetTime();
	int nFPSLastTime = timeGetTime();
	int nNowTime = 0;
	int nFPSCount = 0;

	// ���b�Z�[�W���[�v
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ���C������
			nNowTime = timeGetTime();
			if (nNowTime - nLastTime >= 1000 / 60) {
				nFPSCount++;
				nLastTime = nNowTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();
			}
			if (nNowTime - nFPSLastTime >= 1000) {
				nFPSLastTime = nNowTime;
				g_nFPS = nFPSCount;
				nFPSCount = 0;
			}
		}
	}

	//���̐ݒ�ɖ߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	//  ��P�����F���N���X��
	//  ��Q�����F�A�v���P�[�V�����C���X�^���X�̃n���h��
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	UnInit();

	return 0;
}


// �E�C���h�E�v���V�[�W��//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�I�����܂����H", "�I��", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "�I�����܂����H", "�I��", MB_YESNO);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	default:
		break;
	}
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
