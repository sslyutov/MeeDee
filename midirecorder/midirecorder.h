
/*!\brief midirecorder controls collection of recorded midi messages
 * \file midirecorder.h
 * \date January 12, 2025
 * \author Sergey Slyutov */

#ifndef _MIDIRECORDER_H

#define _MIDIRECORDER_H

#include "utils.h"

#include <QWidget>

#include <QVector>

#include "IMidiRecorder.h"

/*!\class CMidiRecorder collects midi messages with the respect of sources configuration
 * the source configuration is determined by data member m_srcchan.
 * is m_srcchan is empty then all messages are collected.
 */
class CMidiRecorder:
    public QWidget
, public IMidiRecorder
{
    //Q_OBJECT
    
    Q_INTERFACES(IMidiRecorder)
        
public:
    
    CMidiRecorder();
    
    virtual ~CMidiRecorder(){};
    
    /*!\fn addsource  adds  a source desciption to the collection
     * \param src index identifies the source endpoint.
     * \param chan index of the source. valid values between 1 and 16.
     * \param clearprev indicates whether to clean previous content of m_srcchan.
     *  */
    //void addsource(int src, int chan, bool clearprev = false){
    //
    //    if(clearprev)
    //
    //        m_srcchans.clear();
    //
    //    m_srcchans.push_back({src, chan});
    //}

    void setsource(int src, int chan){};
    
    void start(void){};
    
    void stop(void){};
    
    void pause(void){};
    
private:
    
    //QVector<ns_midispecs::srcchan> m_srcchans;
    
    int m_srcid;
    
    int m_chan;
    
    int m_state;
};

#endif // _MIDIRECORDER_H
