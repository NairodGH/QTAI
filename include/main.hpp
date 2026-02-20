#pragma once

#include <QApplication>
#include <QButtonGroup>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMutex>
#include <QProgressBar>
#include <QPushButton>
#include <QQmlContext>
#include <QQuickWidget>
#include <QStackedWidget>
#include <QTabWidget>
#include <QThread>
#include <QVBoxLayout>
#include <QWaitCondition>
#include <QtWidgets>
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <vector>

class ETL;
class KNN;
class QTAI;
class DatasetInfo;
class Data;
class Algorithm;

using AlgorithmArray = QList<Algorithm*>;
Q_DECLARE_METATYPE(AlgorithmArray)