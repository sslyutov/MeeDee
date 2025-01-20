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

#include "midiplaybackrecorder.h"

#include "lighthouse.h"

CMainWindow::CMainWindow(): QMainWindow()
{
    setWindowTitle(QApplication::tr("mainwindow creation"));
    
    QWidget * pcentralWidget = new QWidget(); 
    setCentralWidget(pcentralWidget);
    
    addDockWidget(Qt::LeftDockWidgetArea, createPlaybackRecorderDoc());
    
    QMenu * aboutmenu = menuBar()->addMenu(QApplication::tr("About"));
    QToolBar * maintoolbar = new QToolBar(this);
    
    QAction * pactabout = aboutmenu->addAction(QIcon(":/meedeeabout.png"), QApplication::tr("About MeeDee"));
    maintoolbar->addAction(pactabout);
    QObject::connect(pactabout, &QAction::triggered,[=](){
        (new CAbout())->show();
    });
    
    QMenu * creationMenu = menuBar()->addMenu(QApplication::tr("Creation"));
    QToolBar * creationtoolbar = new QToolBar(this);
    QAction * pnewaction = creationMenu->addAction(QApplication::tr("New..."));
    QAction * popenaction = creationMenu->addAction(QApplication::tr("Open..."));
    QAction * psaveaction = creationMenu->addAction(QApplication::tr("Save"));
    QAction * psaveasaction = creationMenu->addAction(QApplication::tr("Save As..."));
    creationtoolbar->addAction(pnewaction);
    creationtoolbar->addAction(popenaction);
    creationtoolbar->addAction(psaveaction);
    creationtoolbar->addAction(psaveasaction);
    QObject::connect(pnewaction, &QAction::triggered, [=](){ (new CMainWindow())->show();});
    QObject::connect(popenaction, &QAction::triggered, [=](){});
    QObject::connect(psaveaction, &QAction::triggered, [=](){});
    QObject::connect(psaveasaction, &QAction::triggered, [=](){});
        
    QMenu * viewMenu = menuBar()->addMenu(QApplication::tr("View"));
    QToolBar * viewtoolbar = new QToolBar(this);
    QAction * playbackRecorderAction = viewMenu->addAction(QIcon(":/playbackrecorder.png"), QApplication::tr("Playback-Recorder"));
    playbackRecorderAction->setCheckable(true);
    playbackRecorderAction->setChecked(findChild<QDockWidget*>(PLAYBACKRECORDERDOC));
    viewtoolbar->addAction(playbackRecorderAction);
    QObject::connect(playbackRecorderAction, &QAction::triggered, [=](bool checked){
        QDockWidget * pdock = this->findChild<QDockWidget*>(PLAYBACKRECORDERDOC);
        if(pdock){
            pdock->setVisible(checked);
        }else{
            addDockWidget(Qt::LeftDockWidgetArea, createPlaybackRecorderDoc());
        }
    });
    
    // control menu and toolbar
    QMenu* recordplayMenu = menuBar()->addMenu(QApplication::tr("Control"));
    QToolBar * recordplaytoolbar = new QToolBar(this);
    QAction* pstartrecordaction = recordplayMenu->addAction(QIcon(":/startrecording"), QApplication::tr("Start &record"));
    QAction* pstoprecordaction = recordplayMenu->addAction(QIcon(":/stoprecording"), QApplication::tr("&Stop record"));
    QAction* pstartplayaction = recordplayMenu->addAction(QIcon(":/startplayback"), QApplication::tr("Start &playback"));
    QAction* pstopplayaction = recordplayMenu->addAction(QApplication::tr("S&top playback"));
    recordplaytoolbar->addAction(pstartrecordaction);
    recordplaytoolbar->addAction(pstoprecordaction);
    recordplaytoolbar->addAction(pstartplayaction);
    recordplaytoolbar->addAction(pstopplayaction);
    QObject::connect(pstartrecordaction,&QAction::triggered, [=](){CLighthouse::This()->emit CLighthouse::startRecording(); });
    QObject::connect(pstoprecordaction, &QAction::triggered, [=](){CLighthouse::This()->emit CLighthouse::stopRecording();  });
    QObject::connect(pstartplayaction,  &QAction::triggered, [=](){CLighthouse::This()->emit CLighthouse::startPlayback();  });
    QObject::connect(pstopplayaction,   &QAction::triggered, [=](){CLighthouse::This()->emit CLighthouse::stopPlayback();   });
    
    addToolBar(creationtoolbar);
    addToolBar(viewtoolbar);
    addToolBar(maintoolbar);
    addToolBar(recordplaytoolbar);
    
    
    QDockWidget * ptracksdoc = new QDockWidget();
    ptracksdoc->setFeatures(ptracksdoc->features() & ~QDockWidget::DockWidgetClosable);
    QWidget * ptrackswidget = new QWidget();
    ptrackswidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ptracksdoc->setWidget(ptrackswidget);
    addDockWidget(Qt::RightDockWidgetArea, ptracksdoc);
        
    QDockWidget * pnotesdoc = new QDockWidget();
    pnotesdoc->setFeatures(pnotesdoc->features() & ~QDockWidget::DockWidgetClosable);

    QWidget * pnoteswidget = new QWidget();
    
    pnoteswidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pnoteswidget->setMinimumHeight(150);
    pnotesdoc->setWidget(pnoteswidget);
    addDockWidget(Qt::BottomDockWidgetArea, pnotesdoc);

    resize(800, 600);
    showMaximized();
};

