#pragma once
#include "Camera.h"

namespace Scenes {
	class Scene {
	public:
		Camera camera;

		Scene() {}
		virtual ~Scene() {} 

		virtual void OnUpdate(float deltaTime){}
		virtual void OnRender() {}
		virtual void OnImGuiRender(){}
	};
}


