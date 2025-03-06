
#ifndef PROJECTBASE_WORKER_HPP
#define PROJECTBASE_WORKER_HPP

#include "../test/printing.hpp"

static volatile bool finishedA = false;
static volatile bool finishedB = false;
static volatile bool finishedC = false;
static volatile bool finishedD = false;

 void workerBodyA(void* arg);

 void workerBodyB(void* arg);

 void workerBodyC(void* arg);

 void workerBodyD(void* arg);

#endif //PROJECTBASE_WORKER_HPP
