load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "application",
    srcs = ["application.cpp"],
    hdrs = [
        "application.h",
        "iapplication.h",
    ],
    deps = [
        "//components:cout_logger",
        "//components:iservice",
        "//components:signal_handler",
        "//registry",
        "//util:stop_token",
    ],
)
