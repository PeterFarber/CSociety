#pragma once
#include "EntitySystemTypes.h"
#include <limits>

class EntityComponent
{
private:
	// Todo: Consider using uint64
	static constexpr size_t INVALID_POOL_INDEX = std::numeric_limits<size_t>::max();

	EntityId m_entityId;
	size_t m_poolIndex;
public:
	EntityComponent() : m_entityId(INVALID_ENTITY_ID), m_poolIndex(INVALID_POOL_INDEX) {}
	virtual ~EntityComponent() = default;
	virtual bool Init(void* pDescription) = 0; // TODO


	void SetEntityId(EntityId id) { m_entityId = id; } //TODO: private?
	EntityId GetEntityId() const { return m_entityId; }

	void SetPoolIndex(size_t index) { m_poolIndex = index; }
	size_t GetPoolIndex() const { return m_poolIndex; }
};
