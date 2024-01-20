#pragma once

namespace Scenes {
	class Scene {
	public:
		Scene() {}
		virtual ~Scene() {} 

		virtual void OnUpdate(float deltaTime){}
		virtual void OnRender(glm::mat4 MVP) {}
		virtual void OnImGuiRender(){}
	};
}


