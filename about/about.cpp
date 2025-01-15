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

#include "utils.h"

CAbout::CAbout()
    :QWidget()
{
    m_ui.setupUi(this);
    
    m_ui.tableMidiDevs->setAttribute(Qt::WA_InputMethodEnabled, false);
    
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
    
    refreshMidiSources();
    
    refreshMIDIDestinations();
    
    refreshExternalDevices();
    
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
    
    //refreshMidiDevsDetails(m_ui.tableMidiDevs->item(m_ui.tableMidiDevs->currentRow(), 0)->data(Qt::UserRole).toString());
};


void CAbout::refreshMidiSources(void)
{
    m_ui.tableMIDISrc->clear();
    m_ui.tableMIDISrc->setColumnCount(5);
    m_ui.tableMIDISrc->setHorizontalHeaderLabels({"name", "model", "manufacturer", "unique id", "device id"});
    m_ui.tableMIDISrc->verticalHeader()->QWidget::setVisible(false);
    
    ItemCount srcnum = MIDIGetNumberOfSources();
    
    m_ui.tableMIDISrc->setRowCount((int)srcnum);
    
    for(ItemCount c = 0; c < srcnum; c++){
        MIDIEndpointRef endpoint = MIDIGetSource(c);
        
        CFStringRef name = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
        
        CFStringRef model = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyModel, &model);
 
        CFStringRef manuf = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyManufacturer, &manuf);
        
        CFStringRef uniqid = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyUniqueID, &uniqid);
        
        CFStringRef devid = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyDeviceID, &devid);
        
        if(name){
            QString strname = QString::fromCFString(name);
            QTableWidgetItem * pname = new QTableWidgetItem(strname);
            pname->setData(Qt::UserRole + 0, strname);
            m_ui.tableMIDISrc->setItem(static_cast<int>(c), 0, pname);
        }
            
        QTableWidgetItem * pmodel = new QTableWidgetItem(QString::fromCFString(model));
        pmodel->setData(Qt::UserRole + 0, QString::fromCFString(model));
        m_ui.tableMIDISrc->setItem(static_cast<int>(c), 1, pmodel);

        QTableWidgetItem * pmanuf = new QTableWidgetItem(QString::fromCFString(manuf));
        pmanuf->setData(Qt::UserRole + 0, QString::fromCFString(manuf));
        m_ui.tableMIDISrc->setItem(static_cast<int>(c), 2, pmanuf);

        QTableWidgetItem * puniqid = new QTableWidgetItem(QString::fromCFString(uniqid));
        puniqid->setData(Qt::UserRole + 0, QString::fromCFString(uniqid));
        m_ui.tableMIDISrc->setItem(static_cast<int>(c), 3, puniqid);

        QTableWidgetItem * pdevid = new QTableWidgetItem(QString::fromCFString(devid));
        pdevid->setData(Qt::UserRole + 0, QString::fromCFString(devid));
        m_ui.tableMIDISrc->setItem(static_cast<int>(c), 4, pdevid);

    }
}
void CAbout::refreshMIDIDestinations(void)
{
    m_ui.tableMIDIDest->clear();
    m_ui.tableMIDIDest->setColumnCount(5);
    m_ui.tableMIDIDest->setHorizontalHeaderLabels({"name", "model", "manufacturer", "unique id", "device id"});
    m_ui.tableMIDIDest->verticalHeader()->QWidget::setVisible(false);
    
    ItemCount  dstnum = MIDIGetNumberOfDestinations();
    
    m_ui.tableMIDIDest->setRowCount((int)dstnum);

    for(ItemCount c = 0; c < dstnum; c++){
        MIDIEndpointRef endpoint = MIDIGetDestination(c);
        
        CFStringRef name = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
        
        CFStringRef model = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyModel, &model);
 
        CFStringRef manuf = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyManufacturer, &manuf);
        
        CFStringRef uniqid = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyUniqueID, &uniqid);
        
        CFStringRef devid = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyDeviceID, &devid);
        
        if(name){
            QString strname = QString::fromCFString(name);
            QTableWidgetItem * pname = new QTableWidgetItem(strname);
            pname->setData(Qt::UserRole + 0, strname);
            m_ui.tableMIDIDest->setItem(static_cast<int>(c), 0, pname);
        }
            
        QTableWidgetItem * pmodel = new QTableWidgetItem(QString::fromCFString(model));
        pmodel->setData(Qt::UserRole + 0, QString::fromCFString(model));
        m_ui.tableMIDIDest->setItem(static_cast<int>(c), 1, pmodel);

        QTableWidgetItem * pmanuf = new QTableWidgetItem(QString::fromCFString(manuf));
        pmanuf->setData(Qt::UserRole + 0, QString::fromCFString(manuf));
        m_ui.tableMIDIDest->setItem(static_cast<int>(c), 2, pmanuf);

        QTableWidgetItem * puniqid = new QTableWidgetItem(QString::fromCFString(uniqid));
        puniqid->setData(Qt::UserRole + 0, QString::fromCFString(uniqid));
        m_ui.tableMIDIDest->setItem(static_cast<int>(c), 3, puniqid);

        QTableWidgetItem * pdevid = new QTableWidgetItem(QString::fromCFString(devid));
        pdevid->setData(Qt::UserRole + 0, QString::fromCFString(devid));
        m_ui.tableMIDIDest->setItem(static_cast<int>(c), 4, pdevid);
    }
}

void CAbout::refreshExternalDevices()
{
    m_ui.tableMIDIExt->clear();
    m_ui.tableMIDIExt->setColumnCount(5);
    m_ui.tableMIDIExt->setHorizontalHeaderLabels({"name", "model", "manufacturer", "unique id", "device id"});
    m_ui.tableMIDIExt->verticalHeader()->QWidget::setVisible(false);

    ItemCount extnum = MIDIGetNumberOfExternalDevices();
    
    m_ui.tableMIDIExt->setRowCount((int)extnum);

    
    for (ItemCount c = 0 ; c < extnum; c++) {
        MIDIEndpointRef endpoint = MIDIGetExternalDevice(c);
        
        CFStringRef name = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &name);
        
        CFStringRef model = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyModel, &model);
 
        CFStringRef manuf = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyManufacturer, &manuf);
        
        CFStringRef uniqid = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyUniqueID, &uniqid);
        
        CFStringRef devid = nullptr;
        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyDeviceID, &devid);
        
        if(name){
            QString strname = QString::fromCFString(name);
            QTableWidgetItem * pname = new QTableWidgetItem(strname);
            pname->setData(Qt::UserRole + 0, strname);
            m_ui.tableMIDIExt->setItem(static_cast<int>(c), 0, pname);
            
        }
            
        QTableWidgetItem * pmodel = new QTableWidgetItem(QString::fromCFString(model));
        pmodel->setData(Qt::UserRole + 0, QString::fromCFString(model));
        m_ui.tableMIDIExt->setItem(static_cast<int>(c), 1, pmodel);

        QTableWidgetItem * pmanuf = new QTableWidgetItem(QString::fromCFString(manuf));
        pmanuf->setData(Qt::UserRole + 0, QString::fromCFString(manuf));
        m_ui.tableMIDIExt->setItem(static_cast<int>(c), 2, pmanuf);

        QTableWidgetItem * puniqid = new QTableWidgetItem(QString::fromCFString(uniqid));
        puniqid->setData(Qt::UserRole + 0, QString::fromCFString(uniqid));
        m_ui.tableMIDIExt->setItem(static_cast<int>(c), 3, puniqid);

        QTableWidgetItem * pdevid = new QTableWidgetItem(QString::fromCFString(devid));
        pdevid->setData(Qt::UserRole + 0, QString::fromCFString(devid));
        m_ui.tableMIDIExt->setItem(static_cast<int>(c), 4, pdevid);
    }
}

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
        
        //CFPropertyListRef propertyList = nullptr;
        
        
        //CFRelease(deviceName);
    }
    m_ui.labelMideDevDtails->setText(details);
}


