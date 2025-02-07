/*!\brief combination of midi source and audio sampler
 * \file midiplaybacksampler.cpp
 * \author Sergey Slyutov
 * \date January 25, 2025
 */

#include "midiplaybacksampler.h"

#include <QTreeWidget>

void addMidiPlaybackSampler(QTreeWidget* ptreewidget)
{
    QTreeWidgetItem * item = new QTreeWidgetItem(ptreewidget);
    
    ptreewidget->setItemWidget(item, 0, new CMidiPlaybackSampler());
};

CMidiPlaybackSampler::CMidiPlaybackSampler(QString name):
QWidget()
{
    m_ui.setupUi(this);
    m_ui.
    
    lineEditPlayRecName;
 
    QComboBox *comboSrcChan;
    QComboBox *comboSources;
    
    QComboBox *comboSampler;
    QComboBox *comboInstrument;
    
};

CMidiPlaybackSampler::~CMidiPlaybackSampler()
{};

void CMidiPlaybackSampler::startRecording(void)
{};

void CMidiPlaybackSampler::stopRecording(void)
{};

void CMidiPlaybackSampler::startPlayback(void)
{};

void CMidiPlaybackSampler::mutePlayback(void)
{};
