#include <GLFW/glfw3.h>
#include <Society/Core/Window.h>
#include <Society/Managers/InputSystem.h>

namespace sc {
	InputSystem::InputSystem(): m_keys{}, m_buttons{}, m_lastPos() {
		m_firstMouse = true;
		// m_lastPos = glm::vec2(1280 * 0.5f, 768 * 0.5f);

		for (auto &m_key: m_keys) {
			m_key = static_cast<unsigned int>(KeyState::Released);
		}
	}

	bool InputSystem::IsKeyPressed(const int key) const {
		if (m_keys[key] == static_cast<unsigned int>(KeyState::Pressed))
			return true;
		return false;
	}

	bool InputSystem::IsKeyReleased(const int key) const {
		if (m_keys[key] == static_cast<unsigned int>(KeyState::Released))
			return true;
		return false;
	}

	bool InputSystem::IsKeyHeld(const int key) const {
		if (m_keys[key] == static_cast<unsigned int>(KeyState::Held))
			return true;
		return false;
	}

	bool InputSystem::GetButton(const int button) const {
		if (m_buttons[button] == static_cast<unsigned int>(KeyState::Pressed))
			return true;
		return false;
	}

	glm::vec2 InputSystem::GetMouseOffset(
		const float xposIn,
		const float yposIn
	) {
		const auto pos = glm::vec2(xposIn, yposIn);

		if (m_firstMouse) {
			m_lastPos = pos;
			m_firstMouse = false;
		}

		const auto offset = glm::vec2(
			pos.x - m_lastPos.x,
			m_lastPos.y - pos.y
		);

		m_lastPos = pos;

		return offset;
	}
}
