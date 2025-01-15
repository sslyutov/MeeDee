#include <QApplication>

#include <CoreMIDI/CoreMIDI.h>

#include "midiinputport.h"

#include "midioutputport.h"

#include "lighthouse.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
     
    QApplication::setOrganizationName("Inetgrad"); 
    
    QApplication::setApplicationName("MeeDee");
    
    QApplication::setApplicationVersion(APP_VERSION);
    
    CMIDIInputPort::This(); // instanciate midiinputport;
    
    CMIDIOutputPort::This(); // instanciate midioutput;
    
    CLighthouse::This(); // instanciate lighhouse
    
    CMainWindow mainwindow;
    
    mainwindow.show();
    
    app.exec();
    
    CMIDIInputPort::This()->terminate();
};
