cc_binary(
    name = "game_board",
    srcs = ["main.cpp"],
    copts = ["-D_DEFAULT_SOURCE", "-D_XOPEN_SOURCE=600"],  # Manually include directories
    linkopts = ["-lnotcurses", "-lnotcurses-core", "-lnotcurses++"],  # Manually specify libraries
    deps = [":game_utils", ":tile", ":board"],
)

cc_library(
    name = "game_utils",
    srcs = ["GameUtils.cpp"],
    hdrs = ["GameUtils.hpp"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)

cc_library(
    name = "tile",
    srcs = ["Tile.cpp"],
    hdrs = ["Tile.hpp"],
    deps = [":game_utils"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)

cc_library(
    name = "board",
    hdrs = ["Board.hpp"],
    deps = [":game_utils"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)

cc_library(
    name = "message",
    srcs = ["Message.cpp"],
    hdrs = ["Message.hpp"],
    deps = [":game_utils"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)

cc_library(
    name = "message_serializer",
    srcs = ["MessageSerializer.cpp"],
    hdrs = ["MessageSerializer.hpp"],
    deps = [":game_utils", ":tile"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)

cc_library(
    name = "message_deserializer",
    srcs = ["MessageDeserializer.cpp"],
    hdrs = ["MessageDeserializer.hpp"],
    deps = [":game_utils", ":tile", ":message"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)
