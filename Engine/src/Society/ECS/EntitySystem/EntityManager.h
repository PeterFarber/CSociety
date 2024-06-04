#pragma once
#include "EntitySystemTypes.h"
#include "Entity.h"
#include <vector>
#include <map>
#include <functional>

class ComponentSystem;
class Component;
class EntityManager
{
	using Entities = std::unordered_map<EntityId, Entity>;
	using Components = std::vector<std::vector<Component*>>;
	using Systems = std::vector<ComponentSystem>;


	Entities m_entities;
	Components m_components;
	Systems m_systems;

public:

	EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
	EntityManager(EntityManager&&) = delete;
	EntityManager& operator=(EntityManager&&) = delete;
	~EntityManager();



	void CreateSystems();
	void Update(double deltaMs);

	EntityId CreateEntity();
	void DestroyEntity(EntityId id);

};
