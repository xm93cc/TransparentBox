#ifndef TASK_H
#define TASK_H

#include <QObject>
#include<QThread>
#include<windows.h>

class Task : public QThread
{
    Q_OBJECT
public:
    explicit Task();
protected:
    void run();

signals:
    void isStartInit();

public slots:
};
#endif // TASK_H
