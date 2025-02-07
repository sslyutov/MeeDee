/*!\brief midi input port */

#include "midiinputport.h"

#include <QDebug>

#include <mach/mach_time.h>

#include "lighthouse.h"

#include <QtConcurrent>

static mach_timebase_info_data_t timebaseInfo = {0, 0};

CMIDIInputPort* CMIDIInputPort::m_this = NULL;

bool CMIDIInputPort::m_bwatcher = true;

ItemCount CMIDIInputPort::m_prevSrcCount = 0;

MIDIPortRef CMIDIInputPort::m_inputPort(NULL);

MIDIClientRef CMIDIInputPort::m_midiClient(NULL);


CMIDIInputPort::CMIDIInputPort():
    QWidget()
{
    m_this = this;
    
    reignite();
    
    m_bwatcher = true;
    
    m_fwatcher = QtConcurrent::run(CMIDIInputPort::cbMidiSourcesWatcher);
    
}

CMIDIInputPort::~CMIDIInputPort()
{
    m_bwatcher = false;
    
    m_fwatcher.waitForFinished();
    
    MIDIPortDispose(m_inputPort);

    MIDIClientDispose(m_midiClient);
}

//void CMIDIInputPort::cbMidiRead(const MIDIPacketList * packetlist, void * readProcRefCon, void * srcConnRefCon )
//{
//    (void)readProcRefCon;
//
//    (void)srcConnRefCon;
//
//    for (unsigned int i = 0; i < packetlist->numPackets; ++i) {
//
//        const MIDIPacket& packet = packetlist->packet[i];
//
//        const uint8_t* data = packet.data;
//
//        if (timebaseInfo.denom == 0) {
//            mach_timebase_info(&timebaseInfo); // Initialize timebase info once
//        }
//
//        // Convert Mach units to nanoseconds
//        uint64_t tsmidi =  packet.timeStamp * timebaseInfo.numer / timebaseInfo.denom;
//        double dtsmidi = tsmidi;
//        // Print each byte in the packet
//        //qDebug() << "Timestamp: " << packet.timeStamp << " Data: ";
//        qDebug() << dtsmidi/1000000000.0;
//
//        CLighthouse::This()->emit CLighthouse::heartbit_0xfe(tsmidi);
//
//        for (unsigned int j = 0; j < packet.length; ++j) {
//
//            qDebug() << static_cast<int>(data[j]);
//        }
//    }
//}

//void cbmidiRead(const MIDIPacketList *packetlist, void *srcConnRefCon)
//{
//    for (unsigned int i = 0; i < packetlist->numPackets; ++i) {
//
//        const MIDIPacket& packet = packetlist->packet[i];
//
//        const uint8_t* data = packet.data;
//
//        if (timebaseInfo.denom == 0) {
//            mach_timebase_info(&timebaseInfo); // Initialize timebase info once
//        }
//
//        // Convert Mach units to nanoseconds
//        uint64_t tsmidi =  packet.timeStamp * timebaseInfo.numer / timebaseInfo.denom;
//        double dtsmidi = tsmidi;
//        // Print each byte in the packet
//        //qDebug() << "Timestamp: " << packet.timeStamp << " Data: ";
//        qDebug() << dtsmidi/1000000000.0;
//
//        CLighthouse::This()->emit CLighthouse::heartbit_0xfe(tsmidi);
//
//        for (unsigned int j = 0; j < packet.length; ++j) {
//
//            qDebug() << static_cast<int>(data[j]);
//        }
//    }
//}

void CMIDIInputPort::cbMidiSourcesWatcher(void)
{
    while(m_bwatcher){
        
        QThread::msleep(300);
        
        if(m_prevSrcCount == MIDIGetNumberOfSources())

            continue;
        
        m_prevSrcCount = MIDIGetNumberOfSources();
        
        reignite();
    }
};

auto readCallback = [](const MIDIEventList *evtlist, void * __nullable srcConnRefCon)

{
    for (unsigned int i = 0; i < evtlist->numPackets; ++i) {
        const MIDIEventPacket &packet = evtlist->packet[i];
        //qDebug() << "srcConnRefCon: " << srcConnRefCon << "; Timestamp: " << packet.timeStamp << " Data: ";
        
        for (unsigned int j = 0; j < packet.wordCount; ++j) {
            //qDebug() << "wordCount:" << packet.wordCount << "; index:" << j << std::hex << static_cast<UInt32>(packet.words[j]) << " ";
        }
        
        CLighthouse::This()->emit midiEventList(evtlist, srcConnRefCon);
        
    }
};


void CMIDIInputPort::reignite(void)
{
    qDebug() << "inputport reignite";
    
    if(m_inputPort){
        MIDIPortDispose(m_inputPort);
        m_inputPort = NULL;
    }
    
    if(m_midiClient){
        MIDIClientDispose(m_midiClient);
        m_midiClient = NULL;
    }
    
    OSStatus status = MIDIClientCreate(CFSTR("MIDI Src Client"), nullptr, nullptr, &m_midiClient);
    
    if (status != noErr) {
        
        qDebug() << "Failed to create MIDI client: " << status;
    }
    
    // Create an input port
    //status =
    status = MIDIInputPortCreateWithProtocol(m_midiClient, CFSTR("Input Port"), kMIDIProtocol_1_0, &m_inputPort
            , ^(const MIDIEventList *eventList, void *srcConnRefCon)
    {
            readCallback(eventList, srcConnRefCon);
    });
    
    if (status != noErr) {
        
        qDebug() << "Failed to create input port: " << status;
    }
    
    ItemCount sourceCount = MIDIGetNumberOfSources();
    
    for (ItemCount i = 0; i < sourceCount; ++i) {
        
        MIDIEndpointRef srcendpoint = MIDIGetSource(i);
        
        if (srcendpoint != 0) {
            
            status = MIDIPortConnectSource(m_inputPort, srcendpoint, nullptr);
            
            if (status != noErr) {
                
                qDebug() << "Failed to connect to MIDI source: " << status;
            } else {
                
                qDebug() << "Connected to MIDI source " << i;
            }
        }
    }
    CLighthouse::This()->emit reignite();
};
