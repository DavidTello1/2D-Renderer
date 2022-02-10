#pragma once
#include "Globals.h"

#include <string>

#define MODULE_NAME_LENGTH 25

class Module
{
public:
	Module(const char* name, bool start_enabled = true) : enabled(start_enabled) { strcpy_s(this->name, MODULE_NAME_LENGTH, name); }
	virtual ~Module() {}

	virtual bool Init() { return true; }
	virtual bool Start() { return true; }
	virtual bool PreUpdate(float dt) { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate(float dt) { return true; }
	virtual bool CleanUp() { return true; }

	const char* GetName() const { return name; }
	bool IsEnabled() const { return enabled; }

	void SetEnabled(bool active)
	{
		if (enabled != active)
		{
			enabled = active;
			if (active == true)
				Start();
			else
				CleanUp();
		}
	}

private:
	bool enabled;
	char name[MODULE_NAME_LENGTH];
};