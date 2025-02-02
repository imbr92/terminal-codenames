cc_binary(
    name = "game_board",
    srcs = ["main.cpp"],
    copts = ["-D_DEFAULT_SOURCE", "-D_XOPEN_SOURCE=600"],  # Manually include directories
    linkopts = ["-lnotcurses", "-lnotcurses-core", "-lnotcurses++"],  # Manually specify libraries
)

