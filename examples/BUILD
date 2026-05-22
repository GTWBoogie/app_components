load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "printer_service",
    srcs = ["printer_service.cpp"],
    deps = [
        "//application",
        "//components:boost_logger",
    ],
)
