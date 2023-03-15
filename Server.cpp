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

using namespace std;
using namespace Sync;


class serverThread: public Thread{
    public:
       // int sockNum = 0;
      Socket sock;
   
       serverThread(Socket cliSock):Thread(){
            this-> sock = cliSock;
           
        }


    virtual long ThreadMain(void) override{

        
        //std::cout << "In the thread" << std::endl;
        //Socket sock(sockNum);

     //  sock(serSock.Accept());
        //std::cout << "created client" << std::endl;
        
           //SocketServer serSock(3000);
    
    
    
    //waits till the client socket connects
    
    ByteArray message;
    while(true){

    //gets the message from the client
    std::cout << "The bytes of the message: "<< sock.Read(message)<<std::endl;
    std::cout << "The Message: " << message.ToString()<<std::endl;

    //alters the message
    string altMessage = message.ToString();
    for (auto & c: altMessage) c = toupper(c);
    if(altMessage == "DONE")
    {
        break;
    }
    std::cout << "The altered Message: " << altMessage<<std::endl;

    sock.Write(altMessage);
    }

    

    return 0;

    }


    

};


int main(void)
{

    //Starts the sever  
    SocketServer serSock(3000);
    std::cout << "I am a server running on port 3000." << std::endl;

// creates the stack for all the threads
    serverThread* st;
    stack<serverThread*> tStack;
    
   // bool isWaiting = false;

    //Socket sock(serSock.Accept());
    //bool isconnected = true;
    while(true){
         Socket sock(serSock.Accept());
          std::cout << "The Client has connected" << std::endl;
       //std::cout<<listen(serSock.,5) <<" "<<std::endl;
           // sock.Close();
         st = new serverThread(sock);
         tStack.push(st);
          //  sock.Close(); 
        


    }




//std::cout << "The message: "<< message.ToString()<<std::endl;


}