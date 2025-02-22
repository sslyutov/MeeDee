
/* \brief midirecorder controls collection of recorded midi messages
 * \file midirecorder.h
 * \date January 12, 2025
 * \author Sergey Slyutov */

#ifndef _MIDIRECORDER_H

#define _MIDIRECORDER_H

#include "utils.h"

#include <QWidget>

#include <QVector>

#include "imidirecorder.h"

/* \class CMidiRecorder collects midi messages with the respect of sources configuration
 * the source configuration is determined by data member m_srcchan.
 * is m_srcchan is empty then all messages are collected. */
class CMidiRecorder:

    //public QWidget

     public IMidiRecorder

{
    Q_OBJECT
    
    //Q_INTERFACES(IMidiRecorder)
        
public:
    
    CMidiRecorder();
    
    virtual ~CMidiRecorder(){};

    /*\fn setsource assign source parameters
     * \param src id of source device. -1 means midi events from all source devices are accepted
     * \param chan index of the source channel. -1 means midi events all channels are accepted */
    void setsource(int src = -1, int chan = -1)
    {
        m_srcid = src;
        m_chan = chan;
    };
    
    void start(void){
        m_state = MidiRecorderState::recording;
    };
    
    void stop(void){
        m_state = MidiRecorderState::stopped;
    };
    
    void pause(void){
        m_state = MidiRecorderState::paused;
    };
    
private:
    
    //QVector<ns_midispecs::srcchan> m_srcchans;
    
    int m_srcid;
    
    int m_chan;
    
    int m_state;
};

#endif // _MIDIRECORDER_H
