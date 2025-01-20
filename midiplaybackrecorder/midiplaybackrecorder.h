/*!\brief combination of midi player and midi recorder.
* \file midiplaybackrecorder.h
* \date January 13, 2025
* \author Sergey Slyutov */

#ifndef _MIDIPLAYBACKRECORDER_H

#define _MIDIPLAYBACKRECORDER_H

#include "midiPlayer.h"

#include "midirecorder.h"

#include <QWidget>

#include <QDockWidget>

#include "ui_midiplaybackrecorder.h"

#define PLAYBACKRECORDERDOC "PlaybackRecorderDock" // object name for playbackrecorder QDockWidget

QDockWidget * createPlaybackRecorderDoc(void);


class CMidiPlaybackRecorder: public QWidget
{
Q_OBJECT

public:

    CMidiPlaybackRecorder(QString name = {"MidiPlaybackRecorder"});

    virtual ~CMidiPlaybackRecorder();
    
    void startRecording(void); // start recording of midi messages
    
    void stopRecording(void); // stops recording of midi messages
    
    void startPlayback(void); // starts playback from the begining
    
    void stopPlayback(void); // stops the playback
                    
    void mutePlayback(void); // simulates playback by going through the recorder midi messages without output to destination

private:
    
    QString m_name;
    
    Ui_MidiPlaybackRecorderForm m_ui;
};

#endif // _MIDIPLAYBACKRECORDER_H
