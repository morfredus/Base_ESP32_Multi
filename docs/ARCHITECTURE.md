# 📚 Modular Architecture - Reference Guide (v0.6.0+)

**[Version Française](ARCHITECTURE_FR.md)**

## Overview

Starting from version 0.6.0, the web interface is organized into separate modules to improve:
- **Maintainability**: More readable and easy-to-modify code
- **Reusability**: Components usable in other projects
- **Scalability**: Easy addition of new pages/routes
- **Documentation**: Complete Doxygen code

## Module Structure

### 1. `include/web_styles.h` - CSS Stylesheet
**Responsibility**: Contains all CSS styles for the web interface

```cpp
#include "web_styles.h"

// Access to: const char* WEB_STYLES
String html = "<style>" + String(WEB_STYLES) + "</style>";
```

**Contains**:
- CSS Reset (`*` selector)
- Responsive layout (Grid, Flexbox)
- Components (cards, buttons, progress bars)
- Animations and transitions

**Advantages**:
- ✅ Centralized and easily modifiable CSS
- ✅ Can be minified/compressed to save memory
- ✅ Reusable across multiple HTML pages

---

### 2. `include/web_pages.h` - HTML Page Generator
**Responsibility**: Generate HTML content for the web interface

```cpp
#include "web_pages.h"

// Available function:
String html = generateDashboardPage(
    chipId, flashSize, flashSpeed,
    heapSize, freeHeap,
    psramSize, freePsram,
    cpuFreq
);
```

**Contains**:
- `generateDashboardPage()` function - Generates complete dashboard page
- Style inclusion via `web_styles.h`
- Usage percentage calculation logic

**Advantages**:
- ✅ Parameterized and flexible HTML generation
- ✅ Easy to add new cards/sections
- ✅ Separation between data and presentation

**Extensibility**:
```cpp
// Add a new function in web_pages.h:
String generateStatusPage(const char* status) {
    // Return custom HTML
}

// Then use in handleStatus() in web_interface.h
void handleStatus() {
    String html = generateStatusPage("Connected");
    server.send(200, "text/html", html);
}
```

---

### 3. `include/web_interface.h` - Handlers and Server Configuration
**Responsibility**: Manage HTTP routes and web server callbacks

```cpp
#include "web_interface.h"  // Automatically includes web_pages.h and web_styles.h

// Available functions:
void handleRoot();           // GET /
void handleReboot();         // GET /reboot
void handleNotFound();       // 404
void setupWebServer();       // Initialize routes
```

**Contains**:
- `handleRoot()` - Generates and sends homepage
- `handleReboot()` - Handles reboots
- `handleNotFound()` - Handles missing pages
- `setupWebServer()` - Configures all routes

**Dependencies**:
- Uses `generateDashboardPage()` from `web_pages.h`
- Accesses `server` (global variable from `main.cpp`)

---

## Dependency Diagram

```
main.cpp
   └── #include "web_interface.h"
       ├── #include "web_pages.h"
       │   └── #include "web_styles.h"
       └── extern WebServer server
```

## HTTP Request Workflow

```
Client -> GET / 
         ↓
    handleRoot() (web_interface.h)
         ↓
    generateDashboardPage() (web_pages.h)
         ↓
    String(WEB_STYLES) (web_styles.h)
         ↓
    server.send(200, "text/html", html)
         ↓
Client <- HTML + CSS + JS
```

---

## How to Add a New Page/Route

### Example: Adding a `/api/status` page

**1. Create generator function in `web_pages.h`**:
```cpp
String generateStatusJson() {
    String json = "{";
    json += "\"status\":\"connected\",";
    json += "\"uptime\":" + String(millis() / 1000);
    json += "}";
    return json;
}
```

**2. Create handler in `web_interface.h`**:
```cpp
void handleStatus() {
    String json = generateStatusJson();
    server.send(200, "application/json", json);
}
```

**3. Register route in `setupWebServer()`**:
```cpp
void setupWebServer() {
    server.on("/", handleRoot);
    server.on("/reboot", handleReboot);
    server.on("/api/status", handleStatus);  // NEW
    server.onNotFound(handleNotFound);
}
```

**4. Call in `main.cpp`**:
```cpp
void setup() {
    setupWebServer();
    server.begin();
}
```

---

## Best Practices

### ✅ DO
- Keep HTML generation in `web_pages.h`
- Keep HTTP handlers in `web_interface.h`
- Keep CSS in `web_styles.h`
- Document functions with Doxygen comments
- Test each new route independently

### ❌ DON'T
- Put HTML code in `main.cpp`
- Mix presentation and business logic
- Hardcode values (use parameters)
- Forget to update documentation when adding features

---

## Benefits of Modular Architecture

### Before v0.6.0 (Monolithic)
```cpp
// main.cpp (424 lines)
void handleRoot() {
    String html = "<!DOCTYPE html>...";
    html += "<style>...";  // 50 lines
    html += "<body>...";   // 200 lines
    server.send(200, "text/html", html);
}
```

**Problems**:
- ❌ Difficult to maintain
- ❌ Impossible to reuse
- ❌ Mixed concerns
- ❌ Difficult testing

### After v0.6.0 (Modular)
```cpp
// main.cpp (271 lines)
#include "web_interface.h"

void setup() {
    setupWebServer();
}
```

**Benefits**:
- ✅ **-36% lines** in main.cpp
- ✅ Clean separation of concerns
- ✅ Reusable modules
- ✅ Easy testing
- ✅ Simple maintenance

---

## Testing New Routes

### Manual Test
1. Compile and upload code
2. Open browser to `http://ESP32_IP/`
3. Navigate to new route
4. Verify HTML/JSON display
5. Check Serial logs

### Automated Test (future)
```cpp
// test/test_web_interface.cpp
#include <unity.h>
#include "web_pages.h"

void test_generate_dashboard() {
    String html = generateDashboardPage("123", 4, 40, 520, 300, 0, 0, 240);
    TEST_ASSERT_TRUE(html.indexOf("<!DOCTYPE html>") == 0);
    TEST_ASSERT_TRUE(html.indexOf("ESP32-S3") > 0);
}
```

---

## Migration from v0.5.0 to v0.6.0

### Before (v0.5.0)
```cpp
// main.cpp
void handleRoot() {
    String html = "<!DOCTYPE html>";
    html += "<head><style>";
    html += "body { background: #0a0e27; }";
    // ... 200 lines ...
    server.send(200, "text/html", html);
}

void setup() {
    server.on("/", handleRoot);
    server.on("/reboot", handleReboot);
    server.begin();
}
```

### After (v0.6.0)
```cpp
// main.cpp
#include "web_interface.h"

void setup() {
    setupWebServer();  // All routes configured here
    server.begin();
}
```

**Migration Guide**: See [docs/UPGRADE_0.6.0.md](UPGRADE_0.6.0.md)

---

## Module Responsibilities Summary

| Module | File | Lines | Responsibility |
|--------|------|-------|---------------|
| **Styles** | `web_styles.h` | 220 | CSS centralization |
| **Pages** | `web_pages.h` | 130 | HTML generation |
| **Interface** | `web_interface.h` | 90 | HTTP handlers |
| **Main** | `main.cpp` | 271 | Application logic |

**Total web code**: 440 lines (previously 300 lines in main.cpp)  
**Gain**: +140 lines but separated into 3 reusable modules

---

## Further Reading

- [UPGRADE_0.6.0.md](UPGRADE_0.6.0.md) - Migration guide
- [COHERENCE_CHECK.md](COHERENCE_CHECK.md) - Version consistency
- [SYNTHESIS.md](SYNTHESIS.md) - Complete v0.6.0 summary
- [PIN_MAPPING.md](PIN_MAPPING.md) - Hardware wiring
