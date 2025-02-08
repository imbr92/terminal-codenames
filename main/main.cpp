#include <unistd.h>
#include <fcntl.h>

#include <locale.h>
#include <langinfo.h>
#include <pthread.h>
#include <signal.h>
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

#include "Board.hpp"

void mask_signals(){
    sigset_t mask;
    sigemptyset(&mask);

    // Add signals to block (e.g., SIGWINCH)
    sigaddset(&mask, SIGWINCH);

    // Block signals for the current thread
    if(pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0){
        perror("Failed to mask signals");
        exit(EXIT_FAILURE);
    }
}

constexpr int64_t DELAY = 1;

void center_text(ncpp::Plane &plane, const std::string &text) {
    size_t height = plane.get_dim_y();
    size_t width = plane.get_dim_x();

    size_t x = (width - text.length()) / 2;
    size_t y = height / 2;

    plane.putstr(y, x, text.c_str());
}

int run(){
    notcurses_options nopts{};
    // nopts.flags = NCOPTION_DRAIN_INPUT;
    ncpp::NotCurses nc(nopts);
    std::shared_ptr<ncpp::Plane> stdplane(nc.get_stdplane());

    stdplane->set_fg_rgb8(0x80, 0xc0, 0x80);
    stdplane->set_bg_rgb8(0x00, 0x40, 0x00);
    stdplane->putstr("Among us");

    Game::Board<5, 5> board(0);

    // TODO: Remove
    board.color_test();
    nc.render();

    ncinput ni;
    while (true) {
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
            board.update_position(0, -1);
        } else if(ni.id == 'l' || ni.id == NCKEY_RIGHT){
            board.update_position(0, 1);
        } else if(ni.id == 'j' || ni.id == NCKEY_DOWN){
            board.update_position(1, 0);
        } else if(ni.id == 'k' || NCKEY_UP){
            board.update_position(-1, 0);
        } else if(ni.id == NCKEY_ENTER){
            board.select();
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

    if (!setlocale(LC_ALL, "")){
        std::cerr << "Failed to set locale\nExiting...\n";
        return EXIT_FAILURE;
    }

    std::string encoding = std::string(nl_langinfo(CODESET));
    if(encoding != "UTF-8"){
        std::cerr << "Program may not function properly since locale encoding is " << encoding << " instead of UTF-8\n";
    }

    // TODO: Figure out if this is actually necessary
    mask_signals();

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
