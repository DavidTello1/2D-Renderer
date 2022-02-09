#pragma once
#include "Module.h"
#include "Event.h"

#include <map>
#include <list>
#include <typeindex>

class HandlerFunctionBase
{
public:
    void Exec(Event* e) { Call(e); } // Call the member function

private:
    virtual void Call(Event* e) = 0; // Implemented by MemberFunctionHandler
};

template<class T, class EventType>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
    typedef void (T::* MemberFunction)(EventType*);
    MemberFunctionHandler(T* instance, MemberFunction memberFunction) : instance{ instance }, memberFunction{ memberFunction } {};

    void Call(Event* e) { 
        (instance->*memberFunction)(static_cast<EventType*>(e)); // Cast event to the correct type and call member function
    } 

private:
    T* instance; // Pointer to class instance
    MemberFunction memberFunction; // Pointer to member function
};

//---------------------------
typedef std::list<HandlerFunctionBase*> HandlerList;
class ModuleEvent : public Module
{
public:
    ModuleEvent(bool start_enabled = true) : Module("ModuleEvent", start_enabled) {};
    virtual ~ModuleEvent() {};


    template<typename EventType>
    void Publish(EventType* evnt) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        if (handlers == nullptr)
            return;

        for (auto& handler : *handlers) 
        {
            if (handler != nullptr)
                handler->Exec(evnt);
        }
    }

    template<class T, class EventType>
    void Subscribe(T* instance, void (T::* memberFunction)(EventType*)) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        //First time initialization
        if (handlers == nullptr) 
        {
            handlers = new HandlerList();
            subscribers[typeid(EventType)] = handlers;
        }
        handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
    }

private:
    std::map<std::type_index, HandlerList*> subscribers;
};