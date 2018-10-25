#include "peer.h"


void Peer::send(boost::asio::mutable_buffer data)
{
    psock_->async_send_to(data, endpoint,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            //sended
        });
}