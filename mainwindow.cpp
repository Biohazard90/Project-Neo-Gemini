#include "mainwindow.h"
#include "rootview.h"
#include "globals.h"
#include "statistics.h"

#include <QMouseEvent>
#include <QThread>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    switch (e->type())
    {
    case QEvent::MouseMove:
        {
            QMouseEvent *mouseEvent = (QMouseEvent*)e;

            pGlobals->mouse_x = mouseEvent->x();
            pGlobals->mouse_y = mouseEvent->y();
        }
        break;
    case QEvent::Quit:
        {
            RootView *rootView = RootView::GetActiveRootView();

            if (rootView != nullptr
                    && rootView->GetGame() != nullptr)
            {
                Statistics::GetInstance()->SetUploadBlocking(true);

                rootView->onAbortGame();
            }
        }
        break;
    }

    return QMainWindow::eventFilter(o, e);
}
