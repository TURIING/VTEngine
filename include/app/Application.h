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

class RenderRequestEvent;
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
    void render(RenderRequestEvent &event) const;

private:
    MainWindow *m_pWindow = nullptr;
    std::shared_ptr<RHIContext> m_pRHIContext;
};
#endif //APPLICATION_H