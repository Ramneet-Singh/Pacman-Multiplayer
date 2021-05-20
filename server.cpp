#include "include/Message.h"

class GameServer : public server_interface<GameMsg>
{
public:
    GameServer(uint16_t nPort) : server_interface<GameMsg>(nPort)
    {
        curPlayerID = 1;
    }

    int curPlayerID;

protected:
    bool OnClientConnect(std::shared_ptr<connection<GameMsg>> client) override
    {
        return true;
    }

    void OnClientValidated(std::shared_ptr<connection<GameMsg>> client) override
    {
        message<GameMsg> msg;
        msg.header.id = GameMsg::Client_AssignID;
        msg << curPlayerID++;
        client->Send(msg);
    }

    void OnClientDisconnect(std::shared_ptr<connection<GameMsg>> client) override
    {
        if (client)
        {
            curPlayerID = client->GetID() - 10000;
        }
    }

    void OnMessage(std::shared_ptr<connection<GameMsg>> client, message<GameMsg> &msg) override
    {
        switch (msg.header.id)
        {
        case GameMsg::Game_UserInput:
        {
            MessageAllClients(msg, client);
            break;
        }
        }
    }
};

int main()
{
    GameServer server(60000);
    server.Start();

    while (1)
    {
        server.Update(-1, true);
    }
    return 0;
}