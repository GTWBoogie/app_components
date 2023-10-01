workspace(name = "gtw_boogie_app_components")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#-----------------------------------------------------------------------------
# Boost libraries

_RULES_BOOST_COMMIT = "1b73fb2bbbaed530f85db89eb2373c7b2e2d0ac5"

http_archive(
    name = "com_github_nelhage_rules_boost",
    sha256 = "407812a1d32f25810c98d00c2d9c94e73dbdba09707604e423b19ae05ab45cca",
    strip_prefix = "rules_boost-%s" % _RULES_BOOST_COMMIT,
    urls = [
        "https://github.com/nelhage/rules_boost/archive/%s.tar.gz" % _RULES_BOOST_COMMIT,
    ],
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

#-----------------------------------------------------------------------------
# Compile database support

http_archive(
    name = "hedron_compile_commands",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/e16062717d9b098c3c2ac95717d2b3e661c50608.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-e16062717d9b098c3c2ac95717d2b3e661c50608",
    sha256 = "ed5aea1dc87856aa2029cb6940a51511557c5cac3dbbcb05a4abd989862c36b4",
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()

