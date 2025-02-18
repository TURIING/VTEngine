/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 22:22
* @version: 1.0
* @description: 
********************************************************************************/

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ui/window/mainwindow.h"
#include "ui_MainWindow.h"
#include <QWindow>
#include <QSurface>
#include <QResizeEvent>
#include "app/Application.h"
#include "app/UIEvent.h"

MainWindow::MainWindow(const std::string &title, const Size &size, Application *app, QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow), m_pApp(app) {
    ui->setupUi(this);
    this->init(title, size);

    m_timer.setInterval(FRAME_TIME * 1000);
    connect(&m_timer, &QTimer::timeout, [this] {
        RenderRequestEvent renderRequestEvent;
        m_pApp->ProcessEvent(renderRequestEvent);
        this->repaint();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::Update() {
    m_timer.start();
}

void *MainWindow::GetSurfaceHandle() const {
    return reinterpret_cast<void *>(m_pSurface->winId());
}

void MainWindow::createSurfaceWindow() {
    m_pSurface = new QWindow();

#if PLATFORM_MACOS
    m_pSurface->setSurfaceType(QSurface::VulkanSurface);
#endif
    const auto surfaceWidget = this->createWindowContainer(m_pSurface, this);

    const auto layout = new QHBoxLayout(ui->pSurfaceWidget);
    layout->setContentsMargins(QMargins());
    layout->addWidget(surfaceWidget);
}

void MainWindow::init(const std::string &title, const Size &size) {
    this->setWindowTitle(QString::fromStdString(title));
    this->resize(size.width, size.height);
    this->createSurfaceWindow();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if(m_timer.isActive()) {
        const auto size = this->size();
        WindowResizeEvent resizeEvent(Size { static_cast<uint32_t>(size.width()), static_cast<uint32_t>(size.height())});
        resizeEvent.SetFunc([this] {
            const auto size = this->size();
            const auto surfaceSize = m_pSurface->size();
            LOG_INFO("The window size has changed, MainWindow size({}, {}), Surface widget size({}, {})", size.width(), size.height(), surfaceSize.width(), surfaceSize.height());
        });
        m_pApp->ProcessEvent(resizeEvent);
    }
    QWidget::resizeEvent(event);
}