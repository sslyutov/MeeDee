/*!\brief midirecorder controls collection of midi messages
 * \file midirecorder.cpp
 * \date Janiary 12, 2025
 * \author Sergey Slyutov  */

#include "midirecorder.h"

#include "lighthouse.h"

#include <QWidget>

struct MIDIContext {
    int id;
    const char* name;
};

CMidiRecorder::CMidiRecorder():
    //QWidget()
     IMidiRecorder()
    , m_state(MidiRecorderState::stopped)
    , m_srcid(0)
    , m_chan(0)
{

    bool bcon = QObject::connect(CLighthouse::This(), &CLighthouse::midiEventList, [](const MIDIEventList *evtlist, void * __nullable srcConnRefCon){

        if(srcConnRefCon){
            
            MIDIContext * midicontext = static_cast<MIDIContext*>(srcConnRefCon);
            
            qDebug() << "srcConnRefCon id:" << midicontext->id << " name:" << midicontext->name;
        }

    });

    qDebug() << bcon;
}

