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
    name = "registry_base",
    srcs = ["registry_base.cpp"],
    hdrs = ["registry_base.h"],
    deps = [
        ":description",
        "//util:debug_log",
        "//util:demangle",
    ],
)

cc_library(
    name = "provider_base",
    srcs = ["provider_base.cpp"],
    hdrs = ["provider_base.h"],
    deps = [":registry_base"],
)

cc_library(
    name = "tagged_type",
    hdrs = ["tagged_type.h"],
)

cc_library(
    name = "converters",
    hdrs = ["converters.h"],
    deps = [
        ":converter",
        ":tagged_type",
        "//util:demangle",
    ],
)

cc_library(
    name = "creators",
    hdrs = ["creators.h"],
    deps = [
        ":creator",
        ":provider_base",
        "@boost//:callable_traits",
    ],
)

cc_library(
    name = "provider",
    srcs = ["provider.cpp"],
    hdrs = ["provider.h"],
    deps = [
        ":converters",
        ":creators",
    ],
)

cc_library(
    name = "registry",
    srcs = [
        "dependencies.cpp",
        "dependencies.h",
        "detail.h",
    ],
    hdrs = [
        "registry.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":provider",
        "//util:tuple",
    ],
)

cc_test(
    name = "instance_creation_test",
    srcs = ["instance_creation_test.cpp"],
    deps = [":registry"],
)

cc_test(
    name = "instance_scope_test",
    srcs = ["instance_scope_test.cpp"],
    deps = [":registry"],
)

cc_test(
    name = "dependency_injection_test",
    srcs = ["dependency_injection_test.cpp"],
    deps = [":registry"],
)
