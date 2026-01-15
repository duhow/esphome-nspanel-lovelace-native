#pragma once

namespace esphome {
namespace nspanel_lovelace {

#define M_PI 3.14159265358979323846 // pi

// Default icon color (white in RGB565 format)
constexpr uint16_t DEFAULT_ICON_COLOR = 65535u;

// Length of "uuid." prefix used in internal IDs
constexpr size_t UUID_PREFIX_LENGTH = 5;

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif
#ifdef _MSC_VER
#define PACK(__Declaration__)                                 \
  __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

#if defined(__cpp_char8_t)
typedef char8_t icon_char_t;
#define CHAR8_CAST(icon) reinterpret_cast<const char *>(icon)
#else
#define CHAR8_CAST(icon) icon
typedef char icon_char_t;
#endif

} // namespace nspanel_lovelace
} // namespace esphome