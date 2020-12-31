#include "task.h"


Task::Task()
{

}

void Task::run()
{
    while(1)
    {
        Sleep(5000);
        emit isStartInit();
    }

}
