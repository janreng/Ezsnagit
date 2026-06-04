// Test headless cho ezsnag_core. Chỉ in ASCII. Thiết lập harness cho cả dự án.
#include "core/Version.h"
#include <QString>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using core::Version;

int main() {
    // parse cơ bản
    {
        Version v = Version::parse("1.2.3");
        ok(v.major == 1 && v.minor == 2 && v.patch == 3, "parse 1.2.3");
    }
    // bỏ tiền tố v + phần thiếu = 0
    {
        Version v = Version::parse("v2.0");
        ok(v.major == 2 && v.minor == 0 && v.patch == 0, "parse v2.0 -> 2.0.0");
    }
    // rỗng -> 0.0.0
    {
        Version v = Version::parse("");
        ok(v.major == 0 && v.minor == 0 && v.patch == 0, "parse rong -> 0.0.0");
    }
    // toString round-trip
    {
        ok(Version::parse("3.4.5").toString() == "3.4.5", "toString 3.4.5");
    }
    // compare
    {
        ok(core::compare(Version::parse("1.0.0"), Version::parse("1.0.1")) < 0, "1.0.0 < 1.0.1");
        ok(core::compare(Version::parse("1.2.0"), Version::parse("1.1.9")) > 0, "1.2.0 > 1.1.9");
        ok(core::compare(Version::parse("2.3.4"), Version::parse("2.3.4")) == 0, "2.3.4 == 2.3.4");
        ok(core::compare(Version::parse("10.0.0"), Version::parse("9.9.9")) > 0, "10.0.0 > 9.9.9 (so so, khong lex)");
    }
    // appVersion không rỗng
    {
        ok(!core::appVersion().isEmpty(), "appVersion khong rong");
    }

    std::printf("test_core: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
