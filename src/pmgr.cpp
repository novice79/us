#include "pmgr.h"
#include "peer.h"

void PMgr::peer_checker(const boost::system::error_code & /*e*/, boost::asio::deadline_timer *t)
{
    const double expire_duration = 60 * 2; //two minutes
    for (auto it = peers_.begin(); it != peers_.end();)
    {
        auto &p = it->second;
        if (p->ep_timer.elapse() > expire_duration)
        {
            ep2p_.erase(p->endpoint);
            it = peers_.erase(it);
        }
        else
        {
            ++it;
        }
    }
    t->expires_at(t->expires_at() + boost::posix_time::seconds(10));
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
                switch (buffer_[1])
                {
                case 0:
                    handle_reg(bytes_recvd);
                    break;
                case 1:
                case 2:
                    handle_sig(bytes_recvd);
                    break;
                case 3:
                    handle_pick_ids(bytes_recvd);
                    break;
                case 4:
                    handle_msg_relay(bytes_recvd);
                    break;
                case 5:
                    handle_heartbeat(bytes_recvd);
                    break;
                }
            }
            do_receive();
        });
}

void PMgr::handle_reg(uint16_t len)
{
    if (len != 6)
        return;
    uint32_t id = *((uint32_t *)&buffer_[2]);
    auto p = std::make_shared<Peer>(id, &socket_, sender_endpoint_);
    peers_[id] = p;
    ep2p_[sender_endpoint_] = p;
    std::vector<uint32_t> all_id, sel_ids;
    boost::copy(peers_ | boost::adaptors::map_keys, std::back_inserter(all_id));
    std::sample(all_id.begin(), all_id.end(), std::back_inserter(sel_ids),
                50, std::mt19937{std::random_device{}()});
    std::vector<uint8_t> buff(sel_ids.size() + 8);
    uint16_t id_len = sel_ids.size();
    uint32_t total_len = all_id.size();
    buff[0] = 0x51;
    buff[1] = 0x00;
    memcpy(&buff[2], &id_len, sizeof(id_len));
    memcpy(&buff[4], sel_ids.data(), id_len * sizeof(uint32_t) );
    memcpy(&buff[buff.size() - 4], &total_len, sizeof(total_len));
    p->send(buff);
}
void PMgr::handle_sig(uint16_t len)
{
    uint16_t sig_len = *((uint16_t *)&buffer_[6]);
    if (len != sig_len + 8)
        return;
    auto ep_it = ep2p_.find(sender_endpoint_);
    if (ep_it == ep2p_.end())
        return;
    auto sp = ep_it->second;
    sp->ep_timer.reset();
    uint32_t target_id = *((uint32_t *)&buffer_[2]);
    auto it = peers_.find(target_id);
    if (it == peers_.end())
        return;
    auto tp = it->second;
    buffer_[0] = 0x51;
    buffer_[1] = buffer_[1] == 0x01 ? 0x02 : 0x01;
    memcpy(&buffer_[2], &sp->id, sizeof(uint32_t));
    tp->send(&buffer_[0], len);
}
void PMgr::handle_pick_ids(uint16_t len)
{
    if (len != 2)
        return;
    auto ep_it = ep2p_.find(sender_endpoint_);
    if (ep_it == ep2p_.end())
        return;
    auto sp = ep_it->second;
    sp->ep_timer.reset();
    std::vector<uint32_t> all_id, sel_ids;
    boost::copy(peers_ | boost::adaptors::map_keys, std::back_inserter(all_id));
    std::sample(all_id.begin(), all_id.end(), std::back_inserter(sel_ids),
                50, std::mt19937{std::random_device{}()});
    std::vector<uint8_t> buff(sel_ids.size() + 4);
    uint16_t id_len = sel_ids.size();
    buff[0] = 0x51;
    buff[1] = 0x03;
    memcpy(&buff[2], &id_len, sizeof(id_len));
    memcpy(&buff[4], sel_ids.data(), id_len * 4);
    sp->send(buff);
}
void PMgr::handle_msg_relay(uint16_t len)
{
    auto msg_len = buffer_[2];
    if (len != msg_len + 3)
        return;
    auto ep_it = ep2p_.find(sender_endpoint_);
    if (ep_it == ep2p_.end())
        return;
    auto sp = ep_it->second;
    sp->ep_timer.reset();
    // todo: throttle
    std::vector<uint8_t> buff(len + 4);
    memcpy(&buff[6], &buffer_[2], msg_len +1);
    memcpy(&buff[2], &sp->id, sizeof(uint32_t));
    buff[0] = 0x51;
    buff[1] = 0x04;
    for(auto &it : peers_)
    {
        auto p = it.second;
        p->send(buff);
    }
}
void PMgr::handle_heartbeat(uint16_t len)
{
    if (len != 2)
        return;
    auto ep_it = ep2p_.find(sender_endpoint_);
    if (ep_it == ep2p_.end())
        return;
    auto sp = ep_it->second;
    sp->ep_timer.reset();
    std::vector<uint8_t> buff(6);
    buff[0] = 0x51;
    buff[1] = 0x05;
    uint32_t count = peers_.size();
    memcpy(&buff[2], &count, sizeof(count));
    sp->send(buff);
}