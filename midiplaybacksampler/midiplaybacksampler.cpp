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
    m_ui.lineEditPlayRecName->setText("CMidiPlaybackSampler");
 
    QComboBox *comboSrcChan;
    QComboBox *comboSources;
    
    fillupSampleCombobox(m_ui.comboSampler);
    
    QComboBox *comboInstrument;
    fillupInstrumentCombobox(m_ui.comboInstrument);
    
    createAudioGraph(); // create audio graph that uses Apple sampler and output
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

OSStatus CMidiPlaybackSampler::createAudioGraph(void)
{
    OSStatus res = noErr;
    
    // create a new AUGraph
    if( (res = NewAUGraph(&m_audiograph)) != noErr) return res;
    
    // create AUSampler
    AudioComponentDescription samplerdesc;
    samplerdesc.componentType = kAudioUnitType_MusicDevice;
    samplerdesc.componentSubType = kAudioUnitSubType_Sampler;
    samplerdesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AUNode samplerNode;
    if( (res = AUGraphAddNode(m_audiograph, &samplerdesc, &samplerNode)) != noErr) return res;;
        
    // create output node
    AudioComponentDescription outputdesc;
    outputdesc.componentType = kAudioUnitType_Output;
    outputdesc.componentSubType = kAudioUnitSubType_DefaultOutput;
    outputdesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AUNode outputnode;
    if( (res = AUGraphAddNode(m_audiograph, &outputdesc, &outputnode)) != noErr) return res;
    
    // open the graph and get unit references
    if( (res = AUGraphOpen(m_audiograph) ) != noErr) return res;
    
    if( (res = AUGraphNodeInfo(m_audiograph, outputnode, nullptr, &m_samplerunit)) != noErr) return res;
       
    AudioUnit outputUnit;
    if( (res = AUGraphNodeInfo(m_audiograph, samplerNode, nullptr, &outputUnit)) != noErr) return res;
       
    // connect sampler to output
    if( (res = AUGraphConnectNodeInput(m_audiograph, samplerNode, 0, outputnode, 0)) != noErr) return res;
    
    if( (res = AUGraphInitialize(m_audiograph)) != noErr) return res;
    
    if( (res = AUGraphStart(m_audiograph)) != noErr) return res;
       
    return res;
};
