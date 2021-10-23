#pragma once
#include "ImGuiWrapper.h"
#include <vector>
#include "GameObject.h"
#include <string>

// �Q�[���G���W���Ǘ��N���X
class GameEngine
{
private:
	std::vector<GameObject*> Object;
	bool InspecterFlg = false;
	bool HierarchieFlg = false;
	bool ConsoleFlg = false;
	int ChoiceIdx = -1;

public:
	GameEngine();
	bool Init();
	void Update();
	void Draw();
	void UnInit();
	void GuiDraw();
};

