/*!\brief combination of midi player and midi recorder.
* \file midiplaybackrecorder.h
* \date January 13, 2025
* \author Sergey Slyutov */

#ifndef _MIDIPLAYBACKRECORDER_H

#define _MIDIPLAYBACKRECORDER_H

#include "midiPlayer.h"

#include "midirecorder.h"

#include <QWidget>

class CMidiPlaybackRecorder: public QWidget
{
Q_OBJECT


public:

    CMidiPlaybackRecorder();

    virtual ~CMidiPlaybackRecorder();

};

#endif // _MIDIPLAYBACKRECORDER_H