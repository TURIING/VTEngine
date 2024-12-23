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
    EventType GetEventType() override { return EventType::MousePress; }
    static EventType GetStaticType() { return EventType::MousePress; }
    int GetEventKind() override { return EventKind::Mouse; }
    [[nodiscard]] MouseButton GetMouseButton() const { return m_button; }
    [[nodiscard]] Point GetPosition() const { return m_point; }

private:
    MouseButton m_button;
    Point m_point;
};

class MouseReleaseEvent : public Event {
public:
    EventType GetEventType() override { return EventType::MouseRelease; }
    static EventType GetStaticType() { return EventType::MouseRelease; }
    int GetEventKind() override { return EventKind::Mouse; }
    [[nodiscard]] MouseButton GetMouseButton() const { return m_button; }
    [[nodiscard]] Point GetPosition() const { return m_point; }

private:
    MouseButton m_button;
    Point m_point;
};

class MouseMoveEvent : public Event {
public:
    EventType GetEventType() override { return EventType::MouseMove; }
    static EventType GetStaticType() { return EventType::MouseMove; }
    int GetEventKind() override { return EventKind::Mouse; }
    [[nodiscard]] MouseButton GetMouseButton() const { return m_button; }
    [[nodiscard]] Point GetPosition() const { return m_point; }

private:
    MouseButton m_button;
    Point m_point;
};
#endif //UIEVENT_H
