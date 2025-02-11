/*!\brief combination of midi player and midi recorder.
* \file midiplaybackrecorder.h
* \date January 13, 2025
* \author Sergey Slyutov */

#include "midiplaybackrecorder.h"

#include <QTreeWidget>

#include <QMenu>

#include <QMessageBox>

#include <QApplication>

#include <QString>

#include <CoreFoundation/CoreFoundation.h>

#include <CoreMIDI/CoreMIDI.h>

#include "lighthouse.h"

#include "midiplaybacksampler.h"

void addMidiPlaybackRecorder(QTreeWidget * ptreewidget)
{
    QTreeWidgetItem * item = new QTreeWidgetItem(ptreewidget);
 
    ptreewidget->setItemWidget(item,0, new CMidiPlaybackRecorder());
};

CMidiPlaybackRecorder::CMidiPlaybackRecorder(QString name):
    m_name(name)
{
    m_ui.setupUi(this);
    
    m_ui.lineEditPlayRecName->setText(m_name);
    
    QObject::connect(CLighthouse::This(), &CLighthouse::startRecording, this, &CMidiPlaybackRecorder::startRecording);
    
    QObject::connect(CLighthouse::This(), &CLighthouse::stopRecording, this, &CMidiPlaybackRecorder::stopRecording);
    
    QObject::connect(CLighthouse::This(), &CLighthouse::startPlayback,  [](){
        QMessageBox::information(NULL, qApp->applicationDisplayName(),"handle start playback", QMessageBox::Ok);
    });
    
    QObject::connect(CLighthouse::This(), &CLighthouse::stopPlayback,  [](){
        QMessageBox::information(NULL, qApp->applicationDisplayName(),"handle stop playback", QMessageBox::Ok);
    });
    
    fillupMidiSrcDstComboBoxes(m_ui.comboSources, m_ui.comboSrcChan, m_ui.comboDestinations, m_ui.comboDstChan);
};

CMidiPlaybackRecorder:: ~CMidiPlaybackRecorder()
{};

void CMidiPlaybackRecorder::startRecording(void)
{
    m_midirecorder.start();
};

void CMidiPlaybackRecorder::stopRecording(void)
{
    m_midirecorder.stop();
};

void CMidiPlaybackRecorder::startPlayback(void)
{};

void CMidiPlaybackRecorder::stopPlayback(void)
{};


