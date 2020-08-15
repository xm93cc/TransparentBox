#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(Qt::FramelessWindowHint);//需要去掉标题栏
//        this->setAttribute(Qt::WA_TranslucentBackground);
//        this->setStyleSheet("#centralWidget{background:rgba(0,255,255,0.4);}"
//            "#label{background:rgba(255,255,0,1);}"
//            "#widget{background:rgba(0,255,255,1);}");

    setWindowOpacity(0.7);
    this->setWindowFlags(Qt::FramelessWindowHint);
       boundaryWidth=4;                                    //设置触发resize的宽度
       this->setMinimumSize(317,546);                        //设置最小尺寸
       this->setMaximumSize(317,546);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    DWM_BLURBEHIND *pd;
    pd->dwFlags=1;
    pd->fEnable=1;


  // HWND getwin= FindWindowA(nullptr, "C:\\Users\\QDebug\\Desktop\\new 1 - Notepad++");
 //  DwmEnableBlurBehindWindow(getwin,pd);
  // qDebug()<<getwin<<endl;
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton)
        move(e->pos()+pos()-clickPos);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    switch(msg->message)
    {
    case WM_NCHITTEST:
        int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if(xPos < boundaryWidth && yPos<boundaryWidth)                    //左上角
            *result = HTTOPLEFT;
        else if(xPos>=width()-boundaryWidth&&yPos<boundaryWidth)          //右上角
            *result = HTTOPRIGHT;
        else if(xPos<boundaryWidth&&yPos>=height()-boundaryWidth)         //左下角
            *result = HTBOTTOMLEFT;
        else if(xPos>=width()-boundaryWidth&&yPos>=height()-boundaryWidth)//右下角
            *result = HTBOTTOMRIGHT;
        else if(xPos < boundaryWidth)                                     //左边
            *result =  HTLEFT;
        else if(xPos>=width()-boundaryWidth)                              //右边
            *result = HTRIGHT;
        else if(yPos<boundaryWidth)                                       //上边
            *result = HTTOP;
        else if(yPos>=height()-boundaryWidth)                             //下边
            *result = HTBOTTOM;
        else              //其他部分不做处理，返回false，留给其他事件处理器处理
            return false;
        return true;
    }
    return false;         //此处返回false，留给其他事件处理器处理
}


