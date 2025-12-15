#include <ECS/EntityManager.h>

namespace ECS
{

EntityManager::EntityManager(size_t entityCount)
    : m_entities(), m_entityPool(entityCount), m_entityCount(0)
{
    m_entities.reserve(entityCount);
}

Entity& EntityManager::CreateEntity()
{
    const size_t newEntityId = m_entityCount;
   
    m_entityPool.AddEntity(newEntityId);
    m_entities.emplace_back(Entity(newEntityId, &m_entityPool));

    m_entityCount++;
    return m_entities.back();
}

Entity& EntityManager::GetEntity(size_t id)
{
    return m_entities[id];
}

} // namespace ECS