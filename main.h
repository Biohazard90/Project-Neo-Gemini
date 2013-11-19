#ifndef MAIN_H
#define MAIN_H

#include <QObject>

class MainCleanup : public QObject
{
Q_OBJECT
public:
    explicit MainCleanup(QWidget *parent = 0);
public slots:
    void OnShutdown();
};

extern void ResizeView(int w, int h);

#endif // MAIN_H
