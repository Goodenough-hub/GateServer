#include "CServer.h"

int main()
{
    try {
        unsigned short port = static_cast<unsigned short>(8080);

        // I/O 上下文对象、负责协调异步操作的执行，包括网络套接字、定时器等。它提供了事件循环机制，用于处理异步事件和回调函数。
        net::io_context ioc{ 1 };  // 1 意味着将使用一个工作线程来执行 I/O 操作。

        // signal_set 用于异步地等待和处理来自操作系统的信号.  SIGINT--中断信号，用户Ctrl+C   SIGTERM--终止信号，
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM); // 创建一个 signal_set，等待 SIGINT 和 SIGTERM 信号
        
        // async_wait用于异步等待I/O对象操作完成的成员函数。允许注册一个回调函数，当指定的I/O对象达到某个条件时，调用回调函数。
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) { // 信号到达时，回调函数将被调用
            if (error) {
                return;
            }
            // 如果没有错误，停止io_context的事件循环。
            ioc.stop();
            });

        // 创建了一个指向CServer对象的智能指针，并使用ioc和port作为参数初始化该对象。
        std::make_shared<CServer>(ioc, port)->Start();
        std::cout << "Gate Server listen on port: " << port << std::endl;
        ioc.run(); // 运行 I/O 上下文的事件循环
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}