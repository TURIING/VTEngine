/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/23 22:03
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef EVENT_H
#define EVENT_H

#include "common/common.h"

enum class EventType {
    RenderRequest,
    WindowClose, WindowResize,
    MousePress, MouseRelease, MouseMove,
    KeyPress, KeyRelease
};

enum EventKind {
    App = BIT(0),
    Window = BIT(1),
    Mouse = BIT(2),
    KeyBoard = BIT(3),
};

class Event {
public:
    virtual EventType GetEventType() = 0;
    virtual int GetEventKind() = 0;
    virtual std::string GetEventName() { return typeid(*this).name(); }
    bool isBelongEventKind(EventKind kind) { return GetEventKind() & kind; }
};

class EventDispatcher
{
    template<typename T>
    using EventFunc = std::function<void(T&)>;

public:
    explicit EventDispatcher(Event& event) : m_event(event) {}

    template<typename T>
    bool dispatch(EventFunc<T> func)
    {
        if (m_event.GetEventType() == T::GetStaticType())
        {
            func(*static_cast<T *>(&m_event));
            return true;
        }
        return false;
    }
private:
    Event& m_event;
};

#endif //EVENT_H
