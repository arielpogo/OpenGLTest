#pragma once

#include "Scene.h"
#include "stdafx.h"
#include "imgui/imgui.h"

namespace Scenes {
	class Test_ClearColor : public Scene {
	public:
		Test_ClearColor() : m_ClearColor{ 0.3f, 0.3f, 0.0f, 1.0f } {}
		~Test_ClearColor() {}

		void OnUpdate(float deltaTime) override {};
		void OnRender(glm::mat4 MVP) override {
			glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void OnImGuiRender()  override {
			ImGui::ColorEdit4("Clear Color", m_ClearColor);
		}

	private:
		float m_ClearColor[4];
	};
}