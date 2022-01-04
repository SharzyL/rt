#ifndef RT_DEBUG_H
#define RT_DEBUG_H

#include <Vector3f.h>

#include "fmt/core.h"
#include "fmt/ranges.h"
#include "glog/logging.h"

template <> struct fmt::formatter<Vector3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    template <typename FormatContext> auto format(const Vector3f &p, FormatContext &ctx) {
        return format_to(ctx.out(), "({:.3f}, {:.3f}, {:.3f})", p.x(), p.y(), p.z());
    }
};

#endif // RT_DEBUG_H
