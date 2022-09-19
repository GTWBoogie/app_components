package(default_visibility = ["//visibility:public"])

cc_library(
    name = "iservice",
    hdrs = ["iservice.h"],
    deps = [
        "//util:stop_token",
    ],
)

cc_library(
    name = "ilogger",
    hdrs = ["ilogger.h"],
)

cc_library(
    name = "cout_logger",
    srcs = ["cout_logger.cpp"],
    hdrs = ["cout_logger.h"],
    deps = [
        "ilogger",
    ],
)

cc_library(
    name = "boost_logger",
    srcs = ["boost_logger.cpp"],
    hdrs = ["boost_logger.h"],
    deps = [
        "ilogger",
        "@boost//:log",
    ],
)

cc_library(
    name = "signal_handler",
    srcs = ["signal_handler.cpp"],
    hdrs = ["signal_handler.h"],
    deps = ["//util:stop_token"],
)
