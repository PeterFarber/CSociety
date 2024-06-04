// #pragma once
// #include "../helpers.h"
// #include "../EntitySystem/EntityComponent.h"
// #include "../EntitySystem/EntitySystemTypes.h"

// #include <glm/glm.hpp>



// COUNTER_INC(ComponentAutoCounterId);
// class TransformComponent : public EntityComponent
// {
// 	static constexpr ComponentId ID = COUNTER_READ(ComponentAutoCounterId) - 1;
// 	glm::vec3 m_position;
// 	glm::vec3 m_rotation;
// 	glm::vec3 m_scale;

// public:
// 	TransformComponent() : m_position(0.0f), m_rotation(0.0f), m_scale(1.0f) {}

// 	virtual bool Init(void* pDescription) override { return true; }

// 	MEMORYPOOL_DECLARATION(32);



// 	// void SetPosition(const glm::vec3& position) { m_position = position; }
// 	// const glm::vec3& GetPosition() const { return m_position; }

// 	// void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
// 	// const glm::vec3& GetRotation() const { return m_rotation; }

// 	// void SetScale(const glm::vec3& scale) { m_scale = scale; }
// 	// const glm::vec3& GetScale() const { return m_scale; }
// };
