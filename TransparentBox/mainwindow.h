#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<windows.h>
#include<dwmapi.h>
#include <QMainWindow>
#include<stdlib.h>
#include <QMouseEvent>
#include <QWidget>
#include <windowsx.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();


private:
    Ui::MainWindow *ui;
    int boundaryWidth;
    QPoint clickPos;
};

#endif // MAINWINDOW_H
