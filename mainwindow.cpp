#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"

#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = (QMouseEvent*)e;

        pGlobals->mouse_x = mouseEvent->x();
        pGlobals->mouse_y = mouseEvent->y();
    }

    return QMainWindow::eventFilter(o, e);
}
