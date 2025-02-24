/*!\brief description of mainwindow/creation class
* \file mainwindow.cpp
* \date January 5, 2024
* \author Sergey Slyutov */

#include "mainwindow.h"

#include "about.h"

#include <QMenu>

#include <QMenuBar>

#include <QToolBar>

#include <QAction>

#include <QDockWidget>

#include <QTreeWidget>

CMainWindow::CMainWindow(): QMainWindow()
{
    setWindowTitle("mainwindow creation");
    
    QWidget * pcentralWidget = new QWidget();
    setCentralWidget(pcentralWidget);
    
    QMenu * aboutmenu = menuBar()->addMenu("About");//new QMenu(this);
    QToolBar * maintoolbar = new QToolBar(this);
    
    QAction * pactabout = aboutmenu->addAction("About MeeDee");
    maintoolbar->addAction(pactabout);
    QObject::connect(pactabout, &QAction::triggered,[=](){
        (new CAbout())->show();
    });
    
    QMenu * creationMenu = menuBar()->addMenu("Creation");//new QMenu(this);
    QToolBar * creationtoolbar = new QToolBar(this);
    QAction * pnewaction = creationMenu->addAction("New...");
    QAction * popenaction = creationMenu->addAction("Open...");
    QAction * psaveaction = creationMenu->addAction("Save");
    QAction * psaveasaction = creationMenu->addAction("Save As...");
    creationtoolbar->addAction(pnewaction);
    creationtoolbar->addAction(popenaction);
    creationtoolbar->addAction(psaveaction);
    creationtoolbar->addAction(psaveasaction);
    QObject::connect(pnewaction, &QAction::triggered, [=](){ (new CMainWindow())->show();});
    QObject::connect(popenaction, &QAction::triggered, [=](){});
    QObject::connect(psaveaction, &QAction::triggered, [=](){});
    QObject::connect(psaveasaction, &QAction::triggered, [=](){});
    
    //menuBar()->addMenu(creationMenu);
    addToolBar(creationtoolbar);
    
    //menuBar()->addMenu(aboutmenu);
    addToolBar(maintoolbar);
    
    QDockWidget * pinputchandoc = new QDockWidget();
    QTreeWidget * pinputchantree = new QTreeWidget();
    pinputchantree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    pinputchandoc->setWidget(pinputchantree);
    addDockWidget(Qt::LeftDockWidgetArea, pinputchandoc);

    QDockWidget * ptracksdoc = new QDockWidget();
    QWidget * ptrackswidget = new QWidget();
    ptrackswidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ptracksdoc->setWidget(ptrackswidget);
    addDockWidget(Qt::RightDockWidgetArea, ptracksdoc);
    
    
    QDockWidget * pnotesdoc = new QDockWidget();
    QWidget * pnoteswidget = new QWidget();
    
    pnoteswidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pnoteswidget->setMinimumHeight(50);
    pnotesdoc->setWidget(pnoteswidget);
    addDockWidget(Qt::BottomDockWidgetArea, pnotesdoc);

};

