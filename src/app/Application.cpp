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

Application::Application() {
    m_pWindow = new MainWindow(APP_NAME, WINDOW_SIZE, this);
    const PlatformWindowInfo winInfo {
        .handle = reinterpret_cast<void *>(m_pWindow->winId()),
        .size = WINDOW_SIZE,
    };
    m_pRHIContext = std::make_shared<RHIContext>(winInfo);
    m_pWindow->show();
}

void Application::Run() const {
    m_pWindow->Update();
}

Application::~Application() {
}

void Application::Render() const {
    m_pRHIContext->Render();
}