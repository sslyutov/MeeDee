#include <QApplication>

#include <QWidget>

#include <QMenu>

#include <QMenuBar>

#include <QMessageBox>

#include <QVBoxLayout>	

#include <QPushButton>

#include <QMainWindow>

#include <CoreMIDI/CoreMIDI.h>

#include "about.h"

int main(int argc, char *argv[])
{
    ItemCount deviceCount = MIDIGetNumberOfDevices();
    
    QApplication app(argc, argv);
     
    QApplication::setOrganizationName("Inetgrad");
    
    QApplication::setApplicationName("MeeDee");
    
    QApplication::setApplicationVersion(APP_VERSION);
    
    QMainWindow mainWindow;
      
    QWidget *centralWidget = new QWidget(&mainWindow);
    
    QMenuBar menubar;
    
    QAction * pactabout = menubar.addAction("About");
    
    QObject::connect(pactabout, &QAction::triggered,[=](){
        
        (new CAbout())->show();
        
    });
    
    mainWindow.setMenuBar(&menubar);
    
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    //QPushButton *button = new QPushButton("About", &mainWindow);
    
    //layout->addWidget(button);
    
    centralWidget->setLayout(layout);
    
    mainWindow.setCentralWidget(centralWidget);
    
    /*
    QObject::connect(button, &QPushButton::clicked,  [=](){
        QString version("MeeDee v.1.0.0");
        QString strmsg = QString("number of MIDI devices %1. \n").arg(
        QMessageBox::information(NULL, qApp->applicationDisplayName(), "MeeDee v 1.0.0");
    });*/
    
    mainWindow.show();
    
    app.exec();
};
