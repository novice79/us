#pragma once

#include "common.h"

class Peer;
class PMgr
{
    void peer_checker(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* t);
  public:
    PMgr(boost::asio::io_context &io_context, short port)
        : io_context_(io_context), socket_(io_context, udp::endpoint(udp::v4(), port))
    {
        start();
    }
    void start();
    void do_receive();

  private:
    boost::asio::io_context& io_context_;
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    std::array<uint8_t, 8192> buffer_;
    std::map<uint32_t, std::shared_ptr<Peer>> peers_;
    std::map<udp::endpoint, std::shared_ptr<Peer>> ep2p_;
};
// 65,507 bytes (65,535 − 8 byte UDP header − 20 byte IP header)