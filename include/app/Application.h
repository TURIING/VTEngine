/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 23:39
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "common/common.h"

class MouseWheelScrollEvent;
class MouseReleaseEvent;
class MouseMoveEvent;
class MousePressEvent;
class InstanceSwitchEvent;
class InstanceBase;
enum class Instance;
class RenderRequestEvent;
class WindowResizeEvent;
class RHIContext;
class MainWindow;
class Event;

class Application {
public:
    Application();
    ~Application();
    void Run() const;
    void ProcessEvent(Event &event);

private:
    void renderEvent(RenderRequestEvent &event) const;
    void resizeEvent(const WindowResizeEvent &event) const;
    void switchInstanceEvent(const InstanceSwitchEvent &event);
    void mousePressEvent(const MousePressEvent &event) const;
    void mouseMoveEvent(const MouseMoveEvent &event) const;
    void mouseReleaseEvent(const MouseReleaseEvent &event) const;
    void mouseWheelScrollEvent(const MouseWheelScrollEvent &event) const;
    void switchInstance(const Instance instance);

private:
    MainWindow *m_pWindow = nullptr;
    std::shared_ptr<RHIContext> m_pRHIContext;
    InstanceBase *m_pCurInstance = nullptr;
};
#endif //APPLICATION_H