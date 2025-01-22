/* !\brief interface for midi recorder
* \file imidirecorder.h
* \date 1/21/25
* \author Sergey Slyutov */

#ifndef _IMIDIRECORDER_H

#define _IMIDIRECORDER_H

#include <QObject>

class IMidiRecorder: public QObject
{
    Q_OBJECT
    
    public:
        
        virtual void setsource(int src, int chan) = 0;
        
        virtual void start(void) = 0;
        
        virtual void stop(void) = 0;
        
        virtual void pause(void) = 0;
};

#define IMidiRecorder_iid "com.example.IMidiRecorder"

Q_DECLARE_INTERFACE(IMidiRecorder, IMidiRecorder_iid)

#endif // _IMIDIRECORDER_H
