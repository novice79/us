#include "peer.h"

void Peer::send(const std::vector<uint8_t>& data)
{
    psock_->async_send_to(
        boost::asio::buffer(data), endpoint,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            //sended
        });
}