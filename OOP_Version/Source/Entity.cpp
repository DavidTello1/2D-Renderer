#include "Entity.h"

#include "Application.h"
#include "ModuleScene.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentRenderer.h"
#include "ComponentSprite.h"
#include "ComponentRectCollider.h"
#include "ComponentCircleCollider.h"
#include "ComponentAsteroid.h"

#include "mmgr/mmgr.h"

Entity::~Entity()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) 
	{
		if (*it) 
			delete* it;
	}
	components.clear();
}

Component* Entity::AddComponent(Component::Type type)
{
	Component* component;
	switch (type)
	{
	case Component::Type::TRANSFORM:		component = new ComponentTransform(this);		break;
	case Component::Type::CAMERA:			component = new ComponentCamera(this);			break;
	case Component::Type::RENDERER:			component = new ComponentRenderer(this);		break;
	case Component::Type::SPRITE:			component = new ComponentSprite(this);			break;
	case Component::Type::RECT_COLLIDER:	component = new ComponentRectCollider(this);	break;
	case Component::Type::CIRCLE_COLLIDER:	component = new ComponentCircleCollider(this);	break;
	case Component::Type::ASTEROID:			component = new ComponentAsteroid(this);		break;
	case Component::Type::UNKNOWN:			return nullptr;
	}
	components.push_back(component);
	return component;
}

void Entity::DeleteComponent(Component::Type component)
{
	for (size_t i = 0, size = components.size(); i < size; ++i)
	{
		if (components[i]->GetType() == component)
			components.erase(components.begin() + i);
	}
}

Component* Entity::GetComponent(Component::Type component) const
{
	for (Component* comp : components)
	{
		if (comp->GetType() == component)
			return comp;
	}
	return nullptr;
}

void Entity::Draw()
{
	ComponentRenderer* renderer = (ComponentRenderer*)GetComponent(Component::Type::RENDERER);
	if (renderer != nullptr)
	{
		renderer->Draw();

		if (App->scene->IsDebug())
			App->scene->DrawDebug();
	}
}
