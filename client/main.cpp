#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <pthread.h>
#include <iostream>

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

#include "Client.hpp"

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int run(){

    notcurses_options nopts{};
    // nopts.flags = NCOPTION_DRAIN_INPUT;
    ncpp::NotCurses nc(nopts);
    std::shared_ptr<ncpp::Plane> stdplane(nc.get_stdplane());

    Game::Client<5, 5> client(SERVER_IP, PORT);

    ncinput ni;
    nc.render();
    client.draw();

    while(true){
        int ret = client.poll(0);

        if(ret < 0){
            std::cerr << "[Error] Failed to poll\n";
            exit(EXIT_FAILURE);
        }

        if(client.poll_events(POLLIN)){
            client.process_message();
        }

        char32_t key = nc.get(true, &ni);  // Wait for input

        if(key == (char32_t)-1){  // No event (should not happen with blocking `true`)
            continue;
        }

        std::cerr << key << '\n';

        // Ignore key releases
        if(ni.evtype == ncpp::EvType::Release){
            continue;
        }

        // TODO: figure out how to get this working
        if(ni.ctrl){  // Check if Ctrl is pressed
            std::cerr << "got ctrl'd\n";
            if(ni.id == 'q'){  // Ctrl+Q -> Exit loop
                break;
            } else if(ni.id == 'l'){  // Ctrl+L -> Refresh screen
                std::cerr << "refreshed\n";
                nc.render();
            }
            continue;
        }

        // TODO: maybe move to switch or something
        if(ni.id == 'h' || ni.id == NCKEY_LEFT){
            client.update_position(0, -1);
        } else if(ni.id == 'l' || ni.id == NCKEY_RIGHT){
            client.update_position(0, 1);
        } else if(ni.id == 'j' || ni.id == NCKEY_DOWN){
            client.update_position(1, 0);
        } else if(ni.id == 'k' || NCKEY_UP){
            client.update_position(-1, 0);
        } else if(ni.id == NCKEY_ENTER){
            client.select();
        }

        // Print pressed key
        stdplane->printf(1, 0, "Key: %lc (%d)", key, key);
        stdplane->printf(10, 0, "Ctrl: %d Shift: %d", ni.ctrl, ni.shift);
        nc.render();
    }

    return EXIT_SUCCESS;
}

int main(){

    int errLog = open("error.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(errLog, STDERR_FILENO);
    close(errLog);

    try {
        return run();
    } catch (ncpp::init_error &e) {
        std::cerr << "Initialization error: " << e.what () << std::endl;
    } catch (ncpp::invalid_state_error &e) {
        std::cerr << "Invalid state error: " << e.what () << std::endl;
    } catch (ncpp::invalid_argument &e) {
        std::cerr << "Invalid argument error: " << e.what () << std::endl;
    }

    return 0;
}
