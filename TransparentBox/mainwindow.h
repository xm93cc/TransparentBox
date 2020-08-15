#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<windows.h>
#include<dwmapi.h>
#include <QMainWindow>
#include<stdlib.h>
#include <QMouseEvent>
#include <QWidget>
#include <windowsx.h>
#include<QStandardItemModel>
#include<QFileDialog>
#include <shellapi.h>
#include<QMessageBox>

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
    void on_exit_clicked();

    void on_TransparentBtn_clicked();

private:
    Ui::MainWindow *ui;
    int boundaryWidth;
    QPoint clickPos;
    QStandardItemModel *processList;
    QString begin="cmd /c tasklist |findstr ";
    QString end="  > ./1.ini";

private:
    void initModel();
    void initData();


};

#endif // MAINWINDOW_H
