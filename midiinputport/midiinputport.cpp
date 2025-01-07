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
    
    m_fwatcher = QtConcurrent::run(CMIDIInputPort::cdMidiSourcesWatcher);
    
}

CMIDIInputPort::~CMIDIInputPort()
{
    m_bwatcher = false;
    
    m_fwatcher.waitForFinished();
    
    MIDIPortDispose(m_inputPort);

    MIDIClientDispose(m_midiClient);
}

void CMIDIInputPort::cbMidiRead(const MIDIPacketList * packetlist, void * readProcRefCon, void * srcConnRefCon )
{
    (void)readProcRefCon;

    (void)srcConnRefCon;

    for (unsigned int i = 0; i < packetlist->numPackets; ++i) {

        const MIDIPacket& packet = packetlist->packet[i];

        const uint8_t* data = packet.data;
        
        if (timebaseInfo.denom == 0) {
            mach_timebase_info(&timebaseInfo); // Initialize timebase info once
        }

        // Convert Mach units to nanoseconds
        uint64_t tsmidi =  packet.timeStamp * timebaseInfo.numer / timebaseInfo.denom;
        double dtsmidi = tsmidi;
        // Print each byte in the packet
        //qDebug() << "Timestamp: " << packet.timeStamp << " Data: ";
        qDebug() << dtsmidi/1000000000.0;

        CLighthouse::This()->emit CLighthouse::heartbit_0xfe(tsmidi);
               
        for (unsigned int j = 0; j < packet.length; ++j) {

            qDebug() << static_cast<int>(data[j]);
        }
    }
}

void CMIDIInputPort::cdMidiSourcesWatcher(void)
{
    while(m_bwatcher){
        
        QThread::msleep(300);
        
        if(m_prevSrcCount == MIDIGetNumberOfSources())

            continue;
        
        m_prevSrcCount = MIDIGetNumberOfSources();
        
        reignite();
    }
};


void CMIDIInputPort::reignite(void)
{
    qDebug() << "reignite";
    
    if(m_inputPort){
        MIDIPortDispose(m_inputPort);
        m_inputPort = NULL;
    }

    if(m_midiClient){
        MIDIClientDispose(m_midiClient);
        m_midiClient = NULL;
    }
    
    OSStatus status = MIDIClientCreate(CFSTR("MIDI Client"), nullptr, nullptr, &m_midiClient);

    if (status != noErr) {

        qDebug() << "Failed to create MIDI client: " << status;
    }

    // Create an input port
    status = MIDIInputPortCreate(m_midiClient, CFSTR("Input Port"), CMIDIInputPort::cbMidiRead, nullptr, &m_inputPort);

    if (status != noErr) {

        qDebug() << "Failed to create input port: " << status;
    }

    ItemCount sourceCount = MIDIGetNumberOfSources();

    for (ItemCount i = 0; i < sourceCount; ++i) {

        MIDIEndpointRef source = MIDIGetSource(i);

        if (source != 0) {

            status = MIDIPortConnectSource(m_inputPort, source, nullptr);

            if (status != noErr) {

                qDebug() << "Failed to connect to MIDI source: " << status;
            } else {

                qDebug() << "Connected to MIDI source " << i;
            }
        }
    }};
