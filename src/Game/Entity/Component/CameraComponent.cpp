#include "pch.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent(Entity* entity)
	: Component(entity)
{
}

CameraComponent::CameraComponent(float fov, float aspectRatio, float near, float far, Entity* entity)
	: Component(entity), m_Camera(fov, aspectRatio, near, far)
{
}

