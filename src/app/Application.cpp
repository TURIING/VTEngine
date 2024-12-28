/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 23:39
* @version: 1.0
* @description: 
********************************************************************************/
#include "app/Application.h"
#include "core/RHI/RHIContext.h"
#include "ui/window/mainwindow.h"
#include "app/Event.h"
#include "app/UIEvent.h"

Application::Application() {
    m_pWindow = new MainWindow(APP_NAME, WINDOW_SIZE, this);
    m_pWindow->show();
    const PlatformWindowInfo winInfo {
        .handle = reinterpret_cast<void *>(m_pWindow->GetSurfaceHandle()),
        .size = WINDOW_SIZE,
    };
    m_pRHIContext = std::make_shared<RHIContext>(winInfo);
}

void Application::Run() const {
    m_pWindow->Update();
}

void Application::ProcessEvent(Event &event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<RenderRequestEvent>(std::bind(&Application::render, this, std::placeholders::_1));
    dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::resize, this, std::placeholders::_1));
}

Application::~Application() {
}

void Application::render(RenderRequestEvent &event) const {
    m_pRHIContext->Render();
}

void Application::resize(WindowResizeEvent &event) const {
    m_pRHIContext->Resize(event.GetSize());
}
