#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <ncpp/NotCurses.hh>
#include <ncpp/Menu.hh>
#include <ncpp/Pile.hh>
#include <ncpp/Plane.hh>
#include <ncpp/Reel.hh>
#include <ncpp/MultiSelector.hh>
#include <ncpp/Selector.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Direct.hh>
#include <ncpp/Plot.hh>
#include <ncpp/FDPlane.hh>
#include <ncpp/Subproc.hh>
#include <ncpp/Progbar.hh>

#include <iostream>

#include "Server.hpp"

#define PORT 8080

int run(){
    notcurses_options nopts{};
    // nopts.flags = NCOPTION_DRAIN_INPUT;
    ncpp::NotCurses nc(nopts);
    std::shared_ptr<ncpp::Plane> stdplane(nc.get_stdplane());

    Game::Server<5, 5> server(0, PORT);

    while(true){
        int event = server.poll();

        if(event < 0){
            std::cerr << "[Error] Poll error. Exiting...\n";
            break;
        }

        int32_t idx = server.poll_events(POLLIN);

        if(idx == 0){
            server.process_connection();
        }
        else if(idx > 0){
            std::cerr << "[Info] Received message from client id " << idx << '\n';
            server.process_message(idx);
        }

        // Otherwise, continue

    }

    return EXIT_SUCCESS;
}

int main(){
    int errLog = open("error.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(errLog, STDERR_FILENO);
    close(errLog);

    return run();
}
