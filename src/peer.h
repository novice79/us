#pragma once

#include "common.h"
class Peer
{
public:
    Peer(udp::socket* ps, udp::endpoint ep)
    :psock_(ps), endpoint(ep){}
    void send(const std::vector<uint8_t>& data);
public:    
    PTimer ep_timer;
    udp::endpoint endpoint;  
private:   
    udp::socket* psock_;
      
};