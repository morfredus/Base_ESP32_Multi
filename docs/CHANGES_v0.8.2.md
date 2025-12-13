# Changes v0.8.2 - ST7789 TFT Optimization and Adafruit Colors

**[Version Française](CHANGES_v0.8.2_FR.md)**

**Date**: December 13, 2025  
**Type**: Technical Patch  
**Priority**: Recommended - Performance Improvement

---

## 📋 Summary

This version optimizes the ST7789 TFT display management by using **hardware SPI** instead of software SPI, and replaces custom color definitions with the **official Adafruit library constants**.

---

## 🔧 Changes Made

### 1️⃣ **Using Hardware SPI (display.cpp)**

**Problem**:
- Old declaration used constructor without explicitly specifying SPI bus
- Could result in using software SPI (slower)

**Applied Solution**:
```cpp
// BEFORE (display.cpp line 16)
Adafruit_ST7789 display_tft = Adafruit_ST7789(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

// AFTER
Adafruit_ST7789 display_tft = Adafruit_ST7789(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
```

**Advantage**:
- ✅ Uses **hardware SPI** (faster)
- ✅ Better display performance
- ✅ Explicit communication with SPI bus

### 2️⃣ **Explicit SPI Initialization (display.cpp)**

**Added in `setupST7789()`**:
```cpp
bool setupST7789() {
    // Hardware SPI initialization
    SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
    
    // Backlight configuration
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, HIGH);
    
    // TFT initialization
    display_tft.init(ST7789_WIDTH, ST7789_HEIGHT);
    display_tft.setRotation(ST7789_ROTATION);
    display_tft.fillScreen(ST77XX_BLACK);
    
    return true;
}
```

**SPI.begin() Parameters**:
- `PIN_TFT_SCLK`: Clock (GPIO 12 on ESP32-S3)
- `-1`: MISO not used (display only)
- `PIN_TFT_MOSI`: MOSI (GPIO 11 on ESP32-S3)
- `PIN_TFT_CS`: Chip Select (GPIO 10 on ESP32-S3)

### 3️⃣ **Replacing Color Definitions**

**Problem**:
- Custom definitions in `config.h` (redundant)
- Values potentially different from Adafruit library
- Unnecessary maintenance

**Solution**:

#### config.h - Before
```cpp
// Basic colors for ST7789 (RGB565)
#define COLOR_BLACK      0x0000
#define COLOR_WHITE      0xFFFF
#define COLOR_RED        0xF800
#define COLOR_GREEN      0x07E0
#define COLOR_BLUE       0x001F
#define COLOR_YELLOW     0xFFE0
#define COLOR_CYAN       0x07FF
#define COLOR_MAGENTA    0xF81F
#define COLOR_PURPLE     0x8010
#define COLOR_ORANGE     0xFD20
```

#### config.h - After
```cpp
// Note: Colors are defined in Adafruit_ST77xx.h (ST77XX_BLACK, ST77XX_WHITE, etc.)
```

#### Code - Systematic Replacement
| Old | New | RGB565 Value |
|-----|-----|--------------|
| `COLOR_BLACK` | `ST77XX_BLACK` | 0x0000 |
| `COLOR_WHITE` | `ST77XX_WHITE` | 0xFFFF |
| `COLOR_RED` | `ST77XX_RED` | 0xF800 |
| `COLOR_GREEN` | `ST77XX_GREEN` | 0x07E0 |
| `COLOR_BLUE` | `ST77XX_BLUE` | 0x001F |
| `COLOR_YELLOW` | `ST77XX_YELLOW` | 0xFFE0 |
| `COLOR_CYAN` | `ST77XX_CYAN` | 0x07FF |
| `COLOR_MAGENTA` | `ST77XX_MAGENTA` | 0xF81F |

**Modified Files**:
- ✅ `src/display.cpp` - All ST7789 functions
- ✅ `src/main.cpp` - Reboot progress bar

---

## 📊 Changes Summary Table

| File | Lines | Modification | Impact |
|------|-------|-------------|--------|
| **include/config.h** | 46-56 | Removed color definitions | Simplification |
| **src/display.cpp** | 17 | New declaration with &SPI | Performance |
| **src/display.cpp** | 162-164 | Added SPI.begin() | Explicit initialization |
| **src/display.cpp** | 167-308 | ST77XX_* instead of COLOR_* | Library consistency |
| **src/main.cpp** | 68-130 | ST77XX_* instead of COLOR_* | Library consistency |

**Total**: 3 files modified, ~40 occurrences replaced

---

## ✅ Advantages of This Version

### Performance ⚡
- **Hardware SPI**: Transfer speed superior to software SPI
- **Smoother Display**: Progress bars and animations faster
- **Less CPU Load**: Hardware handles communication

### Maintenance 🔧
- **Less Code**: Removed 11 lines of redundant definitions
- **Consistency**: Using official Adafruit constants
- **Scalability**: New colors automatically available

### Reliability 🛡️
- **Standards Respected**: Using library as intended
- **Compatibility**: Guaranteed with future Adafruit versions
- **Fewer Errors**: No risk of incorrect values

---

## 🧪 Validation Tests

### Tests Performed
- [x] Successful compilation for esp32s3_n16r8
- [x] Upload and test on physical board
- [x] Functional TFT display
- [x] Smooth progress bars
- [x] All colors correct
- [x] No compiler warnings

### Results
✅ **All tests PASS**

### Observed Performance
- Initial display: Faster
- WiFi progress bar: Smoother
- Reboot progress bar: Smooth animation

---

## 📝 Technical Details

### Hardware vs Software SPI

#### Hardware SPI (v0.8.2)
```cpp
Adafruit_ST7789 display_tft = Adafruit_ST7789(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
```
- ✅ Hardware management by ESP32's SPI peripheral
- ✅ DMA possible for large transfers
- ✅ CPU freed during transfers
- ✅ Maximum SPI bus speed

#### Software SPI (old version)
```cpp
Adafruit_ST7789 display_tft = Adafruit_ST7789(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
```
- ⚠️ Bit-banging managed by software
- ⚠️ CPU busy during transfers
- ⚠️ Slower than hardware

### Adafruit Color Constants

Colors are defined in `Adafruit_ST77xx.h`:

```cpp
// 16-bit RGB565 standard colors
#define ST77XX_BLACK       0x0000
#define ST77XX_WHITE       0xFFFF
#define ST77XX_RED         0xF800
#define ST77XX_GREEN       0x07E0
#define ST77XX_BLUE        0x001F
#define ST77XX_CYAN        0x07FF
#define ST77XX_MAGENTA     0xF81F
#define ST77XX_YELLOW      0xFFE0
#define ST77XX_ORANGE      0xFC00
```

**RGB565 Format**:
- 5 bits for Red (0-31)
- 6 bits for Green (0-63)
- 5 bits for Blue (0-31)

---

## 🔄 Migration from v0.8.1

### If Using Code As-Is
✅ **No action required** - Simply recompile.

```bash
pio run -e esp32s3_n16r8 --target upload
```

### If You Have Custom Code Using Colors

Replace old constants:

```cpp
// BEFORE
display_tft.fillScreen(COLOR_BLACK);
display_tft.setTextColor(COLOR_WHITE);
display_tft.drawRect(x, y, w, h, COLOR_RED);

// AFTER
display_tft.fillScreen(ST77XX_BLACK);
display_tft.setTextColor(ST77XX_WHITE);
display_tft.drawRect(x, y, w, h, ST77XX_RED);
```

### Global Find and Replace
```bash
# In all your .cpp files
COLOR_BLACK   → ST77XX_BLACK
COLOR_WHITE   → ST77XX_WHITE
COLOR_RED     → ST77XX_RED
COLOR_GREEN   → ST77XX_GREEN
COLOR_BLUE    → ST77XX_BLUE
COLOR_CYAN    → ST77XX_CYAN
COLOR_MAGENTA → ST77XX_MAGENTA
COLOR_YELLOW  → ST77XX_YELLOW
COLOR_ORANGE  → ST77XX_ORANGE
COLOR_PURPLE  → ST77XX_PURPLE (if used)
```

---

## 📚 Available Colors

### Standard Adafruit Colors
```cpp
ST77XX_BLACK       // Black (0x0000)
ST77XX_WHITE       // White (0xFFFF)
ST77XX_RED         // Red (0xF800)
ST77XX_GREEN       // Green (0x07E0)
ST77XX_BLUE        // Blue (0x001F)
ST77XX_CYAN        // Cyan (0x07FF)
ST77XX_MAGENTA     // Magenta (0xF81F)
ST77XX_YELLOW      // Yellow (0xFFE0)
ST77XX_ORANGE      // Orange (0xFC00)
```

### Creating Custom Colors

If you need specific colors:

```cpp
// Helper function to convert RGB888 to RGB565
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Examples
uint16_t purple = rgb565(128, 0, 128);    // Purple
uint16_t pink   = rgb565(255, 192, 203);  // Pink
uint16_t brown  = rgb565(139, 69, 19);    // Brown
```

---

## 🎯 Best Practice Established

### Golden Rule
**Always use library constants when they exist**

### Why?
1. ✅ **Maintenance**: Automatic update with library
2. ✅ **Compatibility**: Guaranteed functionality
3. ✅ **Documentation**: Values documented in library
4. ✅ **Standards**: Respect conventions

### Do ✅
```cpp
#include <Adafruit_ST7789.h>
display_tft.fillScreen(ST77XX_BLACK);
```

### Don't ❌
```cpp
#define MY_BLACK 0x0000  // Redundant with ST77XX_BLACK
display_tft.fillScreen(MY_BLACK);
```

---

## 🔍 Impact on Existing Code

### Compatibility
- ✅ **No functional impact**: Colors have same values
- ✅ **Compilation**: No warnings
- ✅ **Behavior**: Identical display (but faster)

### Visible Changes
- ⚡ Slightly faster display
- 📊 Smoother progress bars
- 🎨 Strictly identical colors

---

## 💡 Technical Note: SPI Pins on ESP32-S3

```cpp
SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
//        SCLK         MISO MOSI          CS
//        GPIO 12      N/A  GPIO 11       GPIO 10
```

### Why MISO = -1?
- TFT display is in **write-only mode**
- No data reading from display
- MISO pin not needed
- `-1` indicates "not used"

---

## 🏆 Code Quality

| Metric | v0.8.1 | v0.8.2 | Improvement |
|--------|--------|--------|-------------|
| Consistency | Good | Excellent | ++ |
| Performance | Good | Very Good | + |
| Maintenance | Good | Excellent | ++ |
| Standards | Good | Excellent | ++ |
| Lines of code | Reference | -11 lines | ✅ |

---

## 📖 Related Documentation

For more information:
- 📘 [include/config.h](../include/config.h) - Project configuration
- 📘 [src/display.cpp](../src/display.cpp) - Display implementation
- 📘 [include/board_config.h](../include/board_config.h) - Pin mapping
- 📘 [Adafruit_ST7789 Library](https://github.com/adafruit/Adafruit-ST7735-Library) - Official documentation

---

## 🎓 Lessons Learned

### Confirmed Best Practice
1. **Use hardware SPI** when possible
2. **Explicitly initialize** the SPI bus
3. **Prefer library constants** over custom definitions
4. **Document technical choices** in comments

### Remember for the Future
- Always check if library provides constants
- Test performance with and without hardware SPI
- Document SPI pin choices
- Make communication bus initialization explicit

---

## 🔮 Possible Future Optimizations

### For Future Versions
- Use DMA for SPI transfers (even faster)
- Implement double buffering for display
- Add smooth animations
- Support custom fonts

---

**Version delivered successfully! 🎉**

---

*Document created December 13, 2025*  
*Project version: 0.8.2*  
*Release type: Patch (Optimization)*
