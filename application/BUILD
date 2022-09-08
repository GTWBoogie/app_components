package(default_visibility = ["//visibility:public"])

cc_library(
    name = "application",
    srcs = ["application.cpp"],
    hdrs = [
        "application.h",
        "iapplication.h",
        "iservice.h",
    ],
    deps = [
        "//dependency:component_registry",
        "//util:stop_token",
    ],
)
