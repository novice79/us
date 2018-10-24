#include "pmgr.h"
#include "peer.h"

void PMgr::peer_checker(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* t)
{
    const double expire_duration = 60 * 5;  //five minutes
    for ( auto it = peers_.begin(); it != peers_.end(); ) {
        auto &p = it->second;
        if ( p->ep_timer.elapse() > expire_duration) {
            ep2p_.erase(p->endpoint);
            it = peers_.erase(it);
        }
        else {
            ++it;
        }
    }
    t->expires_at(t->expires_at() + boost::posix_time::seconds( 10 ));
    t->async_wait(boost::bind(&PMgr::peer_checker, this,
                              boost::asio::placeholders::error, t));
}
void PMgr::start()
{
    static boost::asio::deadline_timer evictor(io_context_, boost::posix_time::seconds(10));
    evictor.async_wait(boost::bind(&PMgr::peer_checker, this, boost::asio::placeholders::error, &evictor));
    do_receive();
}
void PMgr::do_receive()
{
    socket_.async_receive_from(
        boost::asio::buffer(buffer_), sender_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0 && buffer_[0] == 0x79)
            {
                switch(buffer_[1])
                {
                    case 0:
                        if(bytes_recvd == 6)
                        {
                            auto p = std::make_shared<Peer>(&socket_, sender_endpoint_);
                            uint32_t id = *((uint32_t*)&buffer_[1]);
                            peers_[id] = p;
                            ep2p_[sender_endpoint_] = p;
                            std::vector<uint32_t> all_id, sel_ids;
                            boost::copy(peers_ | boost::adaptors::map_keys, std::back_inserter(all_id));
                            std::sample(all_id.begin(), all_id.end(), std::back_inserter(sel_ids),
                                        50, std::mt19937{std::random_device{}()});
                            std::array<uint8_t, sel_ids.size()+8> buff;
                            uint16_t id_len = sel_ids.size();
                            uint32_t total_len = all_id.size();
                            buff[0] = 0x51;
                            buff[1] = 0x00;
                            memcpy( &buff[2], &id_len, sizeof(id_len) );
                        }
                        break;
                    case 1:

                        break;
                    case 2:

                        break;    
                    case 3:

                        break;
                    case 4:

                        break;
                    case 5:

                        break;

                }

            }
            do_receive();
        });
}

