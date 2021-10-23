#include "GameObject.h"
int GameObject::CreateIdx;

void TransformComponent::ParameterDraw()
{
	ImGui::SliderFloat("Pos.x", &Pos.x, 0.01f, 1.0f);
}
