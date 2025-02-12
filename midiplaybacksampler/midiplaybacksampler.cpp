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
    
    m_ui.comboSampler->setContextMenuPolicy(Qt::CustomContextMenu);

    // Connect the context menu signal
    QObject::connect(m_ui.comboSampler, &QWidget::customContextMenuRequested,
                        [=](const QPoint &pos) { showContextMenu(this, pos); });
};

CMidiPlaybackSampler::~CMidiPlaybackSampler()
{
    AUGraphStop( m_audioGraph);
    AUGraphClose(m_audioGraph);
};

void CMidiPlaybackSampler::startRecording(void)
{};

void CMidiPlaybackSampler::stopRecording(void)
{};

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

    // it is not needed here. just for testing
    CFURLRef soundFontURL = CFURLCreateFromFileSystemRepresentation( kCFAllocatorDefault, (const UInt8 *)m_soundFont.toStdString().c_str()
                                                                    , m_soundFont.length(), false );
    // fill out a instrument data structure
     AUSamplerInstrumentData instdata;
    instdata.fileURL  = soundFontURL; //(CFURLRef) bankURL;
    instdata.instrumentType = kInstrumentType_DLSPreset;
    instdata.bankMSB  = 121; //kAUSampler_DefaultMelodicBankMSB;
    instdata.bankLSB  = 0; //kAUSampler_DefaultBankLSB;
    UInt8 presetNumber = 10;
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

    MusicDeviceMIDIEvent(m_samplerUnit, 0x90, note+4, velocity, 0);
    
    MusicDeviceMIDIEvent(m_samplerUnit, 0x90, note+10, velocity, 0);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    MusicDeviceMIDIEvent(m_samplerUnit, 0x80, note, 0, 0);
    std::cout << "Note Off.\n";
    
    return result;
    
};
