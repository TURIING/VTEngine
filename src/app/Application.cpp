/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 23:39
* @version: 1.0
* @description: 
********************************************************************************/
#include "app/Application.h"
#include "ui/window/mainwindow.h"
#include "app/Event.h"
#include "app/UIEvent.h"
#include "instance/InstanceBase.h"
#include "instance/ModelLoadInstance.h"

typedef InstanceBase* (*CreateInstanceFunc)(const PlatformWindowInfo &info);

template <class T>
InstanceBase *createFunc(const PlatformWindowInfo &info) {
    return new T(info);
}

enum class Instance { ModelLoad, };

const std::unordered_map<Instance, CreateInstanceFunc> gInstanceMap = {
    { Instance::ModelLoad, &::createFunc<ModelLoadInstance> },
};

Application::Application() {
    m_pWindow = new MainWindow(APP_NAME, WINDOW_SIZE, this);
    m_pWindow->show();
    this->switchInstance(Instance::ModelLoad);
}

void Application::Run() const {
    m_pWindow->Update();
}

void Application::ProcessEvent(Event &event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<RenderRequestEvent>(std::bind(&Application::renderEvent, this, std::placeholders::_1));
    dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::resizeEvent, this, std::placeholders::_1));
    dispatcher.dispatch<InstanceSwitchEvent>(std::bind(&Application::switchInstanceEvent, this, std::placeholders::_1));
    dispatcher.dispatch<MousePressEvent>(std::bind(&Application::mousePressEvent, this, std::placeholders::_1));
    dispatcher.dispatch<MouseMoveEvent>(std::bind(&Application::mouseMoveEvent, this, std::placeholders::_1));
    dispatcher.dispatch<MouseReleaseEvent>(std::bind(&Application::mouseReleaseEvent, this, std::placeholders::_1));
    dispatcher.dispatch<MouseWheelScrollEvent>(std::bind(&Application::mouseWheelScrollEvent, this, std::placeholders::_1));
}

Application::~Application() {
}

void Application::renderEvent(RenderRequestEvent &event) const {
    LOG_ASSERT(m_pCurInstance);
    m_pCurInstance->OnRender();
}

void Application::resizeEvent(const WindowResizeEvent &event) const {
    LOG_ASSERT(m_pCurInstance);

    m_pCurInstance->OnResize(event.GetSize());
}

void Application::switchInstanceEvent(const InstanceSwitchEvent& event) {
    this->switchInstance(event.GetInstanceType());
}

void Application::mousePressEvent(const MousePressEvent& event) const {
}

void Application::mouseMoveEvent(const MouseMoveEvent& event) const {
    LOG_ASSERT(m_pCurInstance);
    m_pCurInstance->OnMouseMove(event.GetMouseButton(), event.GetPosition());
}

void Application::mouseReleaseEvent(const MouseReleaseEvent& event) const {
}

void Application::mouseWheelScrollEvent(const MouseWheelScrollEvent& event) const {
    LOG_ASSERT(m_pCurInstance);
    m_pCurInstance->OnMouseScroll(event.GetDelta());
}

void Application::switchInstance(const Instance instance) {
    if(!m_pCurInstance) {
        delete m_pCurInstance;
    }

    const PlatformWindowInfo winInfo {
        .handle = reinterpret_cast<void *>(m_pWindow->GetSurfaceHandle()),
        .size = m_pWindow->GetSurfaceSize(),
    };

    if(const auto it = gInstanceMap.find(instance); it != gInstanceMap.end()) {
        m_pCurInstance = it->second(winInfo);
    }
    else {
        LOG_ASSERT(false);
    }
}
