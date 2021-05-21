#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdint>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

// Message

// Message Header is sent at start of all messages
template <typename T>
struct message_header
{
    T id{};
    uint32_t size = 0;
};

// Message Body contains a header and a std::vector of bytes
template <typename T>
struct message
{
    message_header<T> header{};
    std::vector<uint8_t> body;

    // returns size of the body of message packet in bytes
    size_t size() const
    {
        return body.size();
    }

    // Override for std::cout compatibility - produces friendly description of message
    friend std::ostream &operator<<(std::ostream &os, const message<T> &msg)
    {
        os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
        return os;
    }

    // Pushes any POD(Plain Old Data)-like data into the message buffer
    template <typename DataType>
    friend message<T> &operator<<(message<T> &msg, const DataType &data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t i = msg.body.size();

        msg.body.resize(i + sizeof(DataType));

        std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

        msg.header.size = msg.size();

        // To allow chaining of the operator
        return msg;
    }

    // Pulls any POD-like data form the message buffer
    template <typename DataType>
    friend message<T> &operator>>(message<T> &msg, DataType &data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

        size_t i = msg.body.size() - sizeof(DataType);

        std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

        msg.body.resize(i);

        msg.header.size = msg.size();

        // To allow chaining of the operator
        return msg;
    }
};


template <typename T>
class connection;

// On a server, given a message we need to know which client sent it. So an "owned" message contains a pointer to the connection which it came through
template <typename T>
struct owned_message
{
    std::shared_ptr<connection<T>> remote = nullptr;
    message<T> msg;

    friend std::ostream &operator<<(std::ostream &os, const owned_message<T> &msg)
    {
        os << msg.msg;
        return os;
    }
};

// Making a Thread Safe Queue since multiple threads may want to access/modify it at any point
template <typename T>
class tsqueue
{
public:
    tsqueue() = default;
    tsqueue(const tsqueue<T> &) = delete;
    virtual ~tsqueue() { clear(); }

public:
    const T &front();

    const T &back();

    T pop_front();

    T pop_back();

    void push_back(const T &item);

    void push_front(const T &item);

    bool empty();

    size_t count();

    void clear();

    void wait();

protected:
    std::mutex muxQueue;
    std::deque<T> deqQueue;
    std::condition_variable cvBlocking;
    std::mutex muxBlocking;
};

template <typename T>
class server_interface;

template <typename T>
class connection : public std::enable_shared_from_this<connection<T>>
{
public:
    enum class owner
    {
        server,
        client
    };

public:
    connection(owner parent, asio::io_context &asioContext, asio::ip::tcp::socket socket, tsqueue<owned_message<T>> &qIn)
        : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn)
    {
        m_OwnerType = parent;

        switch(m_OwnerType)
        {
            case owner::client :
            {
                m_nHandshakeIn = 0;
                m_nHandshakeOut = 0;
                break;
            }
            case owner::server :
            {
                m_nHandshakeOut = uint64_t(time(0));

                m_nHandshakeCheck = scramble(m_nHandshakeOut);
                break;
            }
        }
    }

    virtual ~connection()
    {
    }

    uint32_t GetID() const
    {
        return id;
    }

public:
    void ConnectToClient(server_interface<T> *server, uint32_t uid = 0)
    {
        switch(m_OwnerType)
        {
            case owner::server :
            {
                if (m_socket.is_open())
                {
                    id = uid;

                    WriteValidation();

                    ReadValidation(server);
                }
                break;
            }

            default:
                break;
        }
    }

    void ConnectToServer(const asio::ip::tcp::resolver::results_type &endpoints)
    {
        if (m_OwnerType == owner::client)
        {
            asio::async_connect(m_socket, endpoints,
                                [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
                                    if (!ec)
                                    {
                                        ReadValidation();
                                    }
                                });
        }
    }

    void Disconnect()
    {
        if (IsConnected())
            asio::post(m_asioContext, [this]() { m_socket.close(); });
    }

    bool IsConnected() const
    {
        return m_socket.is_open();
    }

public:
    void Send(const message<T> &msg)
    {
        asio::post(m_asioContext,
                   [this, msg]() {
                       if(m_qMessagesOut.empty())
                       {
                           m_qMessagesOut.push_back(msg);
                           WriteHeader();
                       }
                       else
                       {
                           m_qMessagesOut.push_back(msg);
                       }
                   });
    }

private:
    void WriteHeader()
    {
        asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
                          [this](std::error_code ec, std::size_t length) {
                              if (!ec)
                              {
                                  if (m_qMessagesOut.front().body.size() > 0)
                                  {
                                      WriteBody();
                                  }
                                  else
                                  {
                                      m_qMessagesOut.pop_front();

                                      if (!m_qMessagesOut.empty())
                                      {
                                          WriteHeader();
                                      }
                                  }
                              }
                              else
                              {
                                  std::cout << "[" << id << "] Write Header Fail:" << ec.message() << "\n";
                                  m_socket.close();
                              }
                          });
    }

    void WriteBody()
    {
        asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
                          [this](std::error_code ec, std::size_t length) {
                              if (!ec)
                              {
                                  m_qMessagesOut.pop_front();

                                  if (!m_qMessagesOut.empty())
                                  {
                                      WriteHeader();
                                  }
                              }
                              else
                              {
                                  std::cout << "[" << id << "] Write Body Fail:" << ec.message() << "\n";
                                  m_socket.close();
                              }
                          });
    }

    void ReadHeader()
    {
        asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.header, sizeof(message_header<T>)),
                         [this](std::error_code ec, std::size_t length) {
                             if (!ec)
                             {
                                 if (m_msgTemporaryIn.header.size > 0)
                                 {
                                     m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
                                     ReadBody();
                                 }
                                 else
                                 {
                                     AddToIncomingMessageQueue();
                                 }
                             }
                             else
                             {
                                 std::cout << "[" << id << "] Read Header Fail:" << ec.message() << "\n";
                                 m_socket.close();
                             }
                         });
    }

    void ReadBody()
    {
        asio::async_read(m_socket, asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.body.size()),
                         [this](std::error_code ec, std::size_t length) {
                             if (!ec)
                             {
                                 AddToIncomingMessageQueue();
                             }
                             else
                             {
                                 std::cout << "[" << id << "] Read Body Fail : " << ec.message() << "\n";
                                 m_socket.close();
                             }
                         });
    }

    uint64_t scramble(uint64_t nInput)
    {
        uint64_t out = nInput ^ 0xDEADBEEFC0DECAFE;
        out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
        return out ^ 0xC0DEFACE12345678;
    }

    void WriteValidation()
    {
        asio::async_write(m_socket, asio::buffer(&m_nHandshakeOut, sizeof(uint64_t)),
                          [this](std::error_code ec, std::size_t length) {
                              if (!ec)
                              {
                                  if (m_OwnerType == owner::client)
                                      ReadHeader();
                              }
                              else
                              {
                                  m_socket.close();
                              }
                          });
    }

    void ReadValidation(server_interface<T> *server = nullptr)
    {
        asio::async_read(m_socket, asio::buffer(&m_nHandshakeIn, sizeof(uint64_t)),
                         [this, server](std::error_code ec, std::size_t length) {
                             if (!ec)
                             {
                                 switch(m_OwnerType)
                                 {
                                     case owner::client :
                                     {                                            
                                        m_nHandshakeOut = scramble(m_nHandshakeIn);

                                        WriteValidation();
                                         break;
                                     }

                                     case owner::server :
                                     {
                                         if (m_nHandshakeIn == m_nHandshakeCheck)
                                        {
                                            std::cout << "Client Validated" << std::endl;
                                            server->OnClientValidated(this->shared_from_this());

                                            ReadHeader();
                                        }
                                        else
                                        {
                                            std::cout << "Client Disconnected (Fail Validation)" << std::endl;
                                            m_socket.close();
                                        }
                                         break;
                                     }
                                 }                             }
                             else
                             {
                                 std::cout << "Client Disconnected (ReadValidation) : " << ec.message() << "\n";
                                 m_socket.close();
                             }
                         });
    }

    void AddToIncomingMessageQueue()
    {
        if (m_OwnerType == owner::server)
            m_qMessagesIn.push_back({this->shared_from_this(), m_msgTemporaryIn});
        else
            m_qMessagesIn.push_back({nullptr, m_msgTemporaryIn});

        ReadHeader();
    }

protected:
    asio::ip::tcp::socket m_socket;

    asio::io_context &m_asioContext;

    tsqueue<message<T>> m_qMessagesOut;

    tsqueue<owned_message<T>> &m_qMessagesIn;

    message<T> m_msgTemporaryIn;

    owner m_OwnerType = owner::server;

    uint64_t m_nHandshakeOut = 0;
    uint64_t m_nHandshakeIn = 0;
    uint64_t m_nHandshakeCheck = 0;

    uint32_t id = 0;
};

// Client
template <typename T>
class client_interface
{
public:
    client_interface()
    {
    }

    virtual ~client_interface()
    {
        Disconnect();
    }

public:
    bool Connect(const std::string &host, const uint16_t port)
    {
        try
        {
            asio::ip::tcp::resolver resolver(m_context);
            asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

            m_connection = std::make_unique<connection<T>>(connection<T>::owner::client, m_context, asio::ip::tcp::socket(m_context), m_qMessagesIn);

            m_connection->ConnectToServer(endpoints);

            thrContext = std::thread([this]() { m_context.run(); });
        }
        catch (std::exception &e)
        {
            std::cerr << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    void Disconnect()
    {
        if (IsConnected())
        {
            m_connection->Disconnect();
        }

        m_context.stop();
        if (thrContext.joinable())
            thrContext.join();

        m_connection.release();
    }

    bool IsConnected()
    {
        if (m_connection)
            return m_connection->IsConnected();
        else
            return false;
    }

public:
    void Send(const message<T> &msg)
    {
        if (IsConnected())
            m_connection->Send(msg);
    }

    tsqueue<owned_message<T>> &Incoming()
    {
        return m_qMessagesIn;
    }

protected:
    asio::io_context m_context;

    std::thread thrContext;

    std::unique_ptr<connection<T>> m_connection;

private:

    tsqueue<owned_message<T>> m_qMessagesIn;
};

// Server
template <typename T>
class server_interface
{
public:
    server_interface(uint16_t port)
        : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    }

    virtual ~server_interface()
    {
        Stop();
    }

    bool Start()
    {
        try
        {
            WaitForClientConnection();

            m_threadContext = std::thread([this]() { m_asioContext.run(); });
        }
        catch (std::exception &e)
        {
            std::cerr << "[SERVER] Error: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] Started!\n";
        return true;
    }

    void Stop()
    {
        m_asioContext.stop();

        if (m_threadContext.joinable())
            m_threadContext.join();

        std::cout << "[SERVER] Stopped!\n";
    }

    void WaitForClientConnection()
    {
        m_asioAcceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket) {
                if (!ec)
                {
                    std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

                    std::shared_ptr<connection<T>> newconn =
                        std::make_shared<connection<T>>(connection<T>::owner::server,
                                                        m_asioContext, std::move(socket), m_qMessagesIn);

                    
                    m_deqConnections.push_back(std::move(newconn));

                    m_deqConnections.back()->ConnectToClient(this, nIDCounter++);

                    std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n";
                    
                }
                else
                {
                    std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                }

                WaitForClientConnection();
            });
    }

    void MessageClient(std::shared_ptr<connection<T>> client, const message<T> &msg)
    {
        if (client && client->IsConnected())
        {
            client->Send(msg);
        }
        else
        {
            OnClientDisconnect(client);

            client.reset();

            m_deqConnections.erase(
                std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
        }
    }

    void MessageAllClients(const message<T> &msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
    {
        for (auto &client : m_deqConnections)
        {
            if (client && client->IsConnected())
            {
                if (client != pIgnoreClient)
                    client->Send(msg);
            }
            else
            {
                OnClientDisconnect(client);
                client.reset();
            }
        }
    }

    void Update(size_t nMaxMessages = -1, bool bWait = false)
    {
        if (bWait)
            m_qMessagesIn.wait();

        size_t nMessageCount = 0;
        while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
        {
            auto msg = m_qMessagesIn.pop_front();

            OnMessage(msg.remote, msg.msg);

            nMessageCount++;
        }
    }

protected:

    virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
    {
    }

    virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T> &msg)
    {
    }

public:

    virtual void OnClientValidated(std::shared_ptr<connection<T>> client)
    {
    }

protected:

    tsqueue<owned_message<T>> m_qMessagesIn;

    std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

    asio::io_context m_asioContext;
    std::thread m_threadContext;

    asio::ip::tcp::acceptor m_asioAcceptor;

    uint32_t nIDCounter = 10000;
};
