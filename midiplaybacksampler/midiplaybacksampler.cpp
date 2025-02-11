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
    AUGraphStop( m_audiograph);
    AUGraphClose(m_audiograph);
};

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
