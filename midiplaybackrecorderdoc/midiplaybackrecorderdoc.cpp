/*!\brief
 * \file midiplaybackrecorderdoc.cpp
 * \author Sergey Slyutov
 * \date February 6, 2025  */

#include "midiplaybackrecorder.h"

#include "midiplaybacksampler.h"

#include <QMenu>

void showContextMenu(QTreeWidget * ptreewidget, const QPoint &pos)
{
    QMenu contextMenu(ptreewidget);

    // Add actions to the menu
    QAction *midisrc_mididest = contextMenu.addAction("new midi source with midi destination");
    QAction *midisrc_sampler = contextMenu.addAction("new midi source with sampler");
    
    
    // Execute the menu and get the selected action
    QAction *selectedAction = contextMenu.exec(ptreewidget->mapToGlobal(pos));

    // Handle the selected action
    if (selectedAction == midisrc_mididest) {
        addMidiPlaybackRecorder(ptreewidget);
    }
    if(selectedAction == midisrc_sampler) {
        addMidiPlaybackSampler(ptreewidget);
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
