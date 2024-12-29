/*!\brief about dialog
 * \file about.cpp
 * \date December 25, 2024
 * \author Sergey Slyutov  */

#include "about.h"

#include <QDebug>

#include <CoreFoundation/CoreFoundation.h>

#include <CoreMIDI/CoreMIDI.h>

#include <QLabel>

#include <QTableWidget>

#include <QStringList>

CAbout::CAbout()
    :QWidget()
{
    m_ui.setupUi(this);
    
    setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(m_ui.pushClose, &QPushButton::released, [=](){ close(); });
    
    refreshMidiDevices();
    
    m_ui.tableMidiDevs->horizontalHeader()->setStretchLastSection(true);
    
    m_ui.tableMidiDevs->setSelectionMode(QAbstractItemView::SingleSelection); // Single row selection

    m_ui.tableMidiDevs->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    QObject::connect(m_ui.tableMidiDevs, &QTableWidget::itemSelectionChanged, [&]() {
        
        QList<QTableWidgetItem *> selectedItems = m_ui.tableMidiDevs->selectedItems();

        if (!selectedItems.isEmpty()) {
            
            int row = selectedItems.first()->row();
            
            QString curdevname = m_ui.tableMidiDevs->item(row, 0)->data(Qt::UserRole).toString();
            
            refreshMidiDevsDetails(curdevname);
        }
    });
  
    m_ui.labelGenInfo->setText(QString("%1 %2 %3").arg(QApplication::organizationName()).arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));
}

CAbout::~CAbout(){};

void CAbout::refreshMidiDevices()
{
    m_ui.tableMidiDevs->clear();
        
    m_ui.tableMidiDevs->verticalHeader()->setVisible(false);
    
    m_ui.tableMidiDevs->setColumnCount(1);
        
    m_ui.tableMidiDevs->horizontalHeader()->setStretchLastSection(true);

    m_ui.tableMidiDevs->setHorizontalHeaderLabels( QStringList( {"MIDI devices"}) );
    
    int deviceCount = (int)MIDIGetNumberOfDevices();
    
    m_ui.tableMidiDevs->setRowCount(deviceCount);
    
    for(int row = 0; row < deviceCount; row++){
        
        MIDIDeviceRef device = MIDIGetDevice(row);
        
        CFStringRef deviceName = nullptr;
        
        MIDIObjectGetStringProperty(device, kMIDIPropertyName, &deviceName);
                
        if (deviceName) {
            
            QString sdevName = QString::fromCFString(deviceName);
            
            QTableWidgetItem * pitem = new QTableWidgetItem(sdevName);
            
            pitem->setData(Qt::UserRole, sdevName);
        
            m_ui.tableMidiDevs->setItem(row, 0, pitem);
        }
    }
    m_ui.tableMidiDevs->resizeRowsToContents();
    
    m_ui.tableMidiDevs->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    m_ui.tableMidiDevs->selectRow(0);
    
    refreshMidiDevsDetails(m_ui.tableMidiDevs->item(m_ui.tableMidiDevs->currentRow(), 0)->data(Qt::UserRole).toString());
};

void CAbout::refreshMidiDevsDetails(QString devname)
{
    
    int deviceCount = (int)MIDIGetNumberOfDevices();
    
    m_ui.tableMidiDevs->setRowCount(deviceCount);
    
    QString details;
    
    for(int row = 0; row < deviceCount; row++){
        
        MIDIDeviceRef device = MIDIGetDevice(row);
        
        CFStringRef deviceName = nullptr;
        
        MIDIObjectGetStringProperty(device, kMIDIPropertyName, &deviceName);
        
        if( QString::fromCFString(deviceName).compare(devname, Qt::CaseInsensitive) == 0){
            
            SInt32 offline;
                
            MIDIObjectGetIntegerProperty(device, kMIDIPropertyOffline, &offline);
                
            details += QString("status: %1").arg((offline) ? "offline": "online") ;
            

            ItemCount entityCount = MIDIDeviceGetNumberOfEntities(device);
            
            details += "\n" + QString("entities count %1").arg(entityCount);
            
            while(entityCount--) {
                
                qWarning() << "No entities available in the selected device.";
                
                MIDIEntityRef entity = MIDIDeviceGetEntity(device, 0); // Use the first entity
                
                if (entity) {
                    
                    MIDIObjectGetIntegerProperty(entity, kMIDIPropertyOffline, &offline);
                    
                    details += QString("\n\tentity %1 %2").arg(entityCount).arg((offline)? "offline": "online");
                }
            }
        }
        
        CFPropertyListRef propertyList = nullptr;
        
        CFRelease(deviceName);
    }
    m_ui.textMideDevDtails->setText(details);
}


