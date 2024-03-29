#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QRegExpValidator>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     this->setWindowIcon(QIcon(":/../res/home-ico.ico"));
    setWindowOpacity(0.8);
    this->setWindowFlags(Qt::FramelessWindowHint);
    boundaryWidth=4;                                    //设置触发resize的宽度
    this->setMinimumSize(380,644);                        //设置最小尺寸
    this->setMaximumSize(380,644);

    initData();
    QLineEdit *edPid=ui->EdPid;
    QRegExp regx("[1-9][0-9][0-9][0-9][0-9]$");
    QValidator *validator = new QRegExpValidator(regx, edPid );
    edPid->setValidator( validator );
    edPid->setPlaceholderText("1~65536");


    QLineEdit *edLevel=ui->EdLevel;
    QRegExp regxl("[1-9][0-9][0-9]$");
    QValidator *validatoredLevel = new QRegExpValidator(regxl, edPid );
    edLevel->setValidator( validatoredLevel );
    edLevel->setPlaceholderText("1~255");
    initData();
    this->task=new Task();
    this->task->start();
    this->setWindowFlags(Qt::ToolTip);
    createSystemTray();
    connect(task,&Task::isStartInit,this,&MainWindow::startInitData);
}

void MainWindow::startInitData()
{
    initData();
}

MainWindow::~MainWindow()
{
    delete ui;
}
typedef struct myWindowInfo
{
    HWND  hwnd1;
    char WindowText[255];
    char ClassName[255];
    DWORD dwProcessId;
    DWORD dwThreadId;
}WindowInfo;

typedef WindowInfo* PWindowInfo;
typedef WindowInfo* LPWindowInfo;

DWORD EnumWindowInfo(WindowInfo WInfo[]);
HWND m_hwndFind[255];
int m_num = 0;

BOOL CALLBACK EnumCallback(HWND hwnd, LPARAM lParam,int t)
{
    DWORD bAlpha = 0;
    GetLayeredWindowAttributes(hwnd, 0L, (BYTE*)&bAlpha, NULL);   // 获得当前窗口的透明度（255为不透明）

    if (bAlpha == 100)                                             // 如果当前透明度为100跳过该窗口
        return TRUE;
    /*HWND getwin = FindWindowA(NULL, "C:\\Users\\QDebug\\Desktop\\new 1 - Notepad++");


    DWM_BLURBEHIND pd = {};
    pd.dwFlags = 1;
    pd.fEnable = 1;*/
    SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED);               // 给窗口增加透明属性
    SetLayeredWindowAttributes(hwnd, 0L, (BYTE)t, LWA_ALPHA);    // 设置窗口的透明度为100
    //DwmEnableBlurBehindWindow(getwin, &pd);
    //EnumChildWindows(hwnd, EnumCallback, lParam);                // 遍历窗口的子窗口

    return TRUE;
}

typedef struct
{
    HWND hwndWindow; // 窗口句柄
    DWORD dwProcessID; // 进程ID
}EnumWindowsArg;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    //判断窗口是否可见
    if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE)
    {
        m_hwndFind[m_num] = hWnd;
        m_num++;
    }

    return true;
}

///< 枚举窗口回调函数
BOOL CALLBACK EnumWindowsProcs(HWND hwnd, LPARAM lParam)
{
    EnumWindowsArg* pArg = (EnumWindowsArg*)lParam;
    DWORD dwProcessID = 0;
    // 通过窗口句柄取得进程ID
    ::GetWindowThreadProcessId(hwnd, &dwProcessID);
    if (dwProcessID == pArg->dwProcessID)
    {
        pArg->hwndWindow = hwnd;
        // 找到了返回FALSE
        return FALSE;
    }
    // 没找到，继续找，返回TRUE
    return TRUE;
}
///< 通过进程ID获取窗口句柄
HWND GetWindowHwndByPID(DWORD dwProcessID)
{
    HWND hwndRet = NULL;
    EnumWindowsArg ewa;
    ewa.dwProcessID = dwProcessID;
    ewa.hwndWindow = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
    if (ewa.hwndWindow)
    {
        hwndRet = ewa.hwndWindow;
    }
    return hwndRet;
}
bool GetHwndByPid(DWORD ProcessId, HWND** hWnd, int* iLength)
{
    bool bRet = false;
    *iLength = 0;
    WindowInfo WinInfo1[255];
    DWORD aa = EnumWindowInfo(WinInfo1);
    for (int i = 0; i < aa; i++)
    {
        if (WinInfo1[i].dwProcessId == ProcessId)
        {
            bRet = true;
            (*hWnd)[(*iLength)] = WinInfo1[i].hwnd1;
            (*iLength)++;

            if (NULL == ::GetWindowLong(WinInfo1[i].hwnd1, GWL_HWNDPARENT))
            {
                //return WinInfo1[i].hwnd1;
            }
        }
    }

    return bRet;
}

DWORD EnumWindowInfo(WindowInfo WInfo[])
{
    WindowInfo Winpro;
    m_num = 0;
    ::EnumWindows(EnumWindowsProc, NULL);

    int i;
    for (i = 0; i < m_num; i++)
    {
        Winpro.hwnd1 = m_hwndFind[i];

        ::GetWindowText(m_hwndFind[i], (LPWSTR)Winpro.WindowText, 128);
        ::GetClassName(m_hwndFind[i], (LPWSTR)Winpro.ClassName, MAX_PATH - 1);

        Winpro.dwThreadId = ::GetWindowThreadProcessId(m_hwndFind[i], &Winpro.dwProcessId);
        WInfo[i] = Winpro;
    }

    return i;
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

void transparentAppByPid(int pid,int t)
{
    try {
        HWND* hWnd = new HWND[1];
        int iLength = 0;
        GetHwndByPid(pid, &hWnd, &iLength);
        //HWND getwin =FindWindowA(NULL, "C:\\Users\\QDebug\\Desktop\\cmmd.txt - Notepad++");
        //GetTopWindow(NULL);
        //FindWindowA(NULL, "火狐主页 - Mozilla Firefox");
        //cout << getwin << endl;
        if (hWnd[0] ==nullptr)
        {
            throw 100;
        }
        EnumCallback(hWnd[0], 0L,t);
    }
    catch (int a) {
        qDebug() << a << endl;
    }
}

void MainWindow::on_exit_clicked()
{
    exit(1);
}

void MainWindow::initModel()
{
    processList =new QStandardItemModel();

}

void MainWindow::initData()
{
    if(this->strListl.size()>0)
    {
        strListl.clear();
    }
   ItemModel =new QStandardItemModel(this);
         HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
            if (INVALID_HANDLE_VALUE == hSnapshot)
            {
                return ;
            }
            PROCESSENTRY32 pi;
            pi.dwSize = sizeof(PROCESSENTRY32); //第一次使用必须初始化成员
            BOOL bRet = Process32First(hSnapshot,&pi);
            while (bRet)
            {
                /*
                循环遍历添加自己的额外代码
                */
               // printf("进程ID = %d ,进程路径 = %s\r\n",pi.th32ProcessID,pi.szExeFile);
                   QString str = QString::fromWCharArray(pi.szExeFile);

                this->strListl.append( str);
                bRet = Process32Next(hSnapshot,&pi);
            }

         int count= strListl.size();
         for(int i=0;i<count;i++)
         {
             QString string =static_cast<QString>(strListl.at(i));
             QStandardItem *item =new QStandardItem(string);
             ItemModel->appendRow(item);
         }



         ui->listView->setModel(ItemModel);
           ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void MainWindow::sendListener()
{

}

void MainWindow::on_TransparentBtn_clicked()
{
    QString edPid=ui->EdPid->text();
    QString edLevel=ui->EdLevel->text();
    QString processName=ui->EdProcessName->text();
    int level=0;
    int pid=0;
    if(edLevel.length()>0)
        level=edLevel.toInt();
    if(level<=0)
        level=50;
    else if(level>255)
        level=255;

    if(edPid.length()>1)
    {
        pid=edPid.toInt();
        transparentAppByPid(pid,level);
        return;
    }
    if(processName.length()<2)
    {
        QMessageBox::warning(this,tr("警告"),tr("<p><span style='color: #FFFFFF;'>pid和processName必填一项</span></p>"),QMessageBox::Yes);

        return;
    }
    QString strcmmd=begin+processName+end;
    qDebug()<<strcmmd.toUtf8()<<endl;
    WinExec(strcmmd.toUtf8(), SW_HIDE);
    Sleep(500);
    QString fileName = "./1.ini";
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    if(t.length()<10)
    {
        QMessageBox::information(this,tr("提示"),tr("<p><span style='color: #FFFFFF;'>没有找到进程</span></p>"),QMessageBox::Yes);
        return;
    }
    QString contest=QString(t);
    file.close();
    QStringList splitList= contest.split('\n');
    QList<QString>::Iterator it = splitList.begin(),itend = splitList.end();
    int i=0;
    for (;it != itend; it++,i++){
        if(it->length()>5)
        {
            QStringList list=it->split("                ");
            QString dd=list.begin()->toUtf8();
            QList<QString>::Iterator its = list.begin(),itsend = list.end();

            //           qDebug()<<<<endl;
            int j=0;
            for (;its != itsend; its++,j++){
                if(j==1)
                {
                    QString pidc=its->toUtf8().trimmed();
                    QStringList newPid=pidc.split(' ');
                    //qDebug()<<newPid.begin()->toUtf8()<<endl;
                    int pid=newPid.begin()->toInt();
                    qDebug() << pid << endl;
                    transparentAppByPid(pid,level);
                }

            }



        }

    }

}


void MainWindow::on_min_clicked()
{
    this->showMinimized();
}

void MainWindow::createSystemTray()
{
    if(m_isShowSystemTray){
        return;
    }
    m_isShowSystemTray=true;
    m_pTrayMennu = new QMenu(this);
    m_pSystemTray = new QSystemTrayIcon(this);
    m_pTrayMennu->setStyleSheet("QMenu{\n"
                                "  background:rgba(255,255,255,1);"
                                "  border:none;"
                                "}"
                                "QMenu::item{"
                                "  padding:11px 32px;"
                                "  color:rgba(51,51,51,1);"
                                "  font-size:12px;"
                                "}"
                                "QMenu::item:hover{"
                                "  background-color:#409CE1;"
                                "}"
                                "QMenu::item:selected{"
                                "  background-color:#409CE1;"
                                "}");
    //创建菜单项
    m_pActionShow = new QAction(tr("Show pet"), this);
  //  m_pActionShow->set
    m_pActionHide = new QAction(tr("Hide pet"), this);
    m_pActionModel = new QAction(tr("Model selecte"), this);
    m_pActionSetting = new QAction(tr("Setting"), this);
    m_pActionQuit = new QAction(tr("Exit"), this);

    //添加菜单项
    m_pTrayMennu->addAction(m_pActionShow);
    m_pTrayMennu->addAction(m_pActionHide);
    m_pTrayMennu->addAction(m_pActionModel);
    m_pTrayMennu->addAction(m_pActionSetting);
    m_pTrayMennu->addSeparator();
    m_pTrayMennu->addAction(m_pActionQuit);

    //为系统托盘设置菜单为m_pTrayMennu
    m_pSystemTray->setContextMenu(m_pTrayMennu);
    m_pSystemTray->setIcon(QIcon(":/res/home-ico.ico"));
    connect(m_pActionShow,&QAction::triggered,m_pTrayMennu,[=](){
        this->showNormal();
    });


    connect(m_pActionQuit,&QAction::triggered,m_pTrayMennu,[=](){
        exit(0);
    });
    connect(m_pSystemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
    m_pSystemTray->show();
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    if(this->strListl.size()>0)
    {
        ui->EdProcessName->setText(this->strListl.at(index.row()));
    }
}


void MainWindow::onActivated(QSystemTrayIcon::ActivationReason action){
    qDebug()<<action;
       if(action==QSystemTrayIcon::ActivationReason::Unknown){
           qDebug()<<"未知点击";
       }
       else if(action==QSystemTrayIcon::ActivationReason::Context || action==QSystemTrayIcon::ActivationReason::Trigger){
           qDebug()<<"鼠标单击";
       }
       else if(action==QSystemTrayIcon::ActivationReason::DoubleClick){
           this->showNormal();
       }
       else if(action==QSystemTrayIcon::ActivationReason::MiddleClick){
           qDebug()<<"鼠标中间单击";
       }else{
           return;
       }
}
