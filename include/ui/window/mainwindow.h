/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 22:22
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTimer>

struct Size;
class Application;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(const std::string &title, const Size &size, Application *app, QWidget *parent = nullptr);
    ~MainWindow() override;
    void Update();
    [[nodiscard]] void *GetSurfaceHandle() const;
    [[nodiscard]] Size GetSurfaceSize() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    inline void init(const std::string &title, const Size &size);
    inline void createSurfaceWindow();

private:
    Ui::MainWindow *ui;
    QTimer m_timer;
    Application *m_pApp = nullptr;
    QWindow *m_pSurface = nullptr;
};


#endif //MAINWINDOW_H
