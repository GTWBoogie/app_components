workspace(name = "gtw_boogie_app_components")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#-----------------------------------------------------------------------------
# LLVM toolchain

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

BAZEL_TOOLCHAIN_TAG = "0.7.2"
BAZEL_TOOLCHAIN_SHA = "f7aa8e59c9d3cafde6edb372d9bd25fb4ee7293ab20b916d867cd0baaa642529"

http_archive(
   name = "com_grail_bazel_toolchain",
   sha256 = BAZEL_TOOLCHAIN_SHA,
   strip_prefix = "bazel-toolchain-{tag}".format(tag = BAZEL_TOOLCHAIN_TAG),
   canonical_id = BAZEL_TOOLCHAIN_TAG,
   url = "https://github.com/grailbio/bazel-toolchain/archive/{tag}.tar.gz".format(tag = BAZEL_TOOLCHAIN_TAG),
)

load("@com_grail_bazel_toolchain//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@com_grail_bazel_toolchain//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
   name = "llvm_toolchain",
   llvm_version = "14.0.0",
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

#-----------------------------------------------------------------------------
# Boost libraries

_RULES_BOOST_COMMIT = "652b21e35e4eeed5579e696da0facbe8dba52b1f"

http_archive(
  name = "com_github_nelhage_rules_boost",
  sha256 = "c1b8b2adc3b4201683cf94dda7eef3fc0f4f4c0ea5caa3ed3feffe07e1fb5b15",
  strip_prefix = "rules_boost-%s" % _RULES_BOOST_COMMIT,
  urls = [
    "https://github.com/nelhage/rules_boost/archive/%s.tar.gz" % _RULES_BOOST_COMMIT,
  ],
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

