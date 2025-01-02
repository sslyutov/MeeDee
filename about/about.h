/*!\brief about dialog for the application
 *  \file about.h
 *  \date Decemebr 25, 2024
 *  \author Sergey Slyutov
 */

#ifndef _ABOUT_H

#define _ABOUT_H

#include "ui_about.h"

#include <CoreMIDI/CoreMIDI.h>

#include <CoreFoundation/CoreFoundation.h>

class CAbout: public QWidget
{
    Q_OBJECT
    
public:
    
    CAbout();
    
    virtual ~CAbout();
    
    void refreshMidiSources(void);

    void refreshMIDIDestinations(void);
    
    void refreshExternalDevices(void);
    
    void refreshMidiDevices(void);
    
    void refreshMidiDevsDetails(QString devname);
    
    private:
    
    Ui_AboutForm m_ui;
};

#endif //_ABOUT_H
