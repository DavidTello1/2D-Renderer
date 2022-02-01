#pragma once
#include "Module.h"

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include "PCG/pcg_basic.h"
#include <memory>

class S_Renderer;
class S_CameraController;
class S_Physics;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void Draw();
	pcg32_random_t& GetRNG() { return rng; }

	//*** UNTIL EVENT MANAGER
	void OnResize(int width, int height);
	void OnZoom(int new_zoom);

	// --- ENTITY COMPONENT SYSTEM ---
    // Entities
    Entity CreateEntity() {
        return mEntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity) {
        mEntityManager->DestroyEntity(entity);
        mComponentManager->EntityDestroyed(entity);
        mSystemManager->EntityDestroyed(entity);
    }


    // Components
    template<typename T>
    void RegisterComponent() {
        mComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        mComponentManager->AddComponent<T>(entity, component);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        mComponentManager->RemoveComponent<T>(entity);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return mComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType() {
        return mComponentManager->GetComponentType<T>();
    }


    // Systems
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        return mSystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature) {
        mSystemManager->SetSignature<T>(signature);
    }

private:
	pcg32_random_t rng;

	// Managers
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;

	// Systems
	std::shared_ptr<S_Renderer> render_system;
	std::shared_ptr<S_CameraController> camera_system;
	std::shared_ptr<S_Physics> physics_system;
};
