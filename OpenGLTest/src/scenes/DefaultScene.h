#pragma once

#include "Scene.h"
#include "stdafx.h"
#include "imgui/imgui.h"

namespace Scenes {
	class Test_Default : public Scene {
	public:
		Test_Default() {}
		~Test_Default() {}

		void OnUpdate(float deltaTime) override {};
		void OnRender() override {
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void OnImGuiRender()  override {}

	private:
	};
}