/*!\brief input port
*/

#ifndef _MIDIINPUTPORT_H

#define _MIDIINPUTPORT_H

#include <QWidget>

#include <CoreFoundation/CoreFoundation.h>

#include <CoreMIDI/CoreMIDI.h>

#include <QFuture>

class CMIDIInputPort: public QWidget
{
     Q_OBJECT
     
public:
    
    CMIDIInputPort();
    
    virtual ~CMIDIInputPort();
    
    static CMIDIInputPort* This(){
        if(m_this == NULL)
            new CMIDIInputPort();
        return m_this;
    }
    
    static CMIDIInputPort* m_this;
    
    static void reignite(void);
    
    //static void cbMidiRead(const MIDIPacketList * packetlist, void * readProcRefCon, void * srcConnRefCon );
    
    static void cbmidiRead(const MIDIPacketList *packetList, void *srcConnRefCon);
    
    static void cdMidiSourcesWatcher(void);

    static void terminate(void){m_bwatcher = false;};
    
private:

    static MIDIPortRef     m_inputPort;
    
    static MIDIClientRef   m_midiClient;
    
    QFuture<void>   m_fwatcher;
        
    static bool m_bwatcher;
    
    static ItemCount m_prevSrcCount;
    
};

#endif // _MIDIINPUTPORT_H 
