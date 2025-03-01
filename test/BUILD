cc_test(
    name = "serialization_tests",
    srcs = ["SerializationTests.cpp"],
    copts = ["-D_DEFAULT_SOURCE", "-D_XOPEN_SOURCE=600"],  # Manually include directories
    linkopts = ["-lnotcurses", "-lnotcurses-core", "-lnotcurses++"],  # Manually specify libraries
    deps = [
        "//common:game_utils",
        "//common:tile",
        "//common:message",
        "//common:message_serializer",
        "//common:message_deserializer",
        ":catch"
    ],
)

cc_library(
    name = "catch",
    hdrs = ["catch_amalgamated.hpp"],
    srcs = ["catch_amalgamated.cpp"],
    visibility = ["//visibility:public"],  # Allows other targets to depend on it
)
