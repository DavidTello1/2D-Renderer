#pragma once
#include "System.h"

struct EventWindowResize;
struct EventCameraZoom;
struct EventCameraSpeedChanged;

class S_CameraController : public System
{
public:
    S_CameraController();
    ~S_CameraController();

    void Init() override;
    void Update(float dt) override;

private:
    // Events
    void OnResize(EventWindowResize* e);
    void OnZoom(EventCameraZoom* e);
    void OnSpeedChange(EventCameraSpeedChanged* e);
};
