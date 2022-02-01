#include "ModuleEvent.h"

ModuleEvent::ModuleEvent(bool start_enabled) : Module("ModuleEvent", start_enabled)
{
}

ModuleEvent::~ModuleEvent()
{
}

bool ModuleEvent::PreUpdate(float dt)
{
	// Process Events
	while (head != tail) 
	{
		int event_type = static_cast<int>(events[head].GetType());
		for (std::vector<Function>::iterator it = listeners[event_type].begin(); it != listeners[event_type].end(); ++it) 
		{
			//***check if we can call the function, delete listener if not accessible
			(*it)(events[head]);
		}
		head = (head + 1) % MAX_EVENTS;
	}

	return true;
}

bool ModuleEvent::CleanUp() 
{
	for (uint i = 0; i < EVENT_TYPES; ++i)
		listeners[i].clear();

	return true;
}

//--------------------------------------
void ModuleEvent::PushEvent(Event& new_event) 
{
	events[tail] = new_event;
	tail = (tail + 1) % MAX_EVENTS;
}

void ModuleEvent::AddListener(Event::EventType type, Function callback)
{
	bool found = false;
	int event_type = static_cast<int>(type);
	for (std::vector<Function>::iterator it = listeners[event_type].begin(); it != listeners[event_type].end(); ++it) 
	{
		if (*it == callback) 
		{
			found = true;
			break;
		}
	}

	if (!found)
		listeners[event_type].push_back(callback);
}

void ModuleEvent::RemoveListener(Event::EventType type, Function callback) 
{
	int event_type = static_cast<int>(type);
	for (std::vector<Function>::iterator it = listeners[event_type].begin(); it != listeners[event_type].end(); ++it) 
	{
		if (*it == callback) 
		{
			listeners[event_type].erase(it);
			break;
		}
	}
}
