/*!\brief description of MIDI output destinations
* \file midioutputport.h
* \date January 7, 2025
* \author Sergey Slyuto */

#include "midioutputport.h"

#include <QtConcurrent>

MIDIClientRef CMIDIOutputPort::m_client(NULL);

MIDIPortRef CMIDIOutputPort::m_outputPort(NULL);

CMIDIOutputPort* CMIDIOutputPort::m_this = NULL;

bool CMIDIOutputPort::m_bwatcher = true;

ItemCount CMIDIOutputPort::m_prevDstCount = 0;

CMIDIOutputPort::CMIDIOutputPort():  QWidget()
{
    m_this = this;
    
    reignite();
    
    m_bwatcher = true;
    
    m_fwatcher = QtConcurrent::run(CMIDIOutputPort::cdMidiDesctinationsWatcher);
};


CMIDIOutputPort::~CMIDIOutputPort()
{};

void CMIDIOutputPort::reignite(void)
{
    qDebug() << "outputport reignite";
    
    if(m_outputPort){
        MIDIPortDispose(m_outputPort);
        m_outputPort = NULL;
    }
    
    if(m_client){
        MIDIClientDispose(m_client);
        m_client = NULL;
    }
    
    OSStatus status = MIDIClientCreate(CFSTR("MIDI Destination Client"), nullptr, nullptr, &m_client);
    if ( status != noErr ) {
        qDebug() << "Failed to create MIDI client.";
        return;
    }
    
    // Create an output port
    status = MIDIOutputPortCreate(m_client, CFSTR("Output Port"), &m_outputPort);
    if ( status != noErr ) {
        qDebug() << "Failed to create MIDI output port.";
        MIDIClientDispose(m_client);
        return;
    }
    
    m_prevDstCount = MIDIGetNumberOfDestinations();
    
    for(ItemCount i = 0; i < m_prevDstCount; i++){
        
        MIDIEndpointRef dstendpoint =  MIDIGetDestination(i);
        
        if(dstendpoint != 0){
        
        }
            
     
    }
    
    //// Send Note On message
    //{
    //    MIDIPacket* packet = MIDIPacketListInit(packetList);
    //    Byte noteOnMessage[] = { static_cast<Byte>(0x90 | (channel & 0x0F)), note, velocity }; // Note On
    //    packet = MIDIPacketListAdd(packetList, sizeof(packetBuffer), packet, 0, sizeof(noteOnMessage), noteOnMessage);
    //    //MIDISend(outputPort, destination, packetList);
    //    //std::cout << "Note On: " << (int)note << " (velocity: " << (int)velocity << ")" << std::endl;
    //}
};

void CMIDIOutputPort::cdMidiDesctinationsWatcher()
{
    while(m_bwatcher){
        
        QThread::msleep(300);
        
        if(m_prevDstCount == MIDIGetNumberOfDestinations())
            
            continue;
        
        m_prevDstCount = MIDIGetNumberOfDestinations();
        
        reignite();
    }
};
