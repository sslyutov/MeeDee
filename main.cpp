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
    //ItemCount deviceCount = MIDIGetNumberOfDevices();
    
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
    
    centralWidget->setLayout(layout);
    
    mainWindow.setCentralWidget(centralWidget);
    
    mainWindow.show();
    
    app.exec();
};
