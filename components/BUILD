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
    name = "boost_logger",
    srcs = ["boost_logger.cpp"],
    hdrs = ["boost_logger.h"],
    deps = [
        "ilogger",
        "@boost//:log",
    ],
)
