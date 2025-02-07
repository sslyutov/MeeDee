/*!\brief
 * \file midiplaybackrecorderdoc.cpp
 * \author Sergey Slyutov
 * \date February 6, 2025  */
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
