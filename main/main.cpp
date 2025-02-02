#include <unistd.h>
#include <fcntl.h>

#include <locale.h>
#include <langinfo.h>
#include <pthread.h>
#include <signal.h>
#include <vector>
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

// dump two wide glyphs, then create a new plane and drop it atop them

auto stomper(ncpp::NotCurses& nc, std::shared_ptr<ncpp::Plane>& nn) -> int {
  // should knock out both wide glyphs
  nn->move(0, 1);
  nc.render();
  sleep(DELAY);

  nn->move(0, 2);
  nc.render();
  sleep(DELAY);

  nn->move(0, 3);
  nc.render();
  sleep(DELAY);

  nn->move(1, 3);
  nc.render();
  sleep(DELAY);

  nn->move(1, 2);
  nc.render();
  sleep(DELAY);

  nn->move(1, 1);
  nc.render();
  sleep(DELAY);
  return 0;
}

int run(){
    /*ncpp::NotCurses nc;*/
    /*ncpp::Plane* stdplane = nc.get_stdplane();*/
    /*auto x_dim = stdplane->get_dim_x();*/
    /*auto y_dim = stdplane->get_dim_y();*/
    /*auto x_pos = stdplane->get_x();*/
    /*auto y_pos = stdplane->get_y();*/
    /*std::cerr << "X Dim: " << x_dim << '\n';*/
    /*std::cerr << "Y Dim: " << y_dim << '\n';*/
    /*std::cerr << "X Pos: " << x_pos << '\n';*/
    /*std::cerr << "Y Pos: " << y_pos << '\n';*/
    /*ncpp::Plane p1 (1, 1, 0, 0);*/
    /*ncpp::Plot plot1 (p1);*/
    /*ncpp::Plane p2 (1, 1, 0, 0);*/
    /*ncpp::PlotU plot2 (p2);*/
    /*ncpp::Plane p3 (1, 1, 0, 0);*/
    /*ncpp::PlotD plot3 (p3);*/
    /*while(true) {nc.render();};*/
    /*nc.stop();*/
    const size_t MAX_WORD_SIZE = 15;
    const size_t BOARD_ROWS_TILE = 5;
    const size_t BOARD_COLS_TILE = 5;
    const size_t TILE_ROWS = 5;
    const size_t TILE_COLS = MAX_WORD_SIZE + 4;

    notcurses_options nopts{};
    nopts.flags = NCOPTION_DRAIN_INPUT;
    ncpp::NotCurses nc(nopts);
    std::shared_ptr<ncpp::Plane> stdplane(nc.get_stdplane());
    std::vector<std::vector<ncpp::Plane>> tiles;

    stdplane->set_fg_rgb8(0x80, 0xc0, 0x80);
    stdplane->set_bg_rgb8(0x00, 0x40, 0x00);
    stdplane->putstr("Among us");


    tiles.reserve(BOARD_COLS_TILE);
    size_t cx = 2, ry = 1;
    for(size_t i = 0; i < BOARD_ROWS_TILE; ++i){
        tiles.emplace_back();
        tiles.back().reserve(BOARD_ROWS_TILE);
        for(size_t i = 0; i < BOARD_COLS_TILE; ++i){
            tiles.back().emplace_back(TILE_ROWS, TILE_COLS, ry, cx);
            ncpp::Plane& cur_tile = tiles.back().back();
            cur_tile.set_fg_rgb8(0xc0, 0x80, 0xc0);
            cur_tile.set_bg_rgb8(0x20, 0x00, 0x20);
            cur_tile.set_base("", 0, NCCHANNELS_INITIALIZER(0xc0, 0x80, 0xc0, 0x20, 0, 0x20));
            cx += TILE_COLS + 2;
            nc.render();
        }
        cx = 2;
        ry += TILE_ROWS + 1;

    }
    nc.render();
    sleep(20);


    // first, a 2x1 with "AB"
    //          explicit Plane (Plane *n, int rows, int cols, int yoff, int xoff, void *opaque = nullptr)
    //          explicit Plane (unsigned rows, unsigned cols, int yoff, int xoff, void *opaque = nullptr, NotCurses *ncinst = nullptr)


    auto nn = std::make_shared<ncpp::Plane>(5, MAX_WORD_SIZE + 2, 10, 10);
    nn->set_fg_rgb8(0xc0, 0x80, 0xc0);
    nn->set_bg_rgb8(0x20, 0x00, 0x20);
    nn->set_base("", 0, NCCHANNELS_INITIALIZER(0xc0, 0x80, 0xc0, 0x20, 0, 0x20));
    /*nn->putstr("AB");*/
    /**/
    /*stdplane->set_fg_rgb8(0x80, 0xc0, 0x80);*/
    /*stdplane->set_bg_rgb8(0x00, 0x40, 0x00);*/
    /*stdplane->putstr("Among us");*/
    /*// stdplane->putstr("\xe5\xbd\xa2\xe5\x85\xa8");*/
    /*stdplane->putstr(1, 0, "\xe5\xbd\xa2\xe5\x85\xa8");*/
    /*stdplane->putstr(2, 0, "\xe5\xbd\xa2\xe5\x85\xa8");*/
    /*stdplane->putstr(3, 0, "\xe5\xbd\xa2\xe5\x85\xa8");*/
    /*stdplane->putstr(4, 0, "abcdef");*/
    /*stdplane->putstr(5, 0, "abcdef");*/
    /*stdplane->putstr(6, 0, "abcdef");*/
    /*stdplane->putstr(7, 0, "abcdef");*/
    nc.render();
    sleep(1);

    /*stomper(nc, nn);*/
    /*if(nn->putstr(0, 0, "\xe5\xbd\xa1") <= 0){*/
    /*  return EXIT_FAILURE;*/
    /*}*/
    /*stomper(nc, nn);*/
    /*nn->erase();*/
    /*if(nn->putstr(0, 0, "r") <= 0){*/
    /*  return EXIT_FAILURE;*/
    /*}*/
    /*stomper(nc, nn);*/

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
