#ifndef SC_INPUTSYSTEM_H
#define SC_INPUTSYSTEM_H
#include <glm/vec2.hpp>
#include <Society/Interfaces/ISingleton.h>
#include <Society/Core/Base.h>

namespace sc {
	enum class KeyState {
		Released, Pressed, Held
	};

	class DECLSPEC InputSystem {
		private:
			unsigned int m_keys[1024];
			unsigned int m_buttons[3];
			bool m_firstMouse;
			glm::vec2 m_lastPos;

		public:
			InputSystem();

			bool IsKeyPressed(int key) const;

			bool IsKeyReleased(int key) const;

			bool IsKeyHeld(int key) const;

			bool GetButton(int button) const;

			void SetKeyState(const int key, const int state) {
				m_keys[key] = static_cast<unsigned int>(state);
			}

			void SetMouseState(const int button, const int action) {
				m_buttons[button] = static_cast<unsigned int>(action);
			}

			glm::vec2 GetMouseOffset(float xposIn, float yposIn);
	};
}

#endif
