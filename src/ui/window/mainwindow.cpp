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
#include "app/Application.h"

MainWindow::MainWindow(const std::string &title, const Size &size, Application *app, QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow), m_pApp(app) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_PaintOnScreen);
    this->setWindowTitle(QString::fromStdString(title));
    this->resize(size.width, size.height);

    m_timer.setInterval(FRAME_TIME * 1000);
    connect(&m_timer, &QTimer::timeout, [this] {
        this->repaint();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::Update() {
    m_timer.start();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    m_pApp->Render();
    QWidget::paintEvent(event);
}
