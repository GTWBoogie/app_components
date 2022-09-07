package(default_visibility = ["//visibility:public"])

cc_library(
    name = "component_registry",
    srcs = [
        "component_converter.cpp",
        "component_creator.cpp",
        "component_description.cpp",
        "component_provider.cpp",
    ],
    hdrs = [
        "component_converter.h",
        "component_creator.h",
        "component_description.h",
        "component_instance.h",
        "component_provider.h",
        "component_registry.h",
        "lifetime.h",
    ],
    deps = [
        "//util:tuple",
        "@boost//:callable_traits",
        "@boost//:core",
    ],
)

cc_library(
    name = "application",
    srcs = ["application.cpp"],
    hdrs = [
        "application.h",
        "iapplication.h",
        "iservice.h",
    ],
    deps = [
        ":component_registry",
        "//util:stop_token",
    ],
)