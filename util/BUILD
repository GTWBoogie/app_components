package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tuple",
    hdrs = ["tuple.h"],
)

cc_library(
    name = "stop_token",
    srcs = ["stop_token.cpp"],
    hdrs = ["stop_token.h"],
)

cc_library(
    name = "demangle",
    srcs = ["demangle.cpp"],
    hdrs = ["demangle.h"],
    deps = ["@boost//:core"],
)
