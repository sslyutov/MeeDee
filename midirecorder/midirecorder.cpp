/*!\brief midirecorder controls collection of midi messages
 * \file midirecorder.cpp
 * \date Janiary 12, 2025
 * \author Sergey Slyutov  */

#include "midirecorder.h"

#include "lighthouse.h"

CMidiRecorder::CMidiRecorder(): QWidget()
{
    
    bool bcon = QObject::connect(CLighthouse::This(), &CLighthouse::midiEventList, [](){
        
    });
   
}

