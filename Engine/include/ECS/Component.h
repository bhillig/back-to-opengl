#pragma once

#include <Renderer/Model.h>

namespace ECS
{

class Component
{
public:
	virtual ~Component() {}

	void SetActive(bool active) { m_active = active; }
	inline bool IsActive() const { return m_active; }

private:
	bool m_active = false;
};

// For now will be very primitive and just be a xyz position
class TransformComponent : public Component
{
public:
	TransformComponent()
		: x(0), y(0), z(0) {}
	TransformComponent(double x, double y, double z)
		: x(x), y(y), z(z) {}
	double x;
	double y;
	double z;
};

class MeshComponent : public Component
{
public:
	MeshComponent() 
	: m_model(nullptr){}
	MeshComponent(Model* model)
		: m_model(model) {}

	inline Model* GetModel() { return m_model; }

private:
	Model* m_model;
};

} // namespace ECS