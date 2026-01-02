/**********************************************************************************************
*
*   rcore_memory - Functions to manage window, graphics device and inputs
*
*   PLATFORM: MEMORY (No OS)
*       - Memory framebuffer output (no os)
*
*   LIMITATIONS:
*       - Software renderer (rlsw)
*       - No input system
*
*   POSSIBLE IMPROVEMENTS:
*       - Improvement 01
*       - Improvement 02
*
*   ADDITIONAL NOTES:
*       - TRACELOG() function is located in raylib [utils] module
*
*   CONFIGURATION:
*       #define RCORE_PLATFORM_CUSTOM_FLAG
*           Custom flag for rcore on target platform -not used-
*
*   DEPENDENCIES:
*       - rlsw: Software renderer
*       - gestures: Gestures system for touch-ready devices (or simulated from mouse inputs)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2025-2026 Ramon Santamaria (@raysan5) and contributors
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#if defined(_WIN32)
    #include <conio.h>              // Required for: kbhit()
#else
    // Provide kbhit() function in non-Windows platforms
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Platform-specific required data for timming (Win32)
#if defined(_WIN32)
typedef struct _LARGE_INTEGER { int64_t QuadPart; } LARGE_INTEGER;
__declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);
__declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency);
#endif

typedef struct {
    unsigned int *pixels;   // Pointer to pixel data buffer (RGBA8888 format)
#if defined(_WIN32)
    LARGE_INTEGER timerFrequency;
#endif
} PlatformData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern CoreData CORE;                   // Global CORE state context

static PlatformData platform = { 0 };   // Platform specific data

//----------------------------------------------------------------------------------
// Module Internal Functions Declaration
//----------------------------------------------------------------------------------
int InitPlatform(void);                 // Initialize platform (graphics, inputs and more)
bool InitGraphicsDevice(void);          // Initialize graphics device

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
// NOTE: Functions declaration is provided by raylib.h

//----------------------------------------------------------------------------------
// Module Internal Functions Declaration
//----------------------------------------------------------------------------------
#if !defined(_WIN32)
static int kbhit(void);                         // Check if a key has been pressed
static char getch(void) { return getchar(); }   // Get pressed character
#endif

//----------------------------------------------------------------------------------
// Module Functions Definition: Window and Graphics Device
//----------------------------------------------------------------------------------

// Check if application should close
bool rl_WindowShouldClose(void)
{
    if (CORE.Window.ready) return CORE.Window.shouldClose;
    else return true;
}

// Toggle fullscreen mode
void rl_ToggleFullscreen(void)
{
    TRACELOG(LOG_WARNING, "rl_ToggleFullscreen() not available on target platform");
}

// Toggle borderless windowed mode
void rl_ToggleBorderlessWindowed(void)
{
    TRACELOG(LOG_WARNING, "rl_ToggleBorderlessWindowed() not available on target platform");
}

// Set window state: maximized, if resizable
void rl_MaximizeWindow(void)
{
    TRACELOG(LOG_WARNING, "rl_MaximizeWindow() not available on target platform");
}

// Set window state: minimized
void rl_MinimizeWindow(void)
{
    TRACELOG(LOG_WARNING, "rl_MinimizeWindow() not available on target platform");
}

// Restore window from being minimized/maximized
void rl_RestoreWindow(void)
{
    TRACELOG(LOG_WARNING, "rl_RestoreWindow() not available on target platform");
}

// Set window configuration state using flags
void rl_SetWindowState(unsigned int flags)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowState() not available on target platform");
}

// Clear window configuration state flags
void rl_ClearWindowState(unsigned int flags)
{
    TRACELOG(LOG_WARNING, "rl_ClearWindowState() not available on target platform");
}

// Set icon for window
void rl_SetWindowIcon(rl_Image image)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowIcon() not available on target platform");
}

// Set icon for window
void rl_SetWindowIcons(rl_Image *images, int count)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowIcons() not available on target platform");
}

// Set title for window
void rl_SetWindowTitle(const char *title)
{
    CORE.Window.title = title;
}

// Set window position on screen (windowed mode)
void rl_SetWindowPosition(int x, int y)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowPosition() not available on target platform");
}

// Set monitor for the current window
void rl_SetWindowMonitor(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowMonitor() not available on target platform");
}

// Set window minimum dimensions (FLAG_WINDOW_RESIZABLE)
void rl_SetWindowMinSize(int width, int height)
{
    CORE.Window.screenMin.width = width;
    CORE.Window.screenMin.height = height;
}

// Set window maximum dimensions (FLAG_WINDOW_RESIZABLE)
void rl_SetWindowMaxSize(int width, int height)
{
    CORE.Window.screenMax.width = width;
    CORE.Window.screenMax.height = height;
}

// Set window dimensions
void rl_SetWindowSize(int width, int height)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowSize() not available on target platform");
}

// Set window opacity, value opacity is between 0.0 and 1.0
void rl_SetWindowOpacity(float opacity)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowOpacity() not available on target platform");
}

// Set window focused
void rl_SetWindowFocused(void)
{
    TRACELOG(LOG_WARNING, "rl_SetWindowFocused() not available on target platform");
}

// Get native window handle
void *rl_GetWindowHandle(void)
{
    TRACELOG(LOG_WARNING, "rl_GetWindowHandle() not implemented on target platform");
    return NULL;
}

// Get number of monitors
int rl_GetMonitorCount(void)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorCount() not implemented on target platform");
    return 1;
}

// Get current monitor where window is placed
int rl_GetCurrentMonitor(void)
{
    TRACELOG(LOG_WARNING, "rl_GetCurrentMonitor() not implemented on target platform");
    return 0;
}

// Get selected monitor position
rl_Vector2 rl_GetMonitorPosition(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorPosition() not implemented on target platform");
    return (rl_Vector2){ 0, 0 };
}

// Get selected monitor width (currently used by monitor)
int rl_GetMonitorWidth(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorWidth() not implemented on target platform");
    return 0;
}

// Get selected monitor height (currently used by monitor)
int rl_GetMonitorHeight(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorHeight() not implemented on target platform");
    return 0;
}

// Get selected monitor physical width in millimetres
int rl_GetMonitorPhysicalWidth(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorPhysicalWidth() not implemented on target platform");
    return 0;
}

// Get selected monitor physical height in millimetres
int rl_GetMonitorPhysicalHeight(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorPhysicalHeight() not implemented on target platform");
    return 0;
}

// Get selected monitor refresh rate
int rl_GetMonitorRefreshRate(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorRefreshRate() not implemented on target platform");
    return 0;
}

// Get the human-readable, UTF-8 encoded name of the selected monitor
const char *rl_GetMonitorName(int monitor)
{
    TRACELOG(LOG_WARNING, "rl_GetMonitorName() not implemented on target platform");
    return "";
}

// Get window position XY on monitor
rl_Vector2 rl_GetWindowPosition(void)
{
    TRACELOG(LOG_WARNING, "rl_GetWindowPosition() not implemented on target platform");
    return (rl_Vector2){ 0, 0 };
}

// Get window scale DPI factor for current monitor
rl_Vector2 rl_GetWindowScaleDPI(void)
{
    TRACELOG(LOG_WARNING, "rl_GetWindowScaleDPI() not implemented on target platform");
    return (rl_Vector2){ 1.0f, 1.0f };
}

// Set clipboard text content
void rl_SetClipboardText(const char *text)
{
    TRACELOG(LOG_WARNING, "rl_SetClipboardText() not implemented on target platform");
}

// Get clipboard text content
// NOTE: returned string is allocated and freed by GLFW
const char *rl_GetClipboardText(void)
{
    TRACELOG(LOG_WARNING, "rl_GetClipboardText() not implemented on target platform");
    return NULL;
}

// Get clipboard image
rl_Image rl_GetClipboardImage(void)
{
    rl_Image image = { 0 };

    TRACELOG(LOG_WARNING, "rl_GetClipboardImage() not implemented on target platform");

    return image;
}

// Show mouse cursor
void rl_ShowCursor(void)
{
    CORE.Input.Mouse.cursorHidden = false;
}

// Hides mouse cursor
void rl_HideCursor(void)
{
    CORE.Input.Mouse.cursorHidden = true;
}

// Enables cursor (unlock cursor)
void rl_EnableCursor(void)
{
    // Set cursor position in the middle
    rl_SetMousePosition(CORE.Window.screen.width/2, CORE.Window.screen.height/2);

    CORE.Input.Mouse.cursorHidden = false;
}

// Disables cursor (lock cursor)
void rl_DisableCursor(void)
{
    // Set cursor position in the middle
    rl_SetMousePosition(CORE.Window.screen.width/2, CORE.Window.screen.height/2);

    CORE.Input.Mouse.cursorHidden = true;
}

// Swap back buffer with front buffer (screen drawing)
void rl_SwapScreenBuffer(void)
{
    // Update framebuffer
    rlCopyFramebuffer(0, 0, CORE.Window.render.width, CORE.Window.render.height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, platform.pixels);
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Misc
//----------------------------------------------------------------------------------

// Get elapsed time measure in seconds since InitTimer()
double rl_GetTime(void)
{
    double time = 0.0;
#if defined(_WIN32)
    LARGE_INTEGER now = { 0 };
    QueryPerformanceCounter(&now);
    return (double)(now.QuadPart - CORE.Time.base)/(double)platform.timerFrequency.QuadPart;
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__EMSCRIPTEN__)
    struct timespec ts = { 0 };
    clock_gettime(CLOCK_MONOTONIC, &ts);
    unsigned long long int nanoSeconds = (unsigned long long int)ts.tv_sec*1000000000LLU + (unsigned long long int)ts.tv_nsec;
    time = (double)(nanoSeconds - CORE.Time.base)*1e-9;  // Elapsed time since InitTimer()
#endif
    return time;
}

// Open URL with default system browser (if available)
// NOTE: This function is only safe to use if you control the URL given.
// A user could craft a malicious string performing another action.
// Only call this function yourself not with user input or make sure to check the string yourself.
// REF: https://github.com/raysan5/raylib/issues/686
void rl_OpenURL(const char *url)
{
    // Security check to (partially) avoid malicious code on target platform
    if (strchr(url, '\'') != NULL) TRACELOG(LOG_WARNING, "SYSTEM: Provided URL could be potentially malicious, avoid [\'] character");
    else
    {
        char *cmd = (char *)RL_CALLOC(strlen(url) + 32, sizeof(char));
        sprintf(cmd, "explorer \"%s\"", url);
        int result = system(cmd);
        if (result == -1) TRACELOG(LOG_WARNING, "rl_OpenURL() child process could not be created");
        RL_FREE(cmd);
    }
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Inputs
//----------------------------------------------------------------------------------

// Set internal gamepad mappings
int rl_SetGamepadMappings(const char *mappings)
{
    TRACELOG(LOG_WARNING, "rl_SetGamepadMappings() not implemented on target platform");
    return 0;
}

// Set gamepad vibration
void rl_SetGamepadVibration(int gamepad, float leftMotor, float rightMotor, float duration)
{
    TRACELOG(LOG_WARNING, "rl_SetGamepadVibration() not implemented on target platform");
}

// Set mouse position XY
void rl_SetMousePosition(int x, int y)
{
    CORE.Input.Mouse.currentPosition = (rl_Vector2){ (float)x, (float)y };
    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
}

// Set mouse cursor
void rl_SetMouseCursor(int cursor)
{
    TRACELOG(LOG_WARNING, "rl_SetMouseCursor() not implemented on target platform");
}

// Get physical key name
const char *rl_GetKeyName(int key)
{
    TRACELOG(LOG_WARNING, "rl_GetKeyName() not implemented on target platform");
    return "";
}

// Register all input events
void rl_PollInputEvents(void)
{
#if defined(SUPPORT_GESTURES_SYSTEM)
    // NOTE: Gestures update must be called every frame to reset gestures correctly
    // because ProcessGestureEvent() is just called on an event, not every frame
    UpdateGestures();
#endif

    // Reset keys/chars pressed registered
    CORE.Input.Keyboard.keyPressedQueueCount = 0;
    CORE.Input.Keyboard.charPressedQueueCount = 0;

    // Reset key repeats
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) CORE.Input.Keyboard.keyRepeatInFrame[i] = 0;

    // Reset last gamepad button/axis registered state
    CORE.Input.Gamepad.lastButtonPressed = 0; // GAMEPAD_BUTTON_UNKNOWN
    //CORE.Input.Gamepad.axisCount = 0;

    // Register previous touch states
    for (int i = 0; i < MAX_TOUCH_POINTS; i++) CORE.Input.Touch.previousTouchState[i] = CORE.Input.Touch.currentTouchState[i];

    // Reset touch positions
    // TODO: It resets on target platform the mouse position and not filled again until a move-event,
    // so, if mouse is not moved it returns a (0, 0) position... this behaviour should be reviewed!
    //for (int i = 0; i < MAX_TOUCH_POINTS; i++) CORE.Input.Touch.position[i] = (rl_Vector2){ 0, 0 };

    // Register previous keys states
    // NOTE: Android supports up to 260 keys
    for (int i = 0; i < 260; i++)
    {
        CORE.Input.Keyboard.previousKeyState[i] = CORE.Input.Keyboard.currentKeyState[i];
        CORE.Input.Keyboard.keyRepeatInFrame[i] = 0;
    }

    // TODO: Poll input events for current platform

    // Check for key pressed to exit
    if (kbhit())
    {
        int key = getch();
        if (key == 27) CORE.Window.shouldClose = true; // KEY_SCAPE
    }
}

//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------

// Initialize platform: graphics, inputs and more
int InitPlatform(void)
{
    // Memory framebuffer can only work with software renderer
    if (rlGetVersion() != RL_OPENGL_11_SOFTWARE)
    {
        TRACELOG(LOG_WARNING, "DISPLAY: Memory platform requires software renderer (GRAPHICS_API_OPENGL_11_SOFTWARE)");
        TRACELOG(LOG_FATAL, "PLATFORM: Failed to initialize graphics device");
        return -1;
    }
    else
    {
        // Load memory framebuffer with desired screen size
        platform.pixels = (unsigned int *)RL_CALLOC(CORE.Window.screen.width*CORE.Window.screen.height, sizeof(int));
    }
    //----------------------------------------------------------------------------

    // If everything work as expected, we can continue
    CORE.Window.render.width = CORE.Window.screen.width;
    CORE.Window.render.height = CORE.Window.screen.height;
    CORE.Window.currentFbo.width = CORE.Window.render.width;
    CORE.Window.currentFbo.height = CORE.Window.render.height;

    TRACELOG(LOG_INFO, "DISPLAY: Device initialized successfully");
    TRACELOG(LOG_INFO, "    > Display size: %i x %i", CORE.Window.display.width, CORE.Window.display.height);
    TRACELOG(LOG_INFO, "    > Screen size:  %i x %i", CORE.Window.screen.width, CORE.Window.screen.height);
    TRACELOG(LOG_INFO, "    > Render size:  %i x %i", CORE.Window.render.width, CORE.Window.render.height);
    TRACELOG(LOG_INFO, "    > Viewport offsets: %i, %i", CORE.Window.renderOffset.x, CORE.Window.renderOffset.y);

    CORE.Window.ready = true;

    // TODO: Load OpenGL extensions
    // NOTE: GL procedures address loader is required to load extensions
    //----------------------------------------------------------------------------
    // ...
    //----------------------------------------------------------------------------

    // TODO: Initialize input events system
    // It could imply keyboard, mouse, gamepad, touch...
    // Depending on the platform libraries/SDK it could use a callback mechanism
    // For system events and inputs evens polling on a per-frame basis, use rl_PollInputEvents()
    //----------------------------------------------------------------------------
    // ...
    //----------------------------------------------------------------------------

    // Initialize timing system
    //----------------------------------------------------------------------------
#if defined(_WIN32)
    LARGE_INTEGER time = { 0 };
    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&platform.timerFrequency);
    CORE.Time.base = time.QuadPart;
#endif
    InitTimer();
    //----------------------------------------------------------------------------

    // Initialize storage system
    //----------------------------------------------------------------------------
    CORE.Storage.basePath = rl_GetWorkingDirectory();
    //----------------------------------------------------------------------------

    TRACELOG(LOG_INFO, "PLATFORM: MEMORY: Initialized successfully");

    return 0;
}

// Close platform
void ClosePlatform(void)
{
    RL_FREE(platform.pixels);
}

//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------
#if !defined(_WIN32)
// Check if a key has been pressed
static int kbhit(void)
{
    struct termios oldt = { 0 };
    struct termios newt = { 0 };
    int ch = 0;
    int oldf = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

// EOF
