#pragma once
#include "Module.h"

#include <vector>

#define MAX_EVENTS 1000
#define EVENT_TYPES 7

typedef void (*Function)(const Event& e);
typedef std::vector<Function> Listeners;

struct Event {
	enum class EventType {
		INVALID = 0,
		ENTITY_DESTROYED,


	};
	Event(EventType type) : type(type) {}
	Event() {}
	EventType GetType() const { return type; };

private:
	EventType type = EventType::INVALID;
};

class ModuleEvent : public Module
{
public:
	ModuleEvent(bool start_enabled = true);
	virtual ~ModuleEvent();

	bool PreUpdate(float dt) override;
	bool CleanUp() override;

	void PushEvent(Event& new_event);
	void AddListener(Event::EventType type, Function callback);
	void RemoveListener(Event::EventType type, Function callback);

private:
	Event events[MAX_EVENTS];
	Listeners listeners[EVENT_TYPES];

	uint head;
	uint tail;
};

