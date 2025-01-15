/*!\brief instance of the call lighthouse provides notifications upon receiving midi messages
* \date January 5, 2025
* \file lighthouse.h
* \author Sergey Slyutov */

#ifndef _LIGHTHOUSE_H

#define _LIGHTHOUSE_H

#include <QWidget>

#include <CoreFoundation/CoreFoundation.h>

#include <CoreMIDI/CoreMIDI.h>

class CLighthouse: public QWidget
{
    Q_OBJECT
    
private: 

    CLighthouse():QWidget()
    {
        m_this = this;
    };
    
    ~CLighthouse(){m_this = NULL;};
    
    inline static CLighthouse * m_this = NULL;
  
public:
    
    static CLighthouse* This(){
        
        if(m_this == NULL){
            
            new CLighthouse();
        }
        return m_this;
    }
    
    signals:
    
        void heartbit_0xfe(uint64_t timestampnanoseconds);
    
        void reignite(void);
    
        void midiEventList (const MIDIEventList *evtlist, void * __nullable srcConnRefCon);
};

//Q_DECLARE_METATYPE(MIDIEventList)

#endif // _LIGHTHOUSE_H
