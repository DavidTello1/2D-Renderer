#pragma once

class Event {
protected:
    virtual ~Event() {};
};

//------------------------
// --- MAIN EVENTS ---
struct EventWindowResize : public Event 
{
    EventWindowResize(int w, int h) : width(w), height(h) {};
    int width;
    int height;
};

struct EventEntityDestroyed : public Event
{
    EventEntityDestroyed(EntityIdx index) : entity(index) {};
    EntityIdx entity;
};

struct EventEntityMaskChanged : public Event
{

};

// --- GAME EVENTS ---
struct EventWorldSizeUpdate : public Event
{
    EventWorldSizeUpdate(int w, int h) : width(w), height(h) {};
    int width;
    int height;
};

struct EventCameraZoom : public Event
{
    EventCameraZoom(int z) : zoom(z) {};
    int zoom;
};

struct EventCameraSpeedChanged : public Event
{
    EventCameraSpeedChanged(float s) : speed(s) {};
    float speed;
};


// --- GUI EVENTS ---
struct EventAsteroidAdded : public Event
{
    EventAsteroidAdded(int n) : num(n) {};
    int num;
};

struct EventAsteroidRemoved : public Event
{
    EventAsteroidRemoved(int n) : num(n) {};
    int num;
};
