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

    Game::Client<5, 5> client(SERVER_IP, PORT, stdplane);

    nc.render();
    client.draw();


    unsigned dimy, dimx;
    nc.get_term_dim(&dimy, &dimx);
    ncreader_options opts{};
    bool horscroll = false;
    opts.flags = NCREADER_OPTION_CURSOR;
    // can't use Plane until we have move constructor for Reader
    struct ncplane_options npopts = {
      .y = 32,
      .x = 2,
      .rows = 4,
      .cols = 105,
      .userptr = nullptr,
      .name = "read",
      .resizecb = nullptr,
      .flags = 0,
      .margin_b = 0, .margin_r = 0,
    };
    ncinput ni;
    /*(*n)->printf(0, 0, "Scroll: %c Cursor: 000/000 Viewgeom: %03u/%03u Textgeom: %03u/%03u",*/
                 /*horscroll ? '+' : '-', vgeomy, vgeomx, tgeomy, tgeomx);*/
    nc.render();

    while(true){
        int ret = client.poll(0);

        if(ret < 0){
            std::cerr << "[Error] Failed to poll\n";
            exit(EXIT_FAILURE);
        }

        if(client.poll_events(POLLIN)){
            std::cerr << "[Info] Received message\n";
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
        } else if(ni.id == 'k' || ni.id == NCKEY_UP){
            client.update_position(-1, 0);
        } else if(ni.id == NCKEY_ENTER){
            client.guess_tile();
        } else if(ni.id == 's'){
            std::cerr << "[Info] Sent game start\n";
            client.start_game();
        }

        // if(ni.id == NCKEY_ENTER && client.allow_text_input()){ // Enter text box section
        if(ni.id == 'i'){ // Enter text box section
            struct ncplane* rp = ncplane_create(*stdplane, &npopts);
            ncplane_set_base(rp, "░", 0, 0);
            auto nr = ncreader_create(rp, &opts);
            if(nr == nullptr){
              return EXIT_FAILURE;
            }
            unsigned tgeomy, tgeomx, vgeomy, vgeomx;
            struct ncplane* ncp = ncreader_plane(nr);
            struct ncplane* tplane = ncplane_above(ncp);
            ncplane_dim_yx(tplane, &tgeomy, &tgeomx);
            ncplane_dim_yx(ncp, &vgeomy, &vgeomx);

            while(nc.get(true, &ni) != (char32_t)-1){
                if(ni.evtype == ncpp::EvType::Release){
                    continue;
                }
                if(ni.ctrl && ni.id == 'L'){
                    notcurses_refresh(nc, NULL, NULL);
                }else if((ni.ctrl && ni.id == 'D') || ni.id == NCKEY_ENTER){
                    break;
                }else if(ncreader_offer_input(nr, &ni)){
                    unsigned ncpy, ncpx;
                    ncplane_cursor_yx(ncp, &ncpy, &ncpx);
                    ncplane_dim_yx(tplane, &tgeomy, &tgeomx);
                    ncplane_dim_yx(ncp, &vgeomy, &vgeomx);
                    // (*n)->printf(0, 0, "Scroll: %c Cursor: %03u/%03u Viewgeom: %03u/%03u Textgeom: %03u/%03u",
                    // horscroll ? '+' : '-', ncpy, ncpx, vgeomy, vgeomx, tgeomy, tgeomx);
                    nc.render();
                }
            }
            nc.render();
            char* contents;
            ncreader_destroy(nr, &contents);

            // nc.stop();
            if(contents){
                stdplane->printf(32, 110, "input: %s\n", contents);
                client.send_clue(contents);
            }
        }

        client.draw();

        // Print pressed key (Probably can remove)
        // stdplane->printf(1, 110, "Key: %lc (%d)", key, key);
        // stdplane->printf(10, 110, "Ctrl: %d Shift: %d", ni.ctrl, ni.shift);
        nc.render();
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]){
    std::string log_file = "error.log";
    if(argc > 1){
        log_file = argv[1];
    }
    int errLog = open(log_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
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
