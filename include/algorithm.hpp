#pragma once

#include "main.hpp"

class Algorithm : public QThread {
    Q_OBJECT

public:
    explicit Algorithm(QObject* parent = nullptr)
        : QThread(parent)
    {
    }

    void forceWake()
    {
        QMutexLocker locker(&pauseMutex);
        paused = false;
        pauseCondition.wakeAll();
    }

protected:
    bool paused = false;
    bool skip = false;
    QMutex pauseMutex;
    QWaitCondition pauseCondition;
};