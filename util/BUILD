package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tuple",
    hdrs = ["tuple.h"],
)

cc_test(
    name = "tuple_test",
    srcs = ["tuple_test.cpp"],
    deps = [
        ":tuple",
        "@boost//:test",
    ],
)

cc_library(
    name = "stop_token",
    srcs = ["stop_token.cpp"],
    hdrs = ["stop_token.h"],
)

cc_test(
    name = "stop_token_test",
    srcs = ["stop_token_test.cpp"],
    deps = [
        ":stop_token",
        "@boost//:test",
    ],
)

cc_library(
    name = "demangle",
    srcs = ["demangle.cpp"],
    hdrs = ["demangle.h"],
    deps = ["@boost//:core"],
)

cc_test(
    name = "demangle_test",
    srcs = ["demangle_test.cpp"],
    deps = [
        ":demangle",
        "@boost//:test",
    ],
)

cc_library(
    name = "debug_log",
    hdrs = ["debug_log.h"],
)

cc_library(
    name = "lockless_lifo_queue",
    srcs = ["lockless_lifo_queue.h"],
)

cc_test(
    name = "lockless_lifo_queue_test",
    srcs = ["lockless_lifo_queue_test.cpp"],
    deps = [":lockless_lifo_queue"],
)
