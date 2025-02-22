/*!\brief combination of midi source and audio sampler
 * \file midiplaybacksampler.cpp
 * \author Sergey Slyutov
 * \date January 25, 2025
 */

#include "midiplaybacksampler.h"

#include <QTreeWidget>

#include <QMenu>

#include <QFileDialog>

#include "utils.h"

#include <QMessageBox>

#include <Qthread>

#include <iostream>

#include "lighthouse.h"

void showContextMenu(CMidiPlaybackSampler * ppbsmlr, const QPoint &pos)
{
    QMenu contextMenu(ppbsmlr->m_ui.comboSampler);
    
    // Add actions to the menu
    QAction *assignsoundfont = contextMenu.addAction("assign sound font");
    
    // Execute the menu and get the selected action
    QAction *selectedAction = contextMenu.exec(ppbsmlr->m_ui.comboSampler->mapToGlobal(pos));
    
    // Handle the selected action
    if (selectedAction == assignsoundfont) {
     
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Select a sound font file", "", "Sound Banks (*.dls *.sf2)");
        
        if( !isSoundFont(filePath) ){
            
            QMessageBox::information(NULL, qApp->applicationDisplayName(), QApplication::tr("The selected file %1 is not a sound font").arg(QFileInfo(filePath).fileName()), QMessageBox::Ok);
        }else{
            ppbsmlr->m_soundFont = filePath;
            
            std::list<SF2PresetHeader> instruments = sf2Instruments(filePath);
            
            ppbsmlr->m_ui.comboInstrument->clear();
            
            for(auto instrument : instruments){
            
                ppbsmlr->m_ui.comboInstrument->addItem(instrument.presetName);
                
                ppbsmlr->m_ui.comboInstrument->setItemData(ppbsmlr->m_ui.comboInstrument->count()-1, QVariant::fromValue<SF2PresetHeader>(instrument));
                
            }
            
        }
    }
};

void addMidiPlaybackSampler(QTreeWidget* ptreewidget)
{
    QTreeWidgetItem * item = new QTreeWidgetItem(ptreewidget);
    
    ptreewidget->setItemWidget(item, 0, new CMidiPlaybackSampler());
    

};

CMidiPlaybackSampler::CMidiPlaybackSampler(QString name):
QWidget()
{
    m_soundFont = "/System/Library/Components/CoreAudio.component/Contents/Resources/gs_instruments.dls"; // default sound font
    
    m_ui.setupUi(this);
    
    m_ui.lineEditPlayRecName->setText("CMidiPlaybackSampler");
    
    fillupMidiSrcDstComboBoxes(m_ui.comboSources, m_ui.comboSrcChan, nullptr, nullptr);
    
    fillupSampleCombobox(m_ui.comboSampler);
    
    fillupInstrumentCombobox(m_ui.comboInstrument);
    
    createAudioGraph(); // create audio graph that uses Apple sampler and output
    
    m_ui.comboSampler->setContextMenuPolicy(Qt::CustomContextMenu); // select a sound font

    // Connect the context menu signal
    QObject::connect(m_ui.comboSampler, &QWidget::customContextMenuRequested,
                        [=](const QPoint &pos) { showContextMenu(this, pos); });
    
    QObject::connect(m_ui.comboInstrument, &QComboBox::currentIndexChanged, this, [=](){
        
        UInt8 presetNumber = (UInt8)m_ui.comboInstrument->currentData().value<SF2PresetHeader>().presetNumber;
        // it is not needed here. just for testing
        CFURLRef soundFontURL = CFURLCreateFromFileSystemRepresentation( kCFAllocatorDefault, (const UInt8 *)m_soundFont.toStdString().c_str()
                                                                        , m_soundFont.length(), false );
        // fill out a instrument data structure
         AUSamplerInstrumentData instdata;
        instdata.fileURL  = soundFontURL; //(CFURLRef) bankURL;
        instdata.instrumentType = kInstrumentType_DLSPreset;
        //instdata.bankMSB  = 121;
        instdata.bankMSB  = kAUSampler_DefaultMelodicBankMSB;
        instdata.bankLSB  = 0; //kAUSampler_DefaultBankLSB;
        
         instdata.presetID = (UInt8) presetNumber;

         // set the kAUSamplerProperty_LoadPresetFromBank property
         OSStatus result = AudioUnitSetProperty(m_samplerUnit,
                                       kAUSamplerProperty_LoadInstrument,
                                       kAudioUnitScope_Global,
                                       0,
                                       &instdata,
                                       sizeof(instdata));

        UInt8 note = 60;   // Middle C
        
        UInt8 velocity = 100;  // Volume

        std::cout << "Playing note: " << (int)note << std::endl;
        
        MusicDeviceMIDIEvent(m_samplerUnit, 0x90, note, velocity, 0);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        MusicDeviceMIDIEvent(m_samplerUnit, 0x80, note, 0, 0);
        
        std::cout << "Note Off.\n";
        
    });
    
    QObject::connect(CLighthouse::This(), &CLighthouse::midiEventList, this, &CMidiPlaybackSampler::handleMidiEventList);
};

CMidiPlaybackSampler::~CMidiPlaybackSampler()
{
    AUGraphStop( m_audioGraph);
    AUGraphClose(m_audioGraph);
};

void CMidiPlaybackSampler::startRecording(void)
{
    m_midirecorder.start();
};

void CMidiPlaybackSampler::stopRecording(void)
{
    m_midirecorder.stop();
};

void CMidiPlaybackSampler::pauseRecording(void)
{
    m_midirecorder.pause();
};

void CMidiPlaybackSampler::startPlayback(void)
{};

void CMidiPlaybackSampler::mutePlayback(void)
{};

int playtestnote();

OSStatus CreateAudioGraph();

OSStatus CMidiPlaybackSampler::createAudioGraph(void)
{
    OSStatus status = NewAUGraph(&m_audioGraph);
    if (status != noErr) return status;
    
    // Create AUSampler node
    AudioComponentDescription samplerDesc = {};
    samplerDesc.componentType = kAudioUnitType_MusicDevice;
    samplerDesc.componentSubType = kAudioUnitSubType_Sampler;
    samplerDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AUNode samplerNode;
    status = AUGraphAddNode(m_audioGraph, &samplerDesc, &samplerNode);
    if (status != noErr) return status;
    
    // Create output node
    AudioComponentDescription outputDesc = {};
    outputDesc.componentType = kAudioUnitType_Output;
    outputDesc.componentSubType = kAudioUnitSubType_DefaultOutput;
    //outputDesc.componentSubType = kAudioUnitSubType_GenericOutput;
    outputDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AUNode outputNode;
    status = AUGraphAddNode(m_audioGraph, &outputDesc, &outputNode);
    if (status != noErr) return status;
    
    // Open the graph and get unit references
    status = AUGraphOpen(m_audioGraph);
    if (status != noErr) return status;
    
    status = AUGraphNodeInfo(m_audioGraph, samplerNode, nullptr, &m_samplerUnit);
    if (status != noErr) return status;
    
    AudioUnit outputUnit;
    status = AUGraphNodeInfo(m_audioGraph, outputNode, nullptr, &outputUnit);
    if (status != noErr) return status;
    
    // Connect sampler to output
    status = AUGraphConnectNodeInput(m_audioGraph, samplerNode, 0, outputNode, 0);
    if (status != noErr) return status;
    
    // Start the audio graph
    status = AUGraphInitialize(m_audioGraph);
    if (status != noErr) return status;
    
    status = AUGraphStart(m_audioGraph);
    
    std::cout << "Audio graph started!\n";
    
    return status;
};

void CMidiPlaybackSampler::handleMidiEventList(const MIDIEventList *evtlist, void * __nullable srcConnRefCon)
{
    if (!evtlist) return;

    const MIDIEventPacket *packet = &evtlist->packet[0];
    
    for (UInt32 i = 0; i < evtlist->numPackets; i++) {
        
        for (UInt32 j = 0; j < packet->wordCount; j++) {
            
            UInt32 midiWord = packet->words[j];
            
            UInt32 byte0 = (midiWord & 0x000000ff); // Remove channel info (lower 4 bits)
            UInt32 byte1 = (midiWord & 0x0000ff00) >> 8; // Remove channel info (lower 4 bits)
            UInt32 byte2 = (midiWord & 0x00ff0000) >> 16; // Remove channel info (lower 4 bits)
            UInt32 byte3 = (midiWord & 0xff000000) >> 24; // Remove channel info (lower 4 bits)
            
            UInt8 channel = byte2 & 0x0f;
            UInt8 status = byte2 & 0xf0;
            UInt8 note = byte1;
            UInt8 velocity = byte0;
            
            //if (status == 0x90 || status == 0x80){
            
                OSStatus result = MusicDeviceMIDIEvent(m_samplerUnit, status |channel, note, velocity, 0);
                if (result != noErr) {
                    printf("Error sending MIDI event: %d\n", result);
                }
            //}
        }
        packet = MIDIEventPacketNext(packet); // Move to next MIDI packet
    }
    
    
};
