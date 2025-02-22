/*!\brief midi input port */

#include "midiinputport.h"

#include <QDebug>

#include <mach/mach_time.h>

#include "lighthouse.h"

#include <QtConcurrent>

#include "utils.h"

#include <CoreAudio/HostTime.h>

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
    //qDebug() << "protocol:" << evtlist->protocol;
    
    UInt8 byte0 = 0;
    UInt8 byte1 = 0;
    UInt8 byte2 = 0;
    UInt8 byte3 = 0;
    
    for (unsigned int i = 0; i < evtlist->numPackets; ++i) {
        const MIDIEventPacket &packet = evtlist->packet[i];
        
       // qDebug() << "timestamp:" << AudioConvertHostTimeToNanos(packet.timeStamp) << " nSecs";
      
        for (unsigned int j = 0; j < packet.wordCount; ++j) {
            UInt32 midiWord = packet.words[j];
            // Extract MIDI status, data1, data2
            
            byte0 = (midiWord & 0x000000ff); // Remove channel info (lower 4 bits)
            byte1 = (midiWord & 0x0000ff00) >> 8; // Remove channel info (lower 4 bits)
            byte2 = (midiWord & 0x00ff0000) >> 16; // Remove channel info (lower 4 bits)
            byte3 = (midiWord & 0xff000000) >> 24; // Remove channel info (lower 4 bits)
        }
    }
    if( byte2 != 0xFE){
        
        qDebug() << "channel:" << (byte2 & 0x0f) << " status:" << (byte2 & 0xf0) << " note:" << byte1 << " velocity:" << byte0;
        
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
            
            status = MIDIPortConnectSource(m_inputPort, srcendpoint, new int(i));
            
            if (status != noErr) {
                
                qDebug() << "Failed to connect to MIDI source: " << status;
            } else {
                
                qDebug() << "Connected to MIDI source " << i;
            }
        }
    }
    CLighthouse::This()->emit reignite();
};
