#package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lifetime",
    hdrs = ["lifetime.h"]
)

cc_library(
    name = "instance",
    hdrs = ["instance.h"],
    deps = [":lifetime"]
)

cc_library(
    name = "converter",
    srcs = ["converter.cpp"],
    hdrs = ["converter.h"]
)

cc_library(
    name = "creator",
    srcs = ["creator.cpp"],
    hdrs = ["creator.h"],
    deps = [":instance"]
)

cc_library(
    name = "description",
    srcs = ["description.cpp"],
    hdrs = ["description.h"],
    deps = [
        ":creator",
        ":converter",
    ]
)

cc_library(
    name = "component_registry",
    srcs = [
        "provider.cpp",
    ],
    hdrs = [
        "provider.h",
        "registry.h",
    ],
    deps = [
        ":description",
        "//util:tuple",
        "@boost//:callable_traits",
        "@boost//:core",
    ],
    visibility = ["//visibility:public"]
)
