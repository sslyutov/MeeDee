/*!\brief combination of midi source and audio sampler
 * \file midiplaybacksampler.h
 * \author Sergey Slyutov
 * \date January 25, 2025
 */
    
#ifndef _MIDIPLAYBACKSAMPLER_H

#define _MIDIPLAYBACKSAMPLER_H

#include "midirecorder.h"

#include <QWidget>

#include <QTreeWidget>

#include "ui_midiplaybacksampler.h"

class CMidiPlaybackSampler: public QWidget
{
   Q_OBJECT
    
public:
    
    CMidiPlaybackSampler(QString name = {"MidiPlaybackSampler"} );
    
    virtual ~CMidiPlaybackSampler();
    
    void startRecording(void);
    
    void stopRecording(void);
    
    void startPlayback(void);
    
    void mutePlayback(void);
    
private:
    
    QString m_name; // nameidentifies the playbacksampler
    
    Ui_MidiPlaybackSamplerForm m_ui;
    
    CMidiRecorder   m_midirecorder;
    
};

void addMidiPlaybackSampler(QTreeWidget* ptreewidget);


#endif // _MIDIPLAYBACKSAMPLER_H
