#pragma once

#include <cstdlib>
#include <memory>
#include <locale>
#include <ctime>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <regex>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <functional>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <iterator>
#include <algorithm>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using boost::asio::ip::udp;

class PTimer
{
    std::chrono::steady_clock::time_point start_;
public:
    PTimer()
    :start_( std::chrono::steady_clock::now() )
    {
        
    }
    double elapse()
    {
        std::chrono::duration<double> sec = std::chrono::steady_clock::now() - start_;
        return sec.count();
    }
    void reset()
    {
        start_ = std::chrono::steady_clock::now();
    }
};
const uint32_t hash2u32(const std::string &str);
const uint64_t hash2u64(const std::string &str);

constexpr uint32_t val_32_const = 0x811c9dc5;
constexpr uint32_t prime_32_const = 0x1000193;
constexpr uint64_t val_64_const = 0xcbf29ce484222325;
constexpr uint64_t prime_64_const = 0x100000001b3;
constexpr uint32_t hash2u32_const(const char *const str, const uint32_t value = val_32_const);
constexpr uint64_t hash2u64_const(const char *const str, const uint64_t value = val_64_const);