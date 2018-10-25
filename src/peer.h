#pragma once

#include "common.h"
class Peer
{
    void send(boost::asio::mutable_buffer data);
public:
    Peer(uint32_t pid, udp::socket* ps, udp::endpoint ep)
    :id(pid), psock_(ps), endpoint(ep){}
    void send(std::vector<uint8_t>& data)
    {
        send( boost::asio::buffer(data) );
    }
    void send(uint8_t* data, uint16_t len)
    {
        send( boost::asio::buffer(data, len) );
    }
public:    
    uint32_t id;
    PTimer ep_timer;
    udp::endpoint endpoint;  
private:   
    udp::socket* psock_;
      
};