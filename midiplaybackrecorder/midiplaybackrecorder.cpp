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

void addMidiPlaybackRecorderTrack(QTreeWidget * ptreewidget)
{
    QTreeWidgetItem * item = new QTreeWidgetItem(ptreewidget);
 
    ptreewidget->setItemWidget(item,0, new CMidiPlaybackRecorder());
};

void showContextMenu(QTreeWidget * ptreewidget, const QPoint &pos)
{
    QMenu contextMenu(ptreewidget);

    // Add actions to the menu
    QAction *newplaybacktrack = contextMenu.addAction("new midi playback recorder track");
    
    // Execute the menu and get the selected action
    QAction *selectedAction = contextMenu.exec(ptreewidget->mapToGlobal(pos));

    // Handle the selected action
    if (selectedAction == newplaybacktrack) {
        
        addMidiPlaybackRecorderTrack(ptreewidget);
    }
};


QDockWidget * createPlaybackRecorderDoc(void)
{
    QDockWidget * playbackrecorderdoc = new QDockWidget();
    
    playbackrecorderdoc->setFeatures(playbackrecorderdoc->features() & ~QDockWidget::DockWidgetClosable);

    playbackrecorderdoc->setObjectName(PLAYBACKRECORDERDOC);
    
    QTreeWidget * pinputchantree = new QTreeWidget();
    
    //ptree->addTopLevelItem(new QTreeWidgetItem("MIDI Playback Recorder"));
    
    pinputchantree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
    playbackrecorderdoc->setWidget(pinputchantree);
      
    pinputchantree->setContextMenuPolicy(Qt::CustomContextMenu);
    
    QObject::connect( pinputchantree, &QWidget::customContextMenuRequested, pinputchantree, [=](const QPoint &pos){
    
        showContextMenu(pinputchantree, pos);
   
    });
    
   
    
    return playbackrecorderdoc;
};

CMidiPlaybackRecorder::CMidiPlaybackRecorder(QString name):
    m_name(name)
{
    m_ui.setupUi(this);
    
    m_ui.lineEditPlayRecName->setText(m_name);
    
    QObject::connect(CLighthouse::This(), &CLighthouse::startRecording, [](){ QMessageBox::information(NULL, qApp->applicationDisplayName(),"handle start recording", QMessageBox::Ok);});
    QObject::connect(CLighthouse::This(), &CLighthouse::stopRecording,  [](){ QMessageBox::information(NULL, qApp->applicationDisplayName(),"handle stop recording", QMessageBox::Ok);});
    QObject::connect(CLighthouse::This(), &CLighthouse::startPlayback,  [](){ QMessageBox::information(NULL, qApp->applicationDisplayName(),"handle start playback", QMessageBox::Ok);});
    QObject::connect(CLighthouse::This(), &CLighthouse::stopPlayback,  [](){ QMessageBox::information(NULL, qApp->applicationDisplayName(),"handle stop playback", QMessageBox::Ok);});
    
    
    
    
    
    
    
    // sources combobox
    ItemCount sourceCount = MIDIGetNumberOfSources();
    
    for (ItemCount i = 0; i < sourceCount; ++i) {
        
        MIDIEndpointRef endpoint = MIDIGetSource(i);
        
        CFStringRef name = nullptr;
        
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
        
        m_ui.comboSources->addItem(QString::fromCFString(name));
    }
    // sources channels
    for(int i = 0; i < 16; i++){
        m_ui.comboSrcChan->addItem(QString("channel %1").arg(i+1));
        m_ui.comboDstChan->setItemData(i, Qt::UserRole, i+1);
    }
    
    
    // destination combobox
    ItemCount destCount = MIDIGetNumberOfDestinations();
    
    for(ItemCount i = 0; i < destCount; i++){
        
        MIDIEndpointRef endpoint = MIDIGetDestination(i);
        
        CFStringRef name = nullptr;
        
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
        
        m_ui.comboDestinations->addItem(QString::fromCFString(name));
    }
    // destination channels
    for(int i = 0 ; i < 16 ; i++){
        m_ui.comboDstChan->addItem(QString("channel %1").arg(i+1));
        m_ui.comboDstChan->setItemData(i, Qt::UserRole, i+1);
    }
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


