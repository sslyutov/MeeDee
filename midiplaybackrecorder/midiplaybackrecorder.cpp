/*!\brief combination of midi player and midi recorder.
* \file midiplaybackrecorder.h
* \date January 13, 2025
* \author Sergey Slyutov */

#include "midiplaybackrecorder.h"

#include <QTreeWidget>

QDockWidget * createPlaybackRecorderDoc(void)
{
    QDockWidget * playbackrecorderdoc = new QDockWidget();
    playbackrecorderdoc->setObjectName(PLAYBACKRECORDERDOC);
    QTreeWidget * pinputchantree = new QTreeWidget();
    pinputchantree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    playbackrecorderdoc->setWidget(pinputchantree);
    return playbackrecorderdoc;
};

CMidiPlaybackRecorder::CMidiPlaybackRecorder(QString name):
    m_name(name)
{
};

CMidiPlaybackRecorder:: ~CMidiPlaybackRecorder()
{};

void CMidiPlaybackRecorder::startRecording(void)
{};

void CMidiPlaybackRecorder::stopRecording(void)
{};

void CMidiPlaybackRecorder::startPlayback(void)
{};

void CMidiPlaybackRecorder::stopPlayback(void)
{};


