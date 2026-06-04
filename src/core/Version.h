#pragma once
#include <QString>

// Phiên bản app + so sánh semver (dùng cho auto-update sau này; logic thuần, test headless).
// Tái dùng pattern VersionCompare của Ezcel C++.
namespace core {

struct Version {
    int major = 0, minor = 0, patch = 0;

    // Parse "1.2.3" (bỏ tiền tố 'v' nếu có). Phần thiếu = 0. Trả Version{0,0,0} nếu rỗng.
    static Version parse(const QString &s);
    QString toString() const;
};

// So sánh semver: <0 nếu a<b, 0 nếu bằng, >0 nếu a>b.
int compare(const Version &a, const Version &b);

// Phiên bản app hiện tại (từ define EZSNAGIT_VERSION lúc build; fallback "0.0.0").
QString appVersion();

} // namespace core
