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
        "converters.h",
        "creators.h",
        "detail.h",
        "provider.cpp",
        "registry_base.cpp",
        "registry_base.h",
    ],
    hdrs = [
        "provider.h",
        "registry.h",
        "tagged_type.h",
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
