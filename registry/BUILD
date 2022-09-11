cc_library(
    name = "lifetime",
    hdrs = ["lifetime.h"],
)

cc_library(
    name = "instance",
    hdrs = ["instance.h"],
)

cc_library(
    name = "converter",
    hdrs = ["converter.h"],
)

cc_library(
    name = "creator",
    hdrs = ["creator.h"],
    deps = [":instance"],
)

cc_library(
    name = "description",
    hdrs = ["description.h"],
    deps = [
        ":converter",
        ":creator",
        ":lifetime",
    ],
)

cc_library(
    name = "registry",
    srcs = [
        "detail.h",
        "provider.cpp",
    ],
    hdrs = [
        "provider.h",
        "registry.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":description",
        "//util:debug_log",
        "//util:demangle",
        "//util:tuple",
        "@boost//:callable_traits",
    ],
)
