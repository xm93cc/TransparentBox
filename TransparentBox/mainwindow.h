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
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>
#include "task.h"
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

signals:
    void clickIndex();


private slots:

    void on_exit_clicked();

    void on_TransparentBtn_clicked();

    void on_min_clicked();

    void sendListener();

    void on_listView_clicked(const QModelIndex &index);

    void startInitData();

private:
    Ui::MainWindow *ui;
    int boundaryWidth;
    QPoint clickPos;
    QStandardItemModel *processList;
    QString begin="cmd /c tasklist |findstr ";
    QString end="  > ./1.ini";
    QStringListModel *Model;
    QStandardItemModel *ItemModel;
    QStringList strListl;
    Task *task;

private:
    void initModel();
    void initData();


};

#endif // MAINWINDOW_H
