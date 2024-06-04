#pragma once
#include <unordered_map>
#include "EntitySystemTypes.h"
class EntityComponent;


class Entity
{
	EntityId m_id;
	std::unordered_map<EntityComponentId, EntityComponent*> m_components;

public:
	explicit Entity(EntityId id) : m_id(id) {}
	EntityId GetId() const { return m_id; }

	template<class CompType>
	CompType* GetComponent() const
	{
		auto it = m_components.find(CompType::ID);
		if (it != m_components.end())
			return static_cast<CompType*>(it->second);
		return nullptr;
	}


};
