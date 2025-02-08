cc_binary(
    name = "game_server",
    srcs = ["main.cpp"],
    copts = ["-D_DEFAULT_SOURCE", "-D_XOPEN_SOURCE=600"],  # Manually include directories
    linkopts = ["-lnotcurses", "-lnotcurses-core", "-lnotcurses++"],  # Manually specify libraries
    deps = ["//common:game_utils", "//common:tile", "//common:board", ":server"],
)

cc_library(
    name = "server",
    hdrs = ["Server.hpp"],
    # deps = [":game_utils", ":tile"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)

