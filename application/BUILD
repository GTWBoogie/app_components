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
        "//registry",
        "//util:stop_token",
    ],
)
