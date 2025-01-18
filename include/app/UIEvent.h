/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/23 22:28
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef UIEVENT_H
#define UIEVENT_H

#include "common/common.h"
#include "app/Event.h"

class RenderRequestEvent: public Event {
public:
    EventType GetEventType() override { return EventType::RenderRequest; }
    static EventType GetStaticType() { return EventType::RenderRequest; }
    int GetEventKind() override { return EventKind::App; }
};

class InstanceSwitchEvent: public Event {
public:
    explicit InstanceSwitchEvent(Instance instance): m_instance(instance) {}
    EventType GetEventType() override { return EventType::InstanceSwitch; }
    static EventType GetStaticType() { return EventType::InstanceSwitch; }
    int GetEventKind() override { return EventKind::App; }
    [[nodiscard]] Instance GetInstanceType() const { return m_instance; }

private:
    Instance m_instance;
};

class WindowResizeEvent : public Event {
public:
    explicit WindowResizeEvent(const Size size): m_size(size) {}
    EventType GetEventType() override { return EventType::WindowResize; }
    static EventType GetStaticType() { return EventType::WindowResize; }
    int GetEventKind() override { return EventKind::Window; }
    [[nodiscard]] Size GetSize() const { return m_size; }

private:
    Size m_size;
};

class WindowCloseEvent : public Event {
public:
    EventType GetEventType() override { return EventType::WindowClose; }
    static EventType GetStaticType() { return EventType::WindowClose; }
    int GetEventKind() override { return EventKind::Window; }
};

class MousePressEvent : public Event {
public:
    MousePressEvent(MouseButton button, const Point &pos): m_button(button), m_pos(pos) {}
    EventType GetEventType() override { return EventType::MousePress; }
    static EventType GetStaticType() { return EventType::MousePress; }
    int GetEventKind() override { return EventKind::Mouse; }
    [[nodiscard]] MouseButton GetMouseButton() const { return m_button; }
    [[nodiscard]] Point GetPosition() const { return m_pos; }

private:
    MouseButton m_button {};
    Point m_pos;
};

class MouseReleaseEvent : public Event {
public:
    MouseReleaseEvent(MouseButton button, const Point &pos): m_button(button), m_pos(pos) {}
    EventType GetEventType() override { return EventType::MouseRelease; }
    static EventType GetStaticType() { return EventType::MouseRelease; }
    int GetEventKind() override { return EventKind::Mouse; }
    [[nodiscard]] MouseButton GetMouseButton() const { return m_button; }
    [[nodiscard]] Point GetPosition() const { return m_pos; }

private:
    MouseButton m_button;
    Point m_pos;
};

class MouseMoveEvent : public Event {
public:
    MouseMoveEvent(MouseButton button, const Point &pos): m_button(button), m_pos(pos) {}
    EventType GetEventType() override { return EventType::MouseMove; }
    static EventType GetStaticType() { return EventType::MouseMove; }
    int GetEventKind() override { return EventKind::Mouse; }
    [[nodiscard]] MouseButton GetMouseButton() const { return m_button; }
    [[nodiscard]] Point GetPosition() const { return m_pos; }

private:
    MouseButton m_button;
    Point m_pos;
};

class MouseWheelScrollEvent : public Event {
public:
    explicit MouseWheelScrollEvent(double delta): m_delta(delta) {}
    EventType GetEventType() override { return EventType::MouseWheelScroll; }
    static EventType GetStaticType() { return EventType::MouseWheelScroll; }
    int GetEventKind() override { return EventKind::Mouse; }
    [[nodiscard]] double GetDelta() const { return m_delta; }

private:
    double m_delta = 0;
};
#endif //UIEVENT_H
