/*!\brief generic description of MIDI destinations
* \file midioutputport.h
* \date January 7. 2025
* \author Sergey Slyutov */

#ifndef _MIDIOUTPUTPORT_H

#define _MIDIOUTPUTPORT_H

#include <QWidget>

#include <CoreFoundation/CoreFoundation.h>

#include <CoreMIDI/CoreMIDI.h>

#include <QFuture>

class CMIDIOutputPort: public QWidget
{
Q_OBJECT

public:

    CMIDIOutputPort();

    virtual ~CMIDIOutputPort();
    
    static CMIDIOutputPort* This(){
        
        if(m_this == NULL)
            
            new CMIDIOutputPort();
        
        return m_this;
    }
    
    static CMIDIOutputPort * m_this;
    
private:
    
    static MIDIClientRef m_client;
    
    static MIDIPortRef m_outputPort;
    
    static void reignite(void);
    
    static void cdMidiDestinationsWatcher(void);
        
    QFuture<void> m_fwatcher;
    
    static bool m_bwatcher;
      
    static ItemCount m_prevDstCount;
};

#endif // _MIDIOUTPUTPORT

