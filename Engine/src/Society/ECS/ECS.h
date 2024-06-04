
/*
	Enities		<--		Components
							^
					Transform Renderable

			Systems
			   ^
  TransformSystem	RenderSystem

	Systems		-->		Manager		<--		Entities
*/

/*
Game Ideas:
	- Robots asking about der purpose
	- Robots trying to find a way to escape
	- Player creates robots to help with simple tasks

*/

class EntityManager {
public:
	void Update(float dt) {
		for (System* pSystem : m_systems) {
			pSystem->Update(dt);
		}
	}
}

class MoverSystem {
public:
	void Update(float dt, Context* pContext) {
		Move(pContext->m_ptransform->m_pos);
		ValidatePos(m_pPhysicsObject->m_PhysicsActor, pos);
		pContext->m_pTransform->SetPos(pos);
	}
}
