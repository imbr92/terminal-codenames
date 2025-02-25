cc_binary(
    name = "game_client",
    srcs = ["main.cpp"],
    copts = ["-D_DEFAULT_SOURCE", "-D_XOPEN_SOURCE=600"],  # Manually include directories
    linkopts = ["-lnotcurses", "-lnotcurses-core", "-lnotcurses++"],  # Manually specify libraries
    deps = [":client"],
)

cc_library(
    name = "client",
    hdrs = ["Client.hpp"],
    deps = ["//common:board", "//common:message", "//common:message_serializer", "//common:message_deserializer"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)

