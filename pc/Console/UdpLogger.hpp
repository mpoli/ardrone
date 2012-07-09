#include <boost/asio.hpp>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <fstream>
using boost::uint16_t;
using std::ofstream;

class UdpLogger {
  ofstream log_;
  boost::array<char, 1024> recvBuffer_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  boost::asio::ip::udp::socket socket_;
public:
  UdpLogger(boost::asio::io_service& ios, uint16_t port, const string& logfile) : socket_(ios, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
  {
    log_.open(logfile.c_str(), std::ios::app);
  }
  void start()
  {
    startReceive();
  }
  void stop()
  {
    socket_.close();
  }
private:
  void startReceive() {
    socket_.async_receive_from(boost::asio::buffer(recvBuffer_), sender_endpoint_, 
      boost::bind(&UdpLogger::handleReceive, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  }
  void handleReceive(const boost::system::error_code& error, size_t bytesReceived) {
    if (!error) {
      log_ << string(recvBuffer_.begin(), recvBuffer_.begin()+bytesReceived) << std::endl;
      startReceive();
    }
  }
};
