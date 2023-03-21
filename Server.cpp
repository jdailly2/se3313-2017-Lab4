#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <iostream>
#include <cstring>
#include <stack>
#include "socket.h"
#include <csignal>

using namespace std;
using namespace Sync;


class SocketThread: public Thread{
    public:
      bool isConnected;
      bool endServer = false;
      Socket& socket;

      ~SocketThread()
    {}
   
       SocketThread(Socket& socket)
    : socket(socket) {
        this->isConnected = true;
    }

        Socket& GetSocket()
    {
        return socket;
    }

    virtual long ThreadMain(void) override{
        ByteArray message;

        while(!endServer){
            int bytes = socket.Read(message);
            if (bytes == 0) {
                isConnected = false;
                break;
            }

            //gets the message from the client
            std::cout << "The bytes of the message: "<< bytes<<std::endl;
            std::cout << "The Message: " << message.ToString()<<std::endl;

            //alters the message
            string altMessage = message.ToString();
            for (auto & c: altMessage) c = toupper(c);
            std::cout << "The altered Message: " << altMessage<<std::endl;

            socket.Write(altMessage);
        }
        socket.Close();
        return 0;
        }
};

class ServerThread : public Thread{
    public:
    SocketServer& serSock;
    ServerThread(SocketServer& serSock) : serSock(serSock){}
    bool endServer = false;
    stack<SocketThread*> tStack;

    ~ServerThread() {
        while(!tStack.empty()){
            SocketThread* finishedThread = tStack.top();
            Socket& socket = finishedThread->GetSocket();
            socket.Close();
            finishedThread -> endServer = true;
            tStack.pop();
        }
    }

    virtual long ThreadMain()
    {
        while(!endServer){
            try {
                Socket* newSocket = new Socket(serSock.Accept());
                std::cout << "The Client has connected" << std::endl;
                Socket& socketReference = *newSocket;
                tStack.push(new SocketThread(socketReference));
            } catch(...) {
                endServer = true;
            }
        }
    }

};

SocketServer* pSocketServer = nullptr;
void sigint_handler(int sig)
{
    //Clean up if terminated using control+c
    std::cout << "Shutting Down Server." << std::endl;
    if (pSocketServer) {
        pSocketServer->Shutdown();
    }
    exit(0);
}


int main(void)
{
    signal(SIGINT, sigint_handler);
    std::cout << "I am a server." << std::endl;
	std::cout << "Press enter to terminate the server...\n";
    std::cout.flush();

    //Starts the sever  
    SocketServer serSock(3000);
    pSocketServer = &serSock;

    std::cout << "I am a server running on port 3000." << std::endl;

    ServerThread serverThread(serSock);

    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    std::cin.get();

    std::cout << "Shutting Down Server." << std::endl;
    serSock.Shutdown();
    return 0;
}