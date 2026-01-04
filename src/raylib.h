/**********************************************************************************************
*
*   raylib v5.6-dev - A simple and easy-to-use library to enjoy videogames programming (www.raylib.com)
*
*   FEATURES:
*       - NO external dependencies, all required libraries included with raylib
*       - Multiplatform: Windows, Linux, FreeBSD, OpenBSD, NetBSD, DragonFly,
*                        MacOS, Haiku, Android, Raspberry Pi, DRM native, HTML5
*       - Written in plain C code (C99) in PascalCase/camelCase notation
*       - Hardware accelerated with OpenGL (1.1, 2.1, 3.3, 4.3, ES2, ES3 - choose at compile)
*       - Unique OpenGL abstraction layer (usable as standalone module): [rlgl]
*       - Multiple Fonts formats supported (TTF, OTF, FNT, BDF, Sprite fonts)
*       - Outstanding texture formats support, including compressed formats (DXT, ETC, ASTC)
*       - Full 3d support for 3d Shapes, Models, Billboards, Heightmaps and more!
*       - Flexible Materials system, supporting classic maps and PBR maps
*       - Animated 3D models supported (skeletal bones animation) (IQM, M3D, GLTF)
*       - Shaders support, including rl_Model shaders and Postprocessing shaders
*       - Powerful math module for Vector, rl_Matrix and rl_Quaternion operations: [raymath]
*       - Audio loading and playing with streaming support (WAV, OGG, MP3, FLAC, QOA, XM, MOD)
*       - VR stereo rendering with configurable HMD device parameters
*       - Bindings to multiple programming languages available!
*
*   NOTES:
*       - One default rl_Font is loaded on rl_InitWindow()->LoadFontDefault() [core, text]
*       - One default rl_Texture2D is loaded on rlglInit(), 1x1 white pixel R8G8B8A8 [rlgl] (OpenGL 3.3 or ES2)
*       - One default rl_Shader is loaded on rlglInit()->rlLoadShaderDefault() [rlgl] (OpenGL 3.3 or ES2)
*       - One default RenderBatch is loaded on rlglInit()->rlLoadRenderBatch() [rlgl] (OpenGL 3.3 or ES2)
*
*   DEPENDENCIES (included):
*       [rcore][GLFW] rglfw (Camilla Löwy - github.com/glfw/glfw) for window/context management and input
*       [rcore][RGFW] rgfw (ColleagueRiley - github.com/ColleagueRiley/RGFW) for window/context management and input
*       [rlgl] glad/glad_gles2 (David Herberth - github.com/Dav1dde/glad) for OpenGL 3.3 extensions loading
*       [raudio] miniaudio (David Reid - github.com/mackron/miniaudio) for audio device/context management
*
*   OPTIONAL DEPENDENCIES (included):
*       [rcore] sinfl (Micha Mettke) for DEFLATE decompression algorithm
*       [rcore] sdefl (Micha Mettke) for DEFLATE compression algorithm
*       [rcore] rprand (Ramon Santamaria) for pseudo-random numbers generation
*       [rtextures] qoi (Dominic Szablewski - https://phoboslab.org) for QOI image manage
*       [rtextures] stb_image (Sean Barret) for images loading (BMP, TGA, PNG, JPEG, HDR...)
*       [rtextures] stb_image_write (Sean Barret) for image writing (BMP, TGA, PNG, JPG)
*       [rtextures] stb_image_resize2 (Sean Barret) for image resizing algorithms
*       [rtextures] stb_perlin (Sean Barret) for Perlin Noise image generation
*       [rtext] stb_truetype (Sean Barret) for ttf fonts loading
*       [rtext] stb_rect_pack (Sean Barret) for rectangles packing
*       [rmodels] par_shapes (Philip Rideout) for parametric 3d shapes generation
*       [rmodels] tinyobj_loader_c (Syoyo Fujita) for models loading (OBJ, MTL)
*       [rmodels] cgltf (Johannes Kuhlmann) for models loading (glTF)
*       [rmodels] m3d (bzt) for models loading (M3D, https://bztsrc.gitlab.io/model3d)
*       [rmodels] vox_loader (Johann Nadalutti) for models loading (VOX)
*       [raudio] dr_wav (David Reid) for WAV audio file loading
*       [raudio] dr_flac (David Reid) for FLAC audio file loading
*       [raudio] dr_mp3 (David Reid) for MP3 audio file loading
*       [raudio] stb_vorbis (Sean Barret) for OGG audio loading
*       [raudio] jar_xm (Joshua Reisenauer) for XM audio module loading
*       [raudio] jar_mod (Joshua Reisenauer) for MOD audio module loading
*       [raudio] qoa (Dominic Szablewski - https://phoboslab.org) for QOA audio manage
*
*
*   LICENSE: zlib/libpng
*
*   raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2013-2026 Ramon Santamaria (@raysan5)
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

#ifndef RAYLIB_H
#define RAYLIB_H

#include <stdarg.h>     // Required for: va_list - Only used by TraceLogCallback

#define rl_RAYLIB_VERSION_MAJOR 5
#define rl_RAYLIB_VERSION_MINOR 6
#define rl_RAYLIB_VERSION_PATCH 0
#define rl_RAYLIB_VERSION  "5.6-dev"

// Function specifiers in case library is build/used as a shared library
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
// NOTE: visibility("default") attribute makes symbols "visible" when compiled with -fvisibility=hidden
#if defined(_WIN32)
    #if defined(__TINYC__)
        #define __declspec(x) __attribute__((x))
    #endif
    #if defined(BUILD_LIBTYPE_SHARED)
        #define rl_RLAPI __declspec(dllexport)     // Building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define rl_RLAPI __declspec(dllimport)     // Using the library as a Win32 shared library (.dll)
    #endif
#else
    #if defined(BUILD_LIBTYPE_SHARED)
        #define rl_RLAPI __attribute__((visibility("default"))) // Building as a Unix shared library (.so/.dylib)
    #endif
#endif

#ifndef rl_RLAPI
    #define rl_RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef rl_PI
    #define rl_PI 3.14159265358979323846f
#endif
#ifndef rl_DEG2RAD
    #define rl_DEG2RAD (rl_PI/180.0f)
#endif
#ifndef rl_RAD2DEG
    #define rl_RAD2DEG (180.0f/rl_PI)
#endif

// Allow custom memory allocators
// NOTE: Require recompiling raylib sources
#ifndef RL_MALLOC
    #define RL_MALLOC(sz)       malloc(sz)
#endif
#ifndef RL_CALLOC
    #define RL_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RL_REALLOC
    #define RL_REALLOC(ptr,sz)  realloc(ptr,sz)
#endif
#ifndef RL_FREE
    #define RL_FREE(ptr)        free(ptr)
#endif

// NOTE: MSVC C++ compiler does not support compound literals (C99 feature)
// Plain structures in C++ (without constructors) can be initialized with { }
// This is called aggregate initialization (C++11 feature)
#if defined(__cplusplus)
    #define CLITERAL(type)      type
#else
    #define CLITERAL(type)      (type)
#endif

// Some compilers (mostly macos clang) default to C++98,
// where aggregate initialization can't be used
// So, give a more clear error stating how to fix this
#if !defined(_MSC_VER) && (defined(__cplusplus) && __cplusplus < 201103L)
    #error "C++11 or later is required. Add -std=c++11"
#endif

// NOTE: Set some defines with some data types declared by raylib
// Other modules (raymath, rlgl) also require some of those types, so,
// to be able to use those other modules as standalone (not depending on raylib)
// this defines are very useful for internal check and avoid type (re)definitions
#define RL_COLOR_TYPE
#define RL_RECTANGLE_TYPE
#define RL_VECTOR2_TYPE
#define RL_VECTOR3_TYPE
#define RL_VECTOR4_TYPE
#define RL_QUATERNION_TYPE
#define RL_MATRIX_TYPE

// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on rl_WHITE background
#define rl_LIGHTGRAY  CLITERAL(rl_Color){ 200, 200, 200, 255 }   // Light Gray
#define rl_GRAY       CLITERAL(rl_Color){ 130, 130, 130, 255 }   // Gray
#define rl_DARKGRAY   CLITERAL(rl_Color){ 80, 80, 80, 255 }      // Dark Gray
#define rl_YELLOW     CLITERAL(rl_Color){ 253, 249, 0, 255 }     // Yellow
#define rl_GOLD       CLITERAL(rl_Color){ 255, 203, 0, 255 }     // Gold
#define rl_ORANGE     CLITERAL(rl_Color){ 255, 161, 0, 255 }     // Orange
#define rl_PINK       CLITERAL(rl_Color){ 255, 109, 194, 255 }   // Pink
#define rl_RED        CLITERAL(rl_Color){ 230, 41, 55, 255 }     // Red
#define rl_MAROON     CLITERAL(rl_Color){ 190, 33, 55, 255 }     // Maroon
#define rl_GREEN      CLITERAL(rl_Color){ 0, 228, 48, 255 }      // Green
#define rl_LIME       CLITERAL(rl_Color){ 0, 158, 47, 255 }      // Lime
#define rl_DARKGREEN  CLITERAL(rl_Color){ 0, 117, 44, 255 }      // Dark Green
#define rl_SKYBLUE    CLITERAL(rl_Color){ 102, 191, 255, 255 }   // Sky Blue
#define rl_BLUE       CLITERAL(rl_Color){ 0, 121, 241, 255 }     // Blue
#define rl_DARKBLUE   CLITERAL(rl_Color){ 0, 82, 172, 255 }      // Dark Blue
#define rl_PURPLE     CLITERAL(rl_Color){ 200, 122, 255, 255 }   // Purple
#define rl_VIOLET     CLITERAL(rl_Color){ 135, 60, 190, 255 }    // Violet
#define rl_DARKPURPLE CLITERAL(rl_Color){ 112, 31, 126, 255 }    // Dark Purple
#define rl_BEIGE      CLITERAL(rl_Color){ 211, 176, 131, 255 }   // Beige
#define rl_BROWN      CLITERAL(rl_Color){ 127, 106, 79, 255 }    // Brown
#define rl_DARKBROWN  CLITERAL(rl_Color){ 76, 63, 47, 255 }      // Dark Brown

#define rl_WHITE      CLITERAL(rl_Color){ 255, 255, 255, 255 }   // White
#define rl_BLACK      CLITERAL(rl_Color){ 0, 0, 0, 255 }         // Black
#define rl_BLANK      CLITERAL(rl_Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define rl_MAGENTA    CLITERAL(rl_Color){ 255, 0, 255, 255 }     // Magenta
#define rl_RAYWHITE   CLITERAL(rl_Color){ 245, 245, 245, 255 }   // My own White (raylib logo)

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Boolean type
#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
    #define RL_BOOL_TYPE
#endif

// rl_Vector2, 2 components
typedef struct rl_Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} rl_Vector2;

// rl_Vector3, 3 components
typedef struct rl_Vector3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} rl_Vector3;

// rl_Vector4, 4 components
typedef struct rl_Vector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} rl_Vector4;

// rl_Quaternion, 4 components (rl_Vector4 alias)
typedef rl_Vector4 rl_Quaternion;

// rl_Matrix, 4x4 components, column major, OpenGL style, right-handed
typedef struct rl_Matrix {
    float m0, m4, m8, m12;  // rl_Matrix first row (4 components)
    float m1, m5, m9, m13;  // rl_Matrix second row (4 components)
    float m2, m6, m10, m14; // rl_Matrix third row (4 components)
    float m3, m7, m11, m15; // rl_Matrix fourth row (4 components)
} rl_Matrix;

// rl_Color, 4 components, R8G8B8A8 (32bit)
typedef struct rl_Color {
    unsigned char r;        // rl_Color red value
    unsigned char g;        // rl_Color green value
    unsigned char b;        // rl_Color blue value
    unsigned char a;        // rl_Color alpha value
} rl_Color;

// rl_Rectangle, 4 components
typedef struct rl_Rectangle {
    float x;                // rl_Rectangle top-left corner position x
    float y;                // rl_Rectangle top-left corner position y
    float width;            // rl_Rectangle width
    float height;           // rl_Rectangle height
} rl_Rectangle;

// rl_Image, pixel data stored in CPU memory (RAM)
typedef struct rl_Image {
    void *data;             // rl_Image raw data
    int width;              // rl_Image base width
    int height;             // rl_Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (rl_PixelFormat type)
} rl_Image;

// rl_Texture, tex data stored in GPU memory (VRAM)
typedef struct rl_Texture {
    unsigned int id;        // OpenGL texture id
    int width;              // rl_Texture base width
    int height;             // rl_Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (rl_PixelFormat type)
} rl_Texture;

// rl_Texture2D, same as rl_Texture
typedef rl_Texture rl_Texture2D;

// rl_TextureCubemap, same as rl_Texture
typedef rl_Texture rl_TextureCubemap;

// rl_RenderTexture, fbo for texture rendering
typedef struct rl_RenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    rl_Texture texture;        // rl_Color buffer attachment texture
    rl_Texture depth;          // Depth buffer attachment texture
} rl_RenderTexture;

// rl_RenderTexture2D, same as rl_RenderTexture
typedef rl_RenderTexture rl_RenderTexture2D;

// rl_NPatchInfo, n-patch layout info
typedef struct rl_NPatchInfo {
    rl_Rectangle source;       // rl_Texture source rectangle
    int left;               // Left border offset
    int top;                // Top border offset
    int right;              // Right border offset
    int bottom;             // Bottom border offset
    int layout;             // Layout of the n-patch: 3x3, 1x3 or 3x1
} rl_NPatchInfo;

// rl_GlyphInfo, font characters glyphs info
typedef struct rl_GlyphInfo {
    int value;              // Character value (Unicode)
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
    rl_Image image;            // Character image data
} rl_GlyphInfo;

// rl_Font, font texture and rl_GlyphInfo array data
typedef struct rl_Font {
    int baseSize;           // Base size (default chars height)
    int glyphCount;         // Number of glyph characters
    int glyphPadding;       // Padding around the glyph characters
    rl_Texture2D texture;      // rl_Texture atlas containing the glyphs
    rl_Rectangle *recs;        // Rectangles in texture for the glyphs
    rl_GlyphInfo *glyphs;      // Glyphs info data
} rl_Font;

// rl_Camera, defines position/orientation in 3d space
typedef struct rl_Camera3D {
    rl_Vector3 position;       // rl_Camera position
    rl_Vector3 target;         // rl_Camera target it looks-at
    rl_Vector3 up;             // rl_Camera up vector (rotation over its axis)
    float fovy;             // rl_Camera field-of-view aperture in Y (degrees) in perspective, used as near plane height in world units in orthographic
    int projection;         // rl_Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} rl_Camera3D;

typedef rl_Camera3D rl_Camera;    // rl_Camera type fallback, defaults to rl_Camera3D

// rl_Camera2D, defines position/orientation in 2d space
typedef struct rl_Camera2D {
    rl_Vector2 offset;         // rl_Camera offset (screen space offset from window origin)
    rl_Vector2 target;         // rl_Camera target (world space target point that is mapped to screen space offset)
	float rotation;         // rl_Camera rotation in degrees (pivots around target)
    float zoom;             // rl_Camera zoom (scaling around target), must not be set to 0, set to 1.0f for no scale
} rl_Camera2D;

// rl_Mesh, vertex data and vao/vbo
typedef struct rl_Mesh {
    int vertexCount;        // Number of vertices stored in arrays
    int triangleCount;      // Number of triangles stored (indexed or not)

    // Vertex attributes data
    float *vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    float *normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float *tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    unsigned char *colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices;    // Vertex indices (in case vertex data comes indexed)

    // Animation vertex data
    float *animVertices;    // Animated vertex positions (after bones transformations)
    float *animNormals;     // Animated normals (after bones transformations)
    unsigned char *boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning) (shader-location = 6)
    float *boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning) (shader-location = 7)
    rl_Matrix *boneMatrices;   // Bones animated transformation matrices
    int boneCount;          // Number of bones

    // OpenGL identifiers
    unsigned int vaoId;     // OpenGL Vertex Array Object id
    unsigned int *vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
} rl_Mesh;

// rl_Shader
typedef struct rl_Shader {
    unsigned int id;        // rl_Shader program id
    int *locs;              // rl_Shader locations array (RL_MAX_SHADER_LOCATIONS)
} rl_Shader;

// rl_MaterialMap
typedef struct rl_MaterialMap {
    rl_Texture2D texture;      // rl_Material map texture
    rl_Color color;            // rl_Material map color
    float value;            // rl_Material map value
} rl_MaterialMap;

// rl_Material, includes shader and maps
typedef struct rl_Material {
    rl_Shader shader;          // rl_Material shader
    rl_MaterialMap *maps;      // rl_Material maps array (MAX_MATERIAL_MAPS)
    float params[4];        // rl_Material generic parameters (if required)
} rl_Material;

// rl_Transform, vertex transformation data
typedef struct rl_Transform {
    rl_Vector3 translation;    // Translation
    rl_Quaternion rotation;    // Rotation
    rl_Vector3 scale;          // Scale
} rl_Transform;

// Bone, skeletal animation bone
typedef struct rl_BoneInfo {
    char name[32];          // Bone name
    int parent;             // Bone parent
} rl_BoneInfo;

// rl_Model, meshes, materials and animation data
typedef struct rl_Model {
    rl_Matrix transform;       // Local transform matrix

    int meshCount;          // Number of meshes
    int materialCount;      // Number of materials
    rl_Mesh *meshes;           // Meshes array
    rl_Material *materials;    // Materials array
    int *meshMaterial;      // rl_Mesh material number

    // Animation data
    int boneCount;          // Number of bones
    rl_BoneInfo *bones;        // Bones information (skeleton)
    rl_Transform *bindPose;    // Bones base transformation (pose)
} rl_Model;

// rl_ModelAnimation
typedef struct rl_ModelAnimation {
    int boneCount;          // Number of bones
    int frameCount;         // Number of animation frames
    rl_BoneInfo *bones;        // Bones information (skeleton)
    rl_Transform **framePoses; // Poses array by frame
    char name[32];          // Animation name
} rl_ModelAnimation;

// rl_Ray, ray for raycasting
typedef struct rl_Ray {
    rl_Vector3 position;       // rl_Ray position (origin)
    rl_Vector3 direction;      // rl_Ray direction (normalized)
} rl_Ray;

// rl_RayCollision, ray hit information
typedef struct rl_RayCollision {
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    rl_Vector3 point;          // Point of the nearest hit
    rl_Vector3 normal;         // Surface normal of hit
} rl_RayCollision;

// rl_BoundingBox
typedef struct rl_BoundingBox {
    rl_Vector3 min;            // Minimum vertex box-corner
    rl_Vector3 max;            // Maximum vertex box-corner
} rl_BoundingBox;

// rl_Wave, audio wave data
typedef struct rl_Wave {
    unsigned int frameCount;    // Total number of frames (considering channels)
    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
    void *data;                 // Buffer data pointer
} rl_Wave;

// Opaque structs declaration
// NOTE: Actual structs are defined internally in raudio module
typedef struct rAudioBuffer rAudioBuffer;
typedef struct rAudioProcessor rAudioProcessor;

// rl_AudioStream, custom audio stream
typedef struct rl_AudioStream {
    rAudioBuffer *buffer;       // Pointer to internal data used by the audio system
    rAudioProcessor *processor; // Pointer to internal data processor, useful for audio effects

    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
} rl_AudioStream;

// rl_Sound
typedef struct rl_Sound {
    rl_AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
} rl_Sound;

// rl_Music, audio stream, anything longer than ~10 seconds should be streamed
typedef struct rl_Music {
    rl_AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
    bool looping;               // rl_Music looping enable

    int ctxType;                // Type of music context (audio filetype)
    void *ctxData;              // Audio context data, depends on type
} rl_Music;

// rl_VrDeviceInfo, Head-Mounted-Display device parameters
typedef struct rl_VrDeviceInfo {
    int hResolution;                // Horizontal resolution in pixels
    int vResolution;                // Vertical resolution in pixels
    float hScreenSize;              // Horizontal size in meters
    float vScreenSize;              // Vertical size in meters
    float eyeToScreenDistance;      // Distance between eye and display in meters
    float lensSeparationDistance;   // Lens separation distance in meters
    float interpupillaryDistance;   // IPD (distance between pupils) in meters
    float lensDistortionValues[4];  // Lens distortion constant parameters
    float chromaAbCorrection[4];    // Chromatic aberration correction parameters
} rl_VrDeviceInfo;

// rl_VrStereoConfig, VR stereo rendering configuration for simulator
typedef struct rl_VrStereoConfig {
    rl_Matrix projection[2];           // VR projection matrices (per eye)
    rl_Matrix viewOffset[2];           // VR view offset matrices (per eye)
    float leftLensCenter[2];        // VR left lens center
    float rightLensCenter[2];       // VR right lens center
    float leftScreenCenter[2];      // VR left screen center
    float rightScreenCenter[2];     // VR right screen center
    float scale[2];                 // VR distortion scale
    float scaleIn[2];               // VR distortion scale in
} rl_VrStereoConfig;

// File path list
typedef struct rl_FilePathList {
    unsigned int capacity;          // Filepaths max entries
    unsigned int count;             // Filepaths entries count
    char **paths;                   // Filepaths entries
} rl_FilePathList;

// Automation event
typedef struct rl_AutomationEvent {
    unsigned int frame;             // Event frame
    unsigned int type;              // Event type (AutomationEventType)
    int params[4];                  // Event parameters (if required)
} rl_AutomationEvent;

// Automation event list
typedef struct rl_AutomationEventList {
    unsigned int capacity;          // Events max entries (MAX_AUTOMATION_EVENTS)
    unsigned int count;             // Events entries count
    rl_AutomationEvent *events;        // Events entries
} rl_AutomationEventList;

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
typedef enum {
    FLAG_VSYNC_HINT         = 0x00000040,   // Set to try enabling V-Sync on GPU
    FLAG_FULLSCREEN_MODE    = 0x00000002,   // Set to run program in fullscreen
    FLAG_WINDOW_RESIZABLE   = 0x00000004,   // Set to allow resizable window
    FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
    FLAG_WINDOW_HIDDEN      = 0x00000080,   // Set to hide window
    FLAG_WINDOW_MINIMIZED   = 0x00000200,   // Set to minimize window (iconify)
    FLAG_WINDOW_MAXIMIZED   = 0x00000400,   // Set to maximize window (expanded to monitor)
    FLAG_WINDOW_UNFOCUSED   = 0x00000800,   // Set to window non focused
    FLAG_WINDOW_TOPMOST     = 0x00001000,   // Set to window always on top
    FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,   // Set to allow windows running while minimized
    FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
    FLAG_WINDOW_HIGHDPI     = 0x00002000,   // Set to support HighDPI
    FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
    FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000, // Set to run program in borderless windowed mode
    FLAG_MSAA_4X_HINT       = 0x00000020,   // Set to try enabling MSAA 4X
    FLAG_INTERLACED_HINT    = 0x00010000    // Set to try enabling interlaced video format (for V3D)
} rl_ConfigFlags;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    LOG_ALL = 0,        // Display all logs
    LOG_TRACE,          // Trace logging, intended for internal use only
    LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    LOG_INFO,           // Info logging, used for program execution info
    LOG_WARNING,        // Warning logging, used on recoverable failures
    LOG_ERROR,          // Error logging, used on unrecoverable failures
    LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    LOG_NONE            // Disable logging
} rl_TraceLogLevel;

// Keyboard keys (US keyboard layout)
// NOTE: Use rl_GetKeyPressed() to allow redefining
// required keys for alternative layouts
typedef enum {
    KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    KEY_APOSTROPHE      = 39,       // Key: '
    KEY_COMMA           = 44,       // Key: ,
    KEY_MINUS           = 45,       // Key: -
    KEY_PERIOD          = 46,       // Key: .
    KEY_SLASH           = 47,       // Key: /
    KEY_ZERO            = 48,       // Key: 0
    KEY_ONE             = 49,       // Key: 1
    KEY_TWO             = 50,       // Key: 2
    KEY_THREE           = 51,       // Key: 3
    KEY_FOUR            = 52,       // Key: 4
    KEY_FIVE            = 53,       // Key: 5
    KEY_SIX             = 54,       // Key: 6
    KEY_SEVEN           = 55,       // Key: 7
    KEY_EIGHT           = 56,       // Key: 8
    KEY_NINE            = 57,       // Key: 9
    KEY_SEMICOLON       = 59,       // Key: ;
    KEY_EQUAL           = 61,       // Key: =
    KEY_A               = 65,       // Key: A | a
    KEY_B               = 66,       // Key: B | b
    KEY_C               = 67,       // Key: C | c
    KEY_D               = 68,       // Key: D | d
    KEY_E               = 69,       // Key: E | e
    KEY_F               = 70,       // Key: F | f
    KEY_G               = 71,       // Key: G | g
    KEY_H               = 72,       // Key: H | h
    KEY_I               = 73,       // Key: I | i
    KEY_J               = 74,       // Key: J | j
    KEY_K               = 75,       // Key: K | k
    KEY_L               = 76,       // Key: L | l
    KEY_M               = 77,       // Key: M | m
    KEY_N               = 78,       // Key: N | n
    KEY_O               = 79,       // Key: O | o
    KEY_P               = 80,       // Key: P | p
    KEY_Q               = 81,       // Key: Q | q
    KEY_R               = 82,       // Key: R | r
    KEY_S               = 83,       // Key: S | s
    KEY_T               = 84,       // Key: T | t
    KEY_U               = 85,       // Key: U | u
    KEY_V               = 86,       // Key: V | v
    KEY_W               = 87,       // Key: W | w
    KEY_X               = 88,       // Key: X | x
    KEY_Y               = 89,       // Key: Y | y
    KEY_Z               = 90,       // Key: Z | z
    KEY_LEFT_BRACKET    = 91,       // Key: [
    KEY_BACKSLASH       = 92,       // Key: '\'
    KEY_RIGHT_BRACKET   = 93,       // Key: ]
    KEY_GRAVE           = 96,       // Key: `
    // Function keys
    KEY_SPACE           = 32,       // Key: Space
    KEY_ESCAPE          = 256,      // Key: Esc
    KEY_ENTER           = 257,      // Key: Enter
    KEY_TAB             = 258,      // Key: Tab
    KEY_BACKSPACE       = 259,      // Key: Backspace
    KEY_INSERT          = 260,      // Key: Ins
    KEY_DELETE          = 261,      // Key: Del
    KEY_RIGHT           = 262,      // Key: Cursor right
    KEY_LEFT            = 263,      // Key: Cursor left
    KEY_DOWN            = 264,      // Key: Cursor down
    KEY_UP              = 265,      // Key: Cursor up
    KEY_PAGE_UP         = 266,      // Key: Page up
    KEY_PAGE_DOWN       = 267,      // Key: Page down
    KEY_HOME            = 268,      // Key: Home
    KEY_END             = 269,      // Key: End
    KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    KEY_NUM_LOCK        = 282,      // Key: Num lock
    KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    KEY_PAUSE           = 284,      // Key: Pause
    KEY_F1              = 290,      // Key: F1
    KEY_F2              = 291,      // Key: F2
    KEY_F3              = 292,      // Key: F3
    KEY_F4              = 293,      // Key: F4
    KEY_F5              = 294,      // Key: F5
    KEY_F6              = 295,      // Key: F6
    KEY_F7              = 296,      // Key: F7
    KEY_F8              = 297,      // Key: F8
    KEY_F9              = 298,      // Key: F9
    KEY_F10             = 299,      // Key: F10
    KEY_F11             = 300,      // Key: F11
    KEY_F12             = 301,      // Key: F12
    KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    KEY_LEFT_CONTROL    = 341,      // Key: Control left
    KEY_LEFT_ALT        = 342,      // Key: Alt left
    KEY_LEFT_SUPER      = 343,      // Key: Super left
    KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    KEY_RIGHT_ALT       = 346,      // Key: Alt right
    KEY_RIGHT_SUPER     = 347,      // Key: Super right
    KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    KEY_KP_0            = 320,      // Key: Keypad 0
    KEY_KP_1            = 321,      // Key: Keypad 1
    KEY_KP_2            = 322,      // Key: Keypad 2
    KEY_KP_3            = 323,      // Key: Keypad 3
    KEY_KP_4            = 324,      // Key: Keypad 4
    KEY_KP_5            = 325,      // Key: Keypad 5
    KEY_KP_6            = 326,      // Key: Keypad 6
    KEY_KP_7            = 327,      // Key: Keypad 7
    KEY_KP_8            = 328,      // Key: Keypad 8
    KEY_KP_9            = 329,      // Key: Keypad 9
    KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    KEY_KP_ADD          = 334,      // Key: Keypad +
    KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    KEY_BACK            = 4,        // Key: Android back button
    KEY_MENU            = 5,        // Key: Android menu button
    KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
} rl_KeyboardKey;

// Add backwards compatibility support for deprecated names
#define rl_MOUSE_LEFT_BUTTON   MOUSE_BUTTON_LEFT
#define rl_MOUSE_RIGHT_BUTTON  MOUSE_BUTTON_RIGHT
#define rl_MOUSE_MIDDLE_BUTTON MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum {
    MOUSE_BUTTON_LEFT    = 0,       // Mouse button left
    MOUSE_BUTTON_RIGHT   = 1,       // Mouse button right
    MOUSE_BUTTON_MIDDLE  = 2,       // Mouse button middle (pressed wheel)
    MOUSE_BUTTON_SIDE    = 3,       // Mouse button side (advanced mouse device)
    MOUSE_BUTTON_EXTRA   = 4,       // Mouse button extra (advanced mouse device)
    MOUSE_BUTTON_FORWARD = 5,       // Mouse button forward (advanced mouse device)
    MOUSE_BUTTON_BACK    = 6,       // Mouse button back (advanced mouse device)
} rl_MouseButton;

// Mouse cursor
typedef enum {
    MOUSE_CURSOR_DEFAULT       = 0,     // Default pointer shape
    MOUSE_CURSOR_ARROW         = 1,     // Arrow shape
    MOUSE_CURSOR_IBEAM         = 2,     // Text writing cursor shape
    MOUSE_CURSOR_CROSSHAIR     = 3,     // Cross shape
    MOUSE_CURSOR_POINTING_HAND = 4,     // Pointing hand cursor
    MOUSE_CURSOR_RESIZE_EW     = 5,     // Horizontal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NS     = 6,     // Vertical resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NWSE   = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NESW   = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_ALL    = 9,     // The omnidirectional resize/move cursor shape
    MOUSE_CURSOR_NOT_ALLOWED   = 10     // The operation-not-allowed shape
} rl_MouseCursor;

// Gamepad buttons
typedef enum {
    GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Square, Xbox: X)
    GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (first), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} rl_GamepadButton;

// Gamepad axes
typedef enum {
    GAMEPAD_AXIS_LEFT_X        = 0,     // Gamepad left stick X axis
    GAMEPAD_AXIS_LEFT_Y        = 1,     // Gamepad left stick Y axis
    GAMEPAD_AXIS_RIGHT_X       = 2,     // Gamepad right stick X axis
    GAMEPAD_AXIS_RIGHT_Y       = 3,     // Gamepad right stick Y axis
    GAMEPAD_AXIS_LEFT_TRIGGER  = 4,     // Gamepad back trigger left, pressure level: [1..-1]
    GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} rl_GamepadAxis;

// rl_Material map index
typedef enum {
    MATERIAL_MAP_ALBEDO = 0,        // Albedo material (same as: rl_MATERIAL_MAP_DIFFUSE)
    MATERIAL_MAP_METALNESS,         // Metalness material (same as: rl_MATERIAL_MAP_SPECULAR)
    MATERIAL_MAP_NORMAL,            // Normal material
    MATERIAL_MAP_ROUGHNESS,         // Roughness material
    MATERIAL_MAP_OCCLUSION,         // Ambient occlusion material
    MATERIAL_MAP_EMISSION,          // Emission material
    MATERIAL_MAP_HEIGHT,            // Heightmap material
    MATERIAL_MAP_CUBEMAP,           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_IRRADIANCE,        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_PREFILTER,         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_BRDF               // Brdf material
} rl_MaterialMapIndex;

#define rl_MATERIAL_MAP_DIFFUSE      MATERIAL_MAP_ALBEDO
#define rl_MATERIAL_MAP_SPECULAR     MATERIAL_MAP_METALNESS

// rl_Shader location index
typedef enum {
    SHADER_LOC_VERTEX_POSITION = 0, // rl_Shader location: vertex attribute: position
    SHADER_LOC_VERTEX_TEXCOORD01,   // rl_Shader location: vertex attribute: texcoord01
    SHADER_LOC_VERTEX_TEXCOORD02,   // rl_Shader location: vertex attribute: texcoord02
    SHADER_LOC_VERTEX_NORMAL,       // rl_Shader location: vertex attribute: normal
    SHADER_LOC_VERTEX_TANGENT,      // rl_Shader location: vertex attribute: tangent
    SHADER_LOC_VERTEX_COLOR,        // rl_Shader location: vertex attribute: color
    SHADER_LOC_MATRIX_MVP,          // rl_Shader location: matrix uniform: model-view-projection
    SHADER_LOC_MATRIX_VIEW,         // rl_Shader location: matrix uniform: view (camera transform)
    SHADER_LOC_MATRIX_PROJECTION,   // rl_Shader location: matrix uniform: projection
    SHADER_LOC_MATRIX_MODEL,        // rl_Shader location: matrix uniform: model (transform)
    SHADER_LOC_MATRIX_NORMAL,       // rl_Shader location: matrix uniform: normal
    SHADER_LOC_VECTOR_VIEW,         // rl_Shader location: vector uniform: view
    SHADER_LOC_COLOR_DIFFUSE,       // rl_Shader location: vector uniform: diffuse color
    SHADER_LOC_COLOR_SPECULAR,      // rl_Shader location: vector uniform: specular color
    SHADER_LOC_COLOR_AMBIENT,       // rl_Shader location: vector uniform: ambient color
    SHADER_LOC_MAP_ALBEDO,          // rl_Shader location: sampler2d texture: albedo (same as: rl_SHADER_LOC_MAP_DIFFUSE)
    SHADER_LOC_MAP_METALNESS,       // rl_Shader location: sampler2d texture: metalness (same as: rl_SHADER_LOC_MAP_SPECULAR)
    SHADER_LOC_MAP_NORMAL,          // rl_Shader location: sampler2d texture: normal
    SHADER_LOC_MAP_ROUGHNESS,       // rl_Shader location: sampler2d texture: roughness
    SHADER_LOC_MAP_OCCLUSION,       // rl_Shader location: sampler2d texture: occlusion
    SHADER_LOC_MAP_EMISSION,        // rl_Shader location: sampler2d texture: emission
    SHADER_LOC_MAP_HEIGHT,          // rl_Shader location: sampler2d texture: height
    SHADER_LOC_MAP_CUBEMAP,         // rl_Shader location: samplerCube texture: cubemap
    SHADER_LOC_MAP_IRRADIANCE,      // rl_Shader location: samplerCube texture: irradiance
    SHADER_LOC_MAP_PREFILTER,       // rl_Shader location: samplerCube texture: prefilter
    SHADER_LOC_MAP_BRDF,            // rl_Shader location: sampler2d texture: brdf
    SHADER_LOC_VERTEX_BONEIDS,      // rl_Shader location: vertex attribute: boneIds
    SHADER_LOC_VERTEX_BONEWEIGHTS,  // rl_Shader location: vertex attribute: boneWeights
    SHADER_LOC_BONE_MATRICES,       // rl_Shader location: array of matrices uniform: boneMatrices
    SHADER_LOC_VERTEX_INSTANCE_TX   // rl_Shader location: vertex attribute: instanceTransform
} rl_ShaderLocationIndex;

#define rl_SHADER_LOC_MAP_DIFFUSE      SHADER_LOC_MAP_ALBEDO
#define rl_SHADER_LOC_MAP_SPECULAR     SHADER_LOC_MAP_METALNESS

// rl_Shader uniform data type
typedef enum {
    SHADER_UNIFORM_FLOAT = 0,       // rl_Shader uniform type: float
    SHADER_UNIFORM_VEC2,            // rl_Shader uniform type: vec2 (2 float)
    SHADER_UNIFORM_VEC3,            // rl_Shader uniform type: vec3 (3 float)
    SHADER_UNIFORM_VEC4,            // rl_Shader uniform type: vec4 (4 float)
    SHADER_UNIFORM_INT,             // rl_Shader uniform type: int
    SHADER_UNIFORM_IVEC2,           // rl_Shader uniform type: ivec2 (2 int)
    SHADER_UNIFORM_IVEC3,           // rl_Shader uniform type: ivec3 (3 int)
    SHADER_UNIFORM_IVEC4,           // rl_Shader uniform type: ivec4 (4 int)
    SHADER_UNIFORM_UINT,            // rl_Shader uniform type: unsigned int
    SHADER_UNIFORM_UIVEC2,          // rl_Shader uniform type: uivec2 (2 unsigned int)
    SHADER_UNIFORM_UIVEC3,          // rl_Shader uniform type: uivec3 (3 unsigned int)
    SHADER_UNIFORM_UIVEC4,          // rl_Shader uniform type: uivec4 (4 unsigned int)
    SHADER_UNIFORM_SAMPLER2D        // rl_Shader uniform type: sampler2d
} rl_ShaderUniformDataType;

// rl_Shader attribute data types
typedef enum {
    SHADER_ATTRIB_FLOAT = 0,        // rl_Shader attribute type: float
    SHADER_ATTRIB_VEC2,             // rl_Shader attribute type: vec2 (2 float)
    SHADER_ATTRIB_VEC3,             // rl_Shader attribute type: vec3 (3 float)
    SHADER_ATTRIB_VEC4              // rl_Shader attribute type: vec4 (4 float)
} rl_ShaderAttributeDataType;

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum {
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} rl_PixelFormat;

// rl_Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    TEXTURE_FILTER_POINT = 0,               // No filter, just pixel approximation
    TEXTURE_FILTER_BILINEAR,                // Linear filtering
    TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
    TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
    TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
    TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} rl_TextureFilter;

// rl_Texture parameters: wrap mode
typedef enum {
    TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
    TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
    TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
    TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
} rl_TextureWrap;

// Cubemap layouts
typedef enum {
    CUBEMAP_LAYOUT_AUTO_DETECT = 0,         // Automatically detect layout type
    CUBEMAP_LAYOUT_LINE_VERTICAL,           // Layout is defined by a vertical line with faces
    CUBEMAP_LAYOUT_LINE_HORIZONTAL,         // Layout is defined by a horizontal line with faces
    CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     // Layout is defined by a 3x4 cross with cubemap faces
    CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE     // Layout is defined by a 4x3 cross with cubemap faces
} rl_CubemapLayout;

// rl_Font type, defines generation method
typedef enum {
    FONT_DEFAULT = 0,               // Default font generation, anti-aliased
    FONT_BITMAP,                    // Bitmap font generation, no anti-aliasing
    FONT_SDF                        // SDF font generation, requires external shader
} rl_FontType;

// rl_Color blending modes (pre-defined)
typedef enum {
    BLEND_ALPHA = 0,                // Blend textures considering alpha (default)
    BLEND_ADDITIVE,                 // Blend textures adding colors
    BLEND_MULTIPLIED,               // Blend textures multiplying colors
    BLEND_ADD_COLORS,               // Blend textures adding colors (alternative)
    BLEND_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
    BLEND_ALPHA_PREMULTIPLY,        // Blend premultiplied textures considering alpha
    BLEND_CUSTOM,                   // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    BLEND_CUSTOM_SEPARATE           // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
} rl_BlendMode;

// rl_Gesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
typedef enum {
    GESTURE_NONE        = 0,        // No gesture
    GESTURE_TAP         = 1,        // Tap gesture
    GESTURE_DOUBLETAP   = 2,        // Double tap gesture
    GESTURE_HOLD        = 4,        // Hold gesture
    GESTURE_DRAG        = 8,        // Drag gesture
    GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
    GESTURE_SWIPE_LEFT  = 32,       // Swipe left gesture
    GESTURE_SWIPE_UP    = 64,       // Swipe up gesture
    GESTURE_SWIPE_DOWN  = 128,      // Swipe down gesture
    GESTURE_PINCH_IN    = 256,      // Pinch in gesture
    GESTURE_PINCH_OUT   = 512       // Pinch out gesture
} rl_Gesture;

// rl_Camera system modes
typedef enum {
    CAMERA_CUSTOM = 0,              // rl_Camera custom, controlled by user (rl_UpdateCamera() does nothing)
    CAMERA_FREE,                    // rl_Camera free mode
    CAMERA_ORBITAL,                 // rl_Camera orbital, around target, zoom supported
    CAMERA_FIRST_PERSON,            // rl_Camera first person
    CAMERA_THIRD_PERSON             // rl_Camera third person
} rl_CameraMode;

// rl_Camera projection
typedef enum {
    CAMERA_PERSPECTIVE = 0,         // Perspective projection
    CAMERA_ORTHOGRAPHIC             // Orthographic projection
} rl_CameraProjection;

// N-patch layout
typedef enum {
    NPATCH_NINE_PATCH = 0,          // Npatch layout: 3x3 tiles
    NPATCH_THREE_PATCH_VERTICAL,    // Npatch layout: 1x3 tiles
    NPATCH_THREE_PATCH_HORIZONTAL   // Npatch layout: 3x1 tiles
} rl_NPatchLayout;

// Callbacks to hook some internal functions
// WARNING: These callbacks are intended for advanced users
typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages
typedef unsigned char *(*LoadFileDataCallback)(const char *fileName, int *dataSize);    // FileIO: Load binary data
typedef bool (*SaveFileDataCallback)(const char *fileName, void *data, int dataSize);   // FileIO: Save binary data
typedef char *(*LoadFileTextCallback)(const char *fileName);            // FileIO: Load text data
typedef bool (*SaveFileTextCallback)(const char *fileName, const char *text); // FileIO: Save text data

//------------------------------------------------------------------------------------
// Global Variables Definition
//------------------------------------------------------------------------------------
// It's lonely here...

//------------------------------------------------------------------------------------
// Window and Graphics Device Functions (Module: core)
//------------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

typedef void (*rl_SDLEventCallback)(void *sdl_event);

rl_RLAPI void* rl_GetWindowSDL();
rl_RLAPI void* rl_GetOpenGLContext();
rl_RLAPI void rl_RegisterSDLEventCallback(rl_SDLEventCallback callback);
rl_RLAPI void rl_UnregisterSDLEventCallback(rl_SDLEventCallback callback);

// Window-related functions
rl_RLAPI void rl_InitWindow(int width, int height, const char *title);  // Initialize window and OpenGL context
rl_RLAPI void rl_CloseWindow(void);                                     // Close window and unload OpenGL context
rl_RLAPI bool rl_WindowShouldClose(void);                               // Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
rl_RLAPI bool rl_IsWindowReady(void);                                   // Check if window has been initialized successfully
rl_RLAPI bool rl_IsWindowFullscreen(void);                              // Check if window is currently fullscreen
rl_RLAPI bool rl_IsWindowHidden(void);                                  // Check if window is currently hidden
rl_RLAPI bool rl_IsWindowMinimized(void);                               // Check if window is currently minimized
rl_RLAPI bool rl_IsWindowMaximized(void);                               // Check if window is currently maximized
rl_RLAPI bool rl_IsWindowFocused(void);                                 // Check if window is currently focused
rl_RLAPI bool rl_IsWindowResized(void);                                 // Check if window has been resized last frame
rl_RLAPI bool rl_IsWindowState(unsigned int flag);                      // Check if one specific window flag is enabled
rl_RLAPI void rl_SetWindowState(unsigned int flags);                    // Set window configuration state using flags
rl_RLAPI void rl_ClearWindowState(unsigned int flags);                  // Clear window configuration state flags
rl_RLAPI void rl_ToggleFullscreen(void);                                // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
rl_RLAPI void rl_ToggleBorderlessWindowed(void);                        // Toggle window state: borderless windowed, resizes window to match monitor resolution
rl_RLAPI void rl_MaximizeWindow(void);                                  // Set window state: maximized, if resizable
rl_RLAPI void rl_MinimizeWindow(void);                                  // Set window state: minimized, if resizable
rl_RLAPI void rl_RestoreWindow(void);                                   // Restore window from being minimized/maximized
rl_RLAPI void rl_SetWindowIcon(rl_Image image);                            // Set icon for window (single image, RGBA 32bit)
rl_RLAPI void rl_SetWindowIcons(rl_Image *images, int count);              // Set icon for window (multiple images, RGBA 32bit)
rl_RLAPI void rl_SetWindowTitle(const char *title);                     // Set title for window
rl_RLAPI void rl_SetWindowPosition(int x, int y);                       // Set window position on screen
rl_RLAPI void rl_SetWindowMonitor(int monitor);                         // Set monitor for the current window
rl_RLAPI void rl_SetWindowMinSize(int width, int height);               // Set window minimum dimensions (for FLAG_WINDOW_RESIZABLE)
rl_RLAPI void rl_SetWindowMaxSize(int width, int height);               // Set window maximum dimensions (for FLAG_WINDOW_RESIZABLE)
rl_RLAPI void rl_SetWindowSize(int width, int height);                  // Set window dimensions
rl_RLAPI void rl_SetWindowOpacity(float opacity);                       // Set window opacity [0.0f..1.0f]
rl_RLAPI void rl_SetWindowFocused(void);                                // Set window focused
rl_RLAPI void *rl_GetWindowHandle(void);                                // Get native window handle
rl_RLAPI int rl_GetScreenWidth(void);                                   // Get current screen width
rl_RLAPI int rl_GetScreenHeight(void);                                  // Get current screen height
rl_RLAPI int rl_GetRenderWidth(void);                                   // Get current render width (it considers HiDPI)
rl_RLAPI int rl_GetRenderHeight(void);                                  // Get current render height (it considers HiDPI)
rl_RLAPI int rl_GetMonitorCount(void);                                  // Get number of connected monitors
rl_RLAPI int rl_GetCurrentMonitor(void);                                // Get current monitor where window is placed
rl_RLAPI rl_Vector2 rl_GetMonitorPosition(int monitor);                    // Get specified monitor position
rl_RLAPI int rl_GetMonitorWidth(int monitor);                           // Get specified monitor width (current video mode used by monitor)
rl_RLAPI int rl_GetMonitorHeight(int monitor);                          // Get specified monitor height (current video mode used by monitor)
rl_RLAPI int rl_GetMonitorPhysicalWidth(int monitor);                   // Get specified monitor physical width in millimetres
rl_RLAPI int rl_GetMonitorPhysicalHeight(int monitor);                  // Get specified monitor physical height in millimetres
rl_RLAPI int rl_GetMonitorRefreshRate(int monitor);                     // Get specified monitor refresh rate
rl_RLAPI rl_Vector2 rl_GetWindowPosition(void);                            // Get window position XY on monitor
rl_RLAPI rl_Vector2 rl_GetWindowScaleDPI(void);                            // Get window scale DPI factor
rl_RLAPI const char *rl_GetMonitorName(int monitor);                    // Get the human-readable, UTF-8 encoded name of the specified monitor
rl_RLAPI void rl_SetClipboardText(const char *text);                    // Set clipboard text content
rl_RLAPI const char *rl_GetClipboardText(void);                         // Get clipboard text content
rl_RLAPI rl_Image rl_GetClipboardImage(void);                              // Get clipboard image content
rl_RLAPI void rl_EnableEventWaiting(void);                              // Enable waiting for events on rl_EndDrawing(), no automatic event polling
rl_RLAPI void rl_DisableEventWaiting(void);                             // Disable waiting for events on rl_EndDrawing(), automatic events polling

// Cursor-related functions
rl_RLAPI void rl_ShowCursor(void);                                      // Shows cursor
rl_RLAPI void rl_HideCursor(void);                                      // Hides cursor
rl_RLAPI bool rl_IsCursorHidden(void);                                  // Check if cursor is not visible
rl_RLAPI void rl_EnableCursor(void);                                    // Enables cursor (unlock cursor)
rl_RLAPI void rl_DisableCursor(void);                                   // Disables cursor (lock cursor)
rl_RLAPI bool rl_IsCursorOnScreen(void);                                // Check if cursor is on the screen

// Drawing-related functions
rl_RLAPI void rl_ClearBackground(rl_Color color);                          // Set background color (framebuffer clear color)
rl_RLAPI void rl_BeginDrawing(void);                                    // Setup canvas (framebuffer) to start drawing
rl_RLAPI void rl_EndDrawing(void);                                      // End canvas drawing and swap buffers (double buffering)
rl_RLAPI void rl_BeginMode2D(rl_Camera2D camera);                          // Begin 2D mode with custom camera (2D)
rl_RLAPI void rl_EndMode2D(void);                                       // Ends 2D mode with custom camera
rl_RLAPI void rl_BeginMode3D(rl_Camera3D camera);                          // Begin 3D mode with custom camera (3D)
rl_RLAPI void rl_EndMode3D(void);                                       // Ends 3D mode and returns to default 2D orthographic mode
rl_RLAPI void rl_BeginTextureMode(rl_RenderTexture2D target);              // Begin drawing to render texture
rl_RLAPI void rl_EndTextureMode(void);                                  // Ends drawing to render texture
rl_RLAPI void rl_BeginShaderMode(rl_Shader shader);                        // Begin custom shader drawing
rl_RLAPI void rl_EndShaderMode(void);                                   // End custom shader drawing (use default shader)
rl_RLAPI void rl_BeginBlendMode(int mode);                              // Begin blending mode (alpha, additive, multiplied, subtract, custom)
rl_RLAPI void rl_EndBlendMode(void);                                    // End blending mode (reset to default: alpha blending)
rl_RLAPI void rl_BeginScissorMode(int x, int y, int width, int height); // Begin scissor mode (define screen area for following drawing)
rl_RLAPI void rl_EndScissorMode(void);                                  // End scissor mode
rl_RLAPI void rl_BeginVrStereoMode(rl_VrStereoConfig config);              // Begin stereo rendering (requires VR simulator)
rl_RLAPI void rl_EndVrStereoMode(void);                                 // End stereo rendering (requires VR simulator)

// VR stereo config functions for VR simulator
rl_RLAPI rl_VrStereoConfig rl_LoadVrStereoConfig(rl_VrDeviceInfo device);     // Load VR stereo config for VR simulator device parameters
rl_RLAPI void rl_UnloadVrStereoConfig(rl_VrStereoConfig config);           // Unload VR stereo config

// rl_Shader management functions
// NOTE: rl_Shader functionality is not available on OpenGL 1.1
rl_RLAPI rl_Shader rl_LoadShader(const char *vsFileName, const char *fsFileName);   // Load shader from files and bind default locations
rl_RLAPI rl_Shader rl_LoadShaderFromMemory(const char *vsCode, const char *fsCode); // Load shader from code strings and bind default locations
rl_RLAPI bool rl_IsShaderValid(rl_Shader shader);                                   // Check if a shader is valid (loaded on GPU)
rl_RLAPI int rl_GetShaderLocation(rl_Shader shader, const char *uniformName);       // Get shader uniform location
rl_RLAPI int rl_GetShaderLocationAttrib(rl_Shader shader, const char *attribName);  // Get shader attribute location
rl_RLAPI void rl_SetShaderValue(rl_Shader shader, int locIndex, const void *value, int uniformType);               // Set shader uniform value
rl_RLAPI void rl_SetShaderValueV(rl_Shader shader, int locIndex, const void *value, int uniformType, int count);   // Set shader uniform value vector
rl_RLAPI void rl_SetShaderValueMatrix(rl_Shader shader, int locIndex, rl_Matrix mat);         // Set shader uniform value (matrix 4x4)
rl_RLAPI void rl_SetShaderValueTexture(rl_Shader shader, int locIndex, rl_Texture2D texture); // Set shader uniform value and bind the texture (sampler2d)
rl_RLAPI void rl_UnloadShader(rl_Shader shader);                                    // Unload shader from GPU memory (VRAM)

// Screen-space-related functions
#define rl_GetMouseRay rl_GetScreenToWorldRay     // Compatibility hack for previous raylib versions
rl_RLAPI rl_Ray rl_GetScreenToWorldRay(rl_Vector2 position, rl_Camera camera);         // Get a ray trace from screen position (i.e mouse)
rl_RLAPI rl_Ray rl_GetScreenToWorldRayEx(rl_Vector2 position, rl_Camera camera, int width, int height); // Get a ray trace from screen position (i.e mouse) in a viewport
rl_RLAPI rl_Vector2 rl_GetWorldToScreen(rl_Vector3 position, rl_Camera camera);        // Get the screen space position for a 3d world space position
rl_RLAPI rl_Vector2 rl_GetWorldToScreenEx(rl_Vector3 position, rl_Camera camera, int width, int height); // Get size position for a 3d world space position
rl_RLAPI rl_Vector2 rl_GetWorldToScreen2D(rl_Vector2 position, rl_Camera2D camera);    // Get the screen space position for a 2d camera world space position
rl_RLAPI rl_Vector2 rl_GetScreenToWorld2D(rl_Vector2 position, rl_Camera2D camera);    // Get the world space position for a 2d camera screen space position
rl_RLAPI rl_Matrix rl_GetCameraMatrix(rl_Camera camera);                            // Get camera transform matrix (view matrix)
rl_RLAPI rl_Matrix rl_GetCameraMatrix2D(rl_Camera2D camera);                        // Get camera 2d transform matrix

// Timing-related functions
rl_RLAPI void rl_SetTargetFPS(int fps);                                 // Set target FPS (maximum)
rl_RLAPI float rl_GetFrameTime(void);                                   // Get time in seconds for last frame drawn (delta time)
rl_RLAPI double rl_GetTime(void);                                       // Get elapsed time in seconds since rl_InitWindow()
rl_RLAPI int rl_GetFPS(void);                                           // Get current FPS

// Custom frame control functions
// NOTE: Those functions are intended for advanced users that want full control over the frame processing
// By default rl_EndDrawing() does this job: draws everything + rl_SwapScreenBuffer() + manage frame timing + rl_PollInputEvents()
// To avoid that behaviour and control frame processes manually, enable in config.h: SUPPORT_CUSTOM_FRAME_CONTROL
rl_RLAPI void rl_SwapScreenBuffer(void);                                // Swap back buffer with front buffer (screen drawing)
rl_RLAPI void rl_PollInputEvents(void);                                 // Register all input events
rl_RLAPI void rl_WaitTime(double seconds);                              // Wait for some time (halt program execution)

// Random values generation functions
rl_RLAPI void rl_SetRandomSeed(unsigned int seed);                      // Set the seed for the random number generator
rl_RLAPI int rl_GetRandomValue(int min, int max);                       // Get a random value between min and max (both included)
rl_RLAPI int *rl_LoadRandomSequence(unsigned int count, int min, int max); // Load random values sequence, no values repeated
rl_RLAPI void rl_UnloadRandomSequence(int *sequence);                   // Unload random values sequence

// Misc. functions
rl_RLAPI void rl_TakeScreenshot(const char *fileName);                  // Takes a screenshot of current screen (filename extension defines format)
rl_RLAPI void rl_SetConfigFlags(unsigned int flags);                    // Setup init configuration flags (view FLAGS)
rl_RLAPI void rl_OpenURL(const char *url);                              // Open URL with default system browser (if available)

// NOTE: Following functions implemented in module [utils]
//------------------------------------------------------------------
rl_RLAPI void rl_TraceLog(int logLevel, const char *text, ...);         // Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
rl_RLAPI void rl_SetTraceLogLevel(int logLevel);                        // Set the current threshold (minimum) log level
rl_RLAPI void *rl_MemAlloc(unsigned int size);                          // Internal memory allocator
rl_RLAPI void *rl_MemRealloc(void *ptr, unsigned int size);             // Internal memory reallocator
rl_RLAPI void rl_MemFree(void *ptr);                                    // Internal memory free

// Set custom callbacks
// WARNING: Callbacks setup is intended for advanced users
rl_RLAPI void rl_SetTraceLogCallback(TraceLogCallback callback);          // Set custom trace log
rl_RLAPI void rl_SetLoadFileDataCallback(LoadFileDataCallback callback);  // Set custom file binary data loader
rl_RLAPI void rl_SetSaveFileDataCallback(SaveFileDataCallback callback);  // Set custom file binary data saver
rl_RLAPI void rl_SetLoadFileTextCallback(LoadFileTextCallback callback);  // Set custom file text data loader
rl_RLAPI void rl_SetSaveFileTextCallback(SaveFileTextCallback callback);  // Set custom file text data saver

// Files management functions
rl_RLAPI unsigned char *rl_LoadFileData(const char *fileName, int *dataSize); // Load file data as byte array (read)
rl_RLAPI void rl_UnloadFileData(unsigned char *data);                     // Unload file data allocated by rl_LoadFileData()
rl_RLAPI bool rl_SaveFileData(const char *fileName, void *data, int dataSize); // Save data to file from byte array (write), returns true on success
rl_RLAPI bool rl_ExportDataAsCode(const unsigned char *data, int dataSize, const char *fileName); // Export data to code (.h), returns true on success
rl_RLAPI char *rl_LoadFileText(const char *fileName);                     // Load text data from file (read), returns a '\0' terminated string
rl_RLAPI void rl_UnloadFileText(char *text);                              // Unload file text data allocated by rl_LoadFileText()
rl_RLAPI bool rl_SaveFileText(const char *fileName, const char *text);    // Save text data to file (write), string must be '\0' terminated, returns true on success
//------------------------------------------------------------------

// File system functions
rl_RLAPI int rl_FileRename(const char *fileName, const char *fileRename); // Rename file (if exists)
rl_RLAPI int rl_FileRemove(const char *fileName);                         // Remove file (if exists)
rl_RLAPI int rl_FileCopy(const char *srcPath, const char *dstPath);       // Copy file from one path to another, dstPath created if it doesn't exist
rl_RLAPI int rl_FileMove(const char *srcPath, const char *dstPath);       // Move file from one directory to another, dstPath created if it doesn't exist
rl_RLAPI int rl_FileTextReplace(const char *fileName, const char *search, const char *replacement); // Replace text in an existing file
rl_RLAPI int rl_FileTextFindIndex(const char *fileName, const char *search); // Find text in existing file
rl_RLAPI bool rl_FileExists(const char *fileName);                        // Check if file exists
rl_RLAPI bool rl_DirectoryExists(const char *dirPath);                    // Check if a directory path exists
rl_RLAPI bool rl_IsFileExtension(const char *fileName, const char *ext);  // Check file extension (recommended include point: .png, .wav)
rl_RLAPI int rl_GetFileLength(const char *fileName);                      // Get file length in bytes (NOTE: GetFileSize() conflicts with windows.h)
rl_RLAPI long rl_GetFileModTime(const char *fileName);                    // Get file modification time (last write time)
rl_RLAPI const char *rl_GetFileExtension(const char *fileName);           // Get pointer to extension for a filename string (includes dot: '.png')
rl_RLAPI const char *rl_GetFileName(const char *filePath);                // Get pointer to filename for a path string
rl_RLAPI const char *rl_GetFileNameWithoutExt(const char *filePath);      // Get filename string without extension (uses static string)
rl_RLAPI const char *rl_GetDirectoryPath(const char *filePath);           // Get full path for a given fileName with path (uses static string)
rl_RLAPI const char *rl_GetPrevDirectoryPath(const char *dirPath);        // Get previous directory path for a given path (uses static string)
rl_RLAPI const char *rl_GetWorkingDirectory(void);                        // Get current working directory (uses static string)
rl_RLAPI const char *rl_GetApplicationDirectory(void);                    // Get the directory of the running application (uses static string)
rl_RLAPI int rl_MakeDirectory(const char *dirPath);                       // Create directories (including full path requested), returns 0 on success
rl_RLAPI bool rl_ChangeDirectory(const char *dirPath);                    // Change working directory, return true on success
rl_RLAPI bool rl_IsPathFile(const char *path);                            // Check if a given path is a file or a directory
rl_RLAPI bool rl_IsFileNameValid(const char *fileName);                   // Check if fileName is valid for the platform/OS
rl_RLAPI rl_FilePathList rl_LoadDirectoryFiles(const char *dirPath);         // Load directory filepaths
rl_RLAPI rl_FilePathList rl_LoadDirectoryFilesEx(const char *basePath, const char *filter, bool scanSubdirs); // Load directory filepaths with extension filtering and recursive directory scan. Use 'DIR' in the filter string to include directories in the result
rl_RLAPI void rl_UnloadDirectoryFiles(rl_FilePathList files);                // Unload filepaths
rl_RLAPI bool rl_IsFileDropped(void);                                     // Check if a file has been dropped into window
rl_RLAPI rl_FilePathList rl_LoadDroppedFiles(void);                          // Load dropped filepaths
rl_RLAPI void rl_UnloadDroppedFiles(rl_FilePathList files);                  // Unload dropped filepaths

// Compression/Encoding functionality
rl_RLAPI unsigned char *rl_CompressData(const unsigned char *data, int dataSize, int *compDataSize);        // Compress data (DEFLATE algorithm), memory must be rl_MemFree()
rl_RLAPI unsigned char *rl_DecompressData(const unsigned char *compData, int compDataSize, int *dataSize);  // Decompress data (DEFLATE algorithm), memory must be rl_MemFree()
rl_RLAPI char *rl_EncodeDataBase64(const unsigned char *data, int dataSize, int *outputSize);               // Encode data to Base64 string (includes NULL terminator), memory must be rl_MemFree()
rl_RLAPI unsigned char *rl_DecodeDataBase64(const char *text, int *outputSize);                             // Decode Base64 string (expected NULL terminated), memory must be rl_MemFree()
rl_RLAPI unsigned int rl_ComputeCRC32(unsigned char *data, int dataSize);       // Compute CRC32 hash code
rl_RLAPI unsigned int *rl_ComputeMD5(unsigned char *data, int dataSize);        // Compute MD5 hash code, returns static int[4] (16 bytes)
rl_RLAPI unsigned int *rl_ComputeSHA1(unsigned char *data, int dataSize);       // Compute SHA1 hash code, returns static int[5] (20 bytes)
rl_RLAPI unsigned int *rl_ComputeSHA256(unsigned char *data, int dataSize);     // Compute SHA256 hash code, returns static int[8] (32 bytes)

// Automation events functionality
rl_RLAPI rl_AutomationEventList rl_LoadAutomationEventList(const char *fileName); // Load automation events list from file, NULL for empty list, capacity = MAX_AUTOMATION_EVENTS
rl_RLAPI void rl_UnloadAutomationEventList(rl_AutomationEventList list);   // Unload automation events list from file
rl_RLAPI bool rl_ExportAutomationEventList(rl_AutomationEventList list, const char *fileName); // Export automation events list as text file
rl_RLAPI void rl_SetAutomationEventList(rl_AutomationEventList *list);     // Set automation event list to record to
rl_RLAPI void rl_SetAutomationEventBaseFrame(int frame);                // Set automation event internal base frame to start recording
rl_RLAPI void rl_StartAutomationEventRecording(void);                   // Start recording automation events (rl_AutomationEventList must be set)
rl_RLAPI void rl_StopAutomationEventRecording(void);                    // Stop recording automation events
rl_RLAPI void rl_PlayAutomationEvent(rl_AutomationEvent event);            // Play a recorded automation event

//------------------------------------------------------------------------------------
// Input Handling Functions (Module: core)
//------------------------------------------------------------------------------------

// Input-related functions: keyboard
rl_RLAPI bool rl_IsKeyPressed(int key);                             // Check if a key has been pressed once
rl_RLAPI bool rl_IsKeyPressedRepeat(int key);                       // Check if a key has been pressed again
rl_RLAPI bool rl_IsKeyDown(int key);                                // Check if a key is being pressed
rl_RLAPI bool rl_IsKeyReleased(int key);                            // Check if a key has been released once
rl_RLAPI bool rl_IsKeyUp(int key);                                  // Check if a key is NOT being pressed
rl_RLAPI int rl_GetKeyPressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
rl_RLAPI int rl_GetCharPressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
rl_RLAPI const char *rl_GetKeyName(int key);                        // Get name of a QWERTY key on the current keyboard layout (eg returns string 'q' for KEY_A on an AZERTY keyboard)
rl_RLAPI void rl_SetExitKey(int key);                               // Set a custom key to exit program (default is ESC)

// Input-related functions: gamepads
rl_RLAPI bool rl_IsGamepadAvailable(int gamepad);                   // Check if a gamepad is available
rl_RLAPI const char *rl_GetGamepadName(int gamepad);                // Get gamepad internal name id
rl_RLAPI bool rl_IsGamepadButtonPressed(int gamepad, int button);   // Check if a gamepad button has been pressed once
rl_RLAPI bool rl_IsGamepadButtonDown(int gamepad, int button);      // Check if a gamepad button is being pressed
rl_RLAPI bool rl_IsGamepadButtonReleased(int gamepad, int button);  // Check if a gamepad button has been released once
rl_RLAPI bool rl_IsGamepadButtonUp(int gamepad, int button);        // Check if a gamepad button is NOT being pressed
rl_RLAPI int rl_GetGamepadButtonPressed(void);                      // Get the last gamepad button pressed
rl_RLAPI int rl_GetGamepadAxisCount(int gamepad);                   // Get axis count for a gamepad
rl_RLAPI float rl_GetGamepadAxisMovement(int gamepad, int axis);    // Get movement value for a gamepad axis
rl_RLAPI int rl_SetGamepadMappings(const char *mappings);           // Set internal gamepad mappings (SDL_GameControllerDB)
rl_RLAPI void rl_SetGamepadVibration(int gamepad, float leftMotor, float rightMotor, float duration); // Set gamepad vibration for both motors (duration in seconds)

// Input-related functions: mouse
rl_RLAPI bool rl_IsMouseButtonPressed(int button);                  // Check if a mouse button has been pressed once
rl_RLAPI bool rl_IsMouseButtonDown(int button);                     // Check if a mouse button is being pressed
rl_RLAPI bool rl_IsMouseButtonReleased(int button);                 // Check if a mouse button has been released once
rl_RLAPI bool rl_IsMouseButtonUp(int button);                       // Check if a mouse button is NOT being pressed
rl_RLAPI int rl_GetMouseX(void);                                    // Get mouse position X
rl_RLAPI int rl_GetMouseY(void);                                    // Get mouse position Y
rl_RLAPI rl_Vector2 rl_GetMousePosition(void);                         // Get mouse position XY
rl_RLAPI rl_Vector2 rl_GetMouseDelta(void);                            // Get mouse delta between frames
rl_RLAPI void rl_SetMousePosition(int x, int y);                    // Set mouse position XY
rl_RLAPI void rl_SetMouseOffset(int offsetX, int offsetY);          // Set mouse offset
rl_RLAPI void rl_SetMouseScale(float scaleX, float scaleY);         // Set mouse scaling
rl_RLAPI float rl_GetMouseWheelMove(void);                          // Get mouse wheel movement for X or Y, whichever is larger
rl_RLAPI rl_Vector2 rl_GetMouseWheelMoveV(void);                       // Get mouse wheel movement for both X and Y
rl_RLAPI void rl_SetMouseCursor(int cursor);                        // Set mouse cursor

// Input-related functions: touch
rl_RLAPI int rl_GetTouchX(void);                                    // Get touch position X for touch point 0 (relative to screen size)
rl_RLAPI int rl_GetTouchY(void);                                    // Get touch position Y for touch point 0 (relative to screen size)
rl_RLAPI rl_Vector2 rl_GetTouchPosition(int index);                    // Get touch position XY for a touch point index (relative to screen size)
rl_RLAPI int rl_GetTouchPointId(int index);                         // Get touch point identifier for given index
rl_RLAPI int rl_GetTouchPointCount(void);                           // Get number of touch points

//------------------------------------------------------------------------------------
// Gestures and Touch Handling Functions (Module: rgestures)
//------------------------------------------------------------------------------------
rl_RLAPI void rl_SetGesturesEnabled(unsigned int flags);            // Enable a set of gestures using flags
rl_RLAPI bool rl_IsGestureDetected(unsigned int gesture);           // Check if a gesture have been detected
rl_RLAPI int rl_GetGestureDetected(void);                           // Get latest detected gesture
rl_RLAPI float rl_GetGestureHoldDuration(void);                     // Get gesture hold time in seconds
rl_RLAPI rl_Vector2 rl_GetGestureDragVector(void);                     // Get gesture drag vector
rl_RLAPI float rl_GetGestureDragAngle(void);                        // Get gesture drag angle
rl_RLAPI rl_Vector2 rl_GetGesturePinchVector(void);                    // Get gesture pinch delta
rl_RLAPI float rl_GetGesturePinchAngle(void);                       // Get gesture pinch angle

//------------------------------------------------------------------------------------
// rl_Camera System Functions (Module: rcamera)
//------------------------------------------------------------------------------------
rl_RLAPI void rl_UpdateCamera(rl_Camera *camera, int mode);            // Update camera position for selected mode
rl_RLAPI void rl_UpdateCameraPro(rl_Camera *camera, rl_Vector3 movement, rl_Vector3 rotation, float zoom); // Update camera movement/rotation

//------------------------------------------------------------------------------------
// Basic Shapes Drawing Functions (Module: shapes)
//------------------------------------------------------------------------------------
// Set texture and rectangle to be used on shapes drawing
// NOTE: It can be useful when using basic shapes and one single font,
// defining a font char white rectangle would allow drawing everything in a single draw call
rl_RLAPI void rl_SetShapesTexture(rl_Texture2D texture, rl_Rectangle source); // Set texture and rectangle to be used on shapes drawing
rl_RLAPI rl_Texture2D rl_GetShapesTexture(void);                 // Get texture that is used for shapes drawing
rl_RLAPI rl_Rectangle rl_GetShapesTextureRectangle(void);        // Get texture source rectangle that is used for shapes drawing

// Basic shapes drawing functions
rl_RLAPI void rl_DrawPixel(int posX, int posY, rl_Color color);                                                   // Draw a pixel using geometry [Can be slow, use with care]
rl_RLAPI void rl_DrawPixelV(rl_Vector2 position, rl_Color color);                                                    // Draw a pixel using geometry (Vector version) [Can be slow, use with care]
rl_RLAPI void rl_DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, rl_Color color);                // Draw a line
rl_RLAPI void rl_DrawLineV(rl_Vector2 startPos, rl_Vector2 endPos, rl_Color color);                                     // Draw a line (using gl lines)
rl_RLAPI void rl_DrawLineEx(rl_Vector2 startPos, rl_Vector2 endPos, float thick, rl_Color color);                       // Draw a line (using triangles/quads)
rl_RLAPI void rl_DrawLineStrip(const rl_Vector2 *points, int pointCount, rl_Color color);                            // Draw lines sequence (using gl lines)
rl_RLAPI void rl_DrawLineBezier(rl_Vector2 startPos, rl_Vector2 endPos, float thick, rl_Color color);                   // Draw line segment cubic-bezier in-out interpolation
rl_RLAPI void rl_DrawLineDashed(rl_Vector2 startPos, rl_Vector2 endPos, int dashSize, int spaceSize, rl_Color color);   // Draw a dashed line
rl_RLAPI void rl_DrawCircle(int centerX, int centerY, float radius, rl_Color color);                              // Draw a color-filled circle
rl_RLAPI void rl_DrawCircleSector(rl_Vector2 center, float radius, float startAngle, float endAngle, int segments, rl_Color color);      // Draw a piece of a circle
rl_RLAPI void rl_DrawCircleSectorLines(rl_Vector2 center, float radius, float startAngle, float endAngle, int segments, rl_Color color); // Draw circle sector outline
rl_RLAPI void rl_DrawCircleGradient(int centerX, int centerY, float radius, rl_Color inner, rl_Color outer);         // Draw a gradient-filled circle
rl_RLAPI void rl_DrawCircleV(rl_Vector2 center, float radius, rl_Color color);                                       // Draw a color-filled circle (Vector version)
rl_RLAPI void rl_DrawCircleLines(int centerX, int centerY, float radius, rl_Color color);                         // Draw circle outline
rl_RLAPI void rl_DrawCircleLinesV(rl_Vector2 center, float radius, rl_Color color);                                  // Draw circle outline (Vector version)
rl_RLAPI void rl_DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, rl_Color color);             // Draw ellipse
rl_RLAPI void rl_DrawEllipseV(rl_Vector2 center, float radiusH, float radiusV, rl_Color color);                      // Draw ellipse (Vector version)
rl_RLAPI void rl_DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, rl_Color color);        // Draw ellipse outline
rl_RLAPI void rl_DrawEllipseLinesV(rl_Vector2 center, float radiusH, float radiusV, rl_Color color);                 // Draw ellipse outline (Vector version)
rl_RLAPI void rl_DrawRing(rl_Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, rl_Color color); // Draw ring
rl_RLAPI void rl_DrawRingLines(rl_Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, rl_Color color);    // Draw ring outline
rl_RLAPI void rl_DrawRectangle(int posX, int posY, int width, int height, rl_Color color);                        // Draw a color-filled rectangle
rl_RLAPI void rl_DrawRectangleV(rl_Vector2 position, rl_Vector2 size, rl_Color color);                                  // Draw a color-filled rectangle (Vector version)
rl_RLAPI void rl_DrawRectangleRec(rl_Rectangle rec, rl_Color color);                                                 // Draw a color-filled rectangle
rl_RLAPI void rl_DrawRectanglePro(rl_Rectangle rec, rl_Vector2 origin, float rotation, rl_Color color);                 // Draw a color-filled rectangle with pro parameters
rl_RLAPI void rl_DrawRectangleGradientV(int posX, int posY, int width, int height, rl_Color top, rl_Color bottom);   // Draw a vertical-gradient-filled rectangle
rl_RLAPI void rl_DrawRectangleGradientH(int posX, int posY, int width, int height, rl_Color left, rl_Color right);   // Draw a horizontal-gradient-filled rectangle
rl_RLAPI void rl_DrawRectangleGradientEx(rl_Rectangle rec, rl_Color topLeft, rl_Color bottomLeft, rl_Color bottomRight, rl_Color topRight); // Draw a gradient-filled rectangle with custom vertex colors
rl_RLAPI void rl_DrawRectangleLines(int posX, int posY, int width, int height, rl_Color color);                   // Draw rectangle outline
rl_RLAPI void rl_DrawRectangleLinesEx(rl_Rectangle rec, float lineThick, rl_Color color);                            // Draw rectangle outline with extended parameters
rl_RLAPI void rl_DrawRectangleRounded(rl_Rectangle rec, float roundness, int segments, rl_Color color);              // Draw rectangle with rounded edges
rl_RLAPI void rl_DrawRectangleRoundedLines(rl_Rectangle rec, float roundness, int segments, rl_Color color);         // Draw rectangle lines with rounded edges
rl_RLAPI void rl_DrawRectangleRoundedLinesEx(rl_Rectangle rec, float roundness, int segments, float lineThick, rl_Color color); // Draw rectangle with rounded edges outline
rl_RLAPI void rl_DrawTriangle(rl_Vector2 v1, rl_Vector2 v2, rl_Vector2 v3, rl_Color color);                                // Draw a color-filled triangle (vertex in counter-clockwise order!)
rl_RLAPI void rl_DrawTriangleLines(rl_Vector2 v1, rl_Vector2 v2, rl_Vector2 v3, rl_Color color);                           // Draw triangle outline (vertex in counter-clockwise order!)
rl_RLAPI void rl_DrawTriangleFan(const rl_Vector2 *points, int pointCount, rl_Color color);                          // Draw a triangle fan defined by points (first vertex is the center)
rl_RLAPI void rl_DrawTriangleStrip(const rl_Vector2 *points, int pointCount, rl_Color color);                        // Draw a triangle strip defined by points
rl_RLAPI void rl_DrawPoly(rl_Vector2 center, int sides, float radius, float rotation, rl_Color color);               // Draw a regular polygon (Vector version)
rl_RLAPI void rl_DrawPolyLines(rl_Vector2 center, int sides, float radius, float rotation, rl_Color color);          // Draw a polygon outline of n sides
rl_RLAPI void rl_DrawPolyLinesEx(rl_Vector2 center, int sides, float radius, float rotation, float lineThick, rl_Color color); // Draw a polygon outline of n sides with extended parameters

// Splines drawing functions
rl_RLAPI void rl_DrawSplineLinear(const rl_Vector2 *points, int pointCount, float thick, rl_Color color);            // Draw spline: Linear, minimum 2 points
rl_RLAPI void rl_DrawSplineBasis(const rl_Vector2 *points, int pointCount, float thick, rl_Color color);             // Draw spline: B-Spline, minimum 4 points
rl_RLAPI void rl_DrawSplineCatmullRom(const rl_Vector2 *points, int pointCount, float thick, rl_Color color);        // Draw spline: Catmull-Rom, minimum 4 points
rl_RLAPI void rl_DrawSplineBezierQuadratic(const rl_Vector2 *points, int pointCount, float thick, rl_Color color);   // Draw spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, c2, p3, c4...]
rl_RLAPI void rl_DrawSplineBezierCubic(const rl_Vector2 *points, int pointCount, float thick, rl_Color color);       // Draw spline: Cubic Bezier, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
rl_RLAPI void rl_DrawSplineSegmentLinear(rl_Vector2 p1, rl_Vector2 p2, float thick, rl_Color color);                    // Draw spline segment: Linear, 2 points
rl_RLAPI void rl_DrawSplineSegmentBasis(rl_Vector2 p1, rl_Vector2 p2, rl_Vector2 p3, rl_Vector2 p4, float thick, rl_Color color); // Draw spline segment: B-Spline, 4 points
rl_RLAPI void rl_DrawSplineSegmentCatmullRom(rl_Vector2 p1, rl_Vector2 p2, rl_Vector2 p3, rl_Vector2 p4, float thick, rl_Color color); // Draw spline segment: Catmull-Rom, 4 points
rl_RLAPI void rl_DrawSplineSegmentBezierQuadratic(rl_Vector2 p1, rl_Vector2 c2, rl_Vector2 p3, float thick, rl_Color color); // Draw spline segment: Quadratic Bezier, 2 points, 1 control point
rl_RLAPI void rl_DrawSplineSegmentBezierCubic(rl_Vector2 p1, rl_Vector2 c2, rl_Vector2 c3, rl_Vector2 p4, float thick, rl_Color color); // Draw spline segment: Cubic Bezier, 2 points, 2 control points

// Spline segment point evaluation functions, for a given t [0.0f .. 1.0f]
rl_RLAPI rl_Vector2 rl_GetSplinePointLinear(rl_Vector2 startPos, rl_Vector2 endPos, float t);                           // Get (evaluate) spline point: Linear
rl_RLAPI rl_Vector2 rl_GetSplinePointBasis(rl_Vector2 p1, rl_Vector2 p2, rl_Vector2 p3, rl_Vector2 p4, float t);              // Get (evaluate) spline point: B-Spline
rl_RLAPI rl_Vector2 rl_GetSplinePointCatmullRom(rl_Vector2 p1, rl_Vector2 p2, rl_Vector2 p3, rl_Vector2 p4, float t);         // Get (evaluate) spline point: Catmull-Rom
rl_RLAPI rl_Vector2 rl_GetSplinePointBezierQuad(rl_Vector2 p1, rl_Vector2 c2, rl_Vector2 p3, float t);                     // Get (evaluate) spline point: Quadratic Bezier
rl_RLAPI rl_Vector2 rl_GetSplinePointBezierCubic(rl_Vector2 p1, rl_Vector2 c2, rl_Vector2 c3, rl_Vector2 p4, float t);        // Get (evaluate) spline point: Cubic Bezier

// Basic shapes collision detection functions
rl_RLAPI bool rl_CheckCollisionRecs(rl_Rectangle rec1, rl_Rectangle rec2);                                           // Check collision between two rectangles
rl_RLAPI bool rl_CheckCollisionCircles(rl_Vector2 center1, float radius1, rl_Vector2 center2, float radius2);        // Check collision between two circles
rl_RLAPI bool rl_CheckCollisionCircleRec(rl_Vector2 center, float radius, rl_Rectangle rec);                         // Check collision between circle and rectangle
rl_RLAPI bool rl_CheckCollisionCircleLine(rl_Vector2 center, float radius, rl_Vector2 p1, rl_Vector2 p2);               // Check if circle collides with a line created betweeen two points [p1] and [p2]
rl_RLAPI bool rl_CheckCollisionPointRec(rl_Vector2 point, rl_Rectangle rec);                                         // Check if point is inside rectangle
rl_RLAPI bool rl_CheckCollisionPointCircle(rl_Vector2 point, rl_Vector2 center, float radius);                       // Check if point is inside circle
rl_RLAPI bool rl_CheckCollisionPointTriangle(rl_Vector2 point, rl_Vector2 p1, rl_Vector2 p2, rl_Vector2 p3);               // Check if point is inside a triangle
rl_RLAPI bool rl_CheckCollisionPointLine(rl_Vector2 point, rl_Vector2 p1, rl_Vector2 p2, int threshold);                // Check if point belongs to line created between two points [p1] and [p2] with defined margin in pixels [threshold]
rl_RLAPI bool rl_CheckCollisionPointPoly(rl_Vector2 point, const rl_Vector2 *points, int pointCount);                // Check if point is within a polygon described by array of vertices
rl_RLAPI bool rl_CheckCollisionLines(rl_Vector2 startPos1, rl_Vector2 endPos1, rl_Vector2 startPos2, rl_Vector2 endPos2, rl_Vector2 *collisionPoint); // Check the collision between two lines defined by two points each, returns collision point by reference
rl_RLAPI rl_Rectangle rl_GetCollisionRec(rl_Rectangle rec1, rl_Rectangle rec2);                                         // Get collision rectangle for two rectangles collision

//------------------------------------------------------------------------------------
// rl_Texture Loading and Drawing Functions (Module: textures)
//------------------------------------------------------------------------------------

// rl_Image loading functions
// NOTE: These functions do not require GPU access
rl_RLAPI rl_Image rl_LoadImage(const char *fileName);                                                             // Load image from file into CPU memory (RAM)
rl_RLAPI rl_Image rl_LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
rl_RLAPI rl_Image rl_LoadImageAnim(const char *fileName, int *frames);                                            // Load image sequence from file (frames appended to image.data)
rl_RLAPI rl_Image rl_LoadImageAnimFromMemory(const char *fileType, const unsigned char *fileData, int dataSize, int *frames); // Load image sequence from memory buffer
rl_RLAPI rl_Image rl_LoadImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
rl_RLAPI rl_Image rl_LoadImageFromTexture(rl_Texture2D texture);                                                     // Load image from GPU texture data
rl_RLAPI rl_Image rl_LoadImageFromScreen(void);                                                                   // Load image from screen buffer and (screenshot)
rl_RLAPI bool rl_IsImageValid(rl_Image image);                                                                    // Check if an image is valid (data and parameters)
rl_RLAPI void rl_UnloadImage(rl_Image image);                                                                     // Unload image from CPU memory (RAM)
rl_RLAPI bool rl_ExportImage(rl_Image image, const char *fileName);                                               // Export image data to file, returns true on success
rl_RLAPI unsigned char *rl_ExportImageToMemory(rl_Image image, const char *fileType, int *fileSize);              // Export image to memory buffer
rl_RLAPI bool rl_ExportImageAsCode(rl_Image image, const char *fileName);                                         // Export image as code file defining an array of bytes, returns true on success

// rl_Image generation functions
rl_RLAPI rl_Image rl_GenImageColor(int width, int height, rl_Color color);                                           // Generate image: plain color
rl_RLAPI rl_Image rl_GenImageGradientLinear(int width, int height, int direction, rl_Color start, rl_Color end);        // Generate image: linear gradient, direction in degrees [0..360], 0=Vertical gradient
rl_RLAPI rl_Image rl_GenImageGradientRadial(int width, int height, float density, rl_Color inner, rl_Color outer);      // Generate image: radial gradient
rl_RLAPI rl_Image rl_GenImageGradientSquare(int width, int height, float density, rl_Color inner, rl_Color outer);      // Generate image: square gradient
rl_RLAPI rl_Image rl_GenImageChecked(int width, int height, int checksX, int checksY, rl_Color col1, rl_Color col2);    // Generate image: checked
rl_RLAPI rl_Image rl_GenImageWhiteNoise(int width, int height, float factor);                                     // Generate image: white noise
rl_RLAPI rl_Image rl_GenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale);           // Generate image: perlin noise
rl_RLAPI rl_Image rl_GenImageCellular(int width, int height, int tileSize);                                       // Generate image: cellular algorithm, bigger tileSize means bigger cells
rl_RLAPI rl_Image rl_GenImageText(int width, int height, const char *text);                                       // Generate image: grayscale image from text data

// rl_Image manipulation functions
rl_RLAPI rl_Image rl_ImageCopy(rl_Image image);                                                                      // Create an image duplicate (useful for transformations)
rl_RLAPI rl_Image rl_ImageFromImage(rl_Image image, rl_Rectangle rec);                                                  // Create an image from another image piece
rl_RLAPI rl_Image rl_ImageFromChannel(rl_Image image, int selectedChannel);                                          // Create an image from a selected channel of another image (GRAYSCALE)
rl_RLAPI rl_Image rl_ImageText(const char *text, int fontSize, rl_Color color);                                      // Create an image from text (default font)
rl_RLAPI rl_Image rl_ImageTextEx(rl_Font font, const char *text, float fontSize, float spacing, rl_Color tint);         // Create an image from text (custom sprite font)
rl_RLAPI void rl_ImageFormat(rl_Image *image, int newFormat);                                                     // Convert image data to desired format
rl_RLAPI void rl_ImageToPOT(rl_Image *image, rl_Color fill);                                                         // Convert image to POT (power-of-two)
rl_RLAPI void rl_ImageCrop(rl_Image *image, rl_Rectangle crop);                                                      // Crop an image to a defined rectangle
rl_RLAPI void rl_ImageAlphaCrop(rl_Image *image, float threshold);                                                // Crop image depending on alpha value
rl_RLAPI void rl_ImageAlphaClear(rl_Image *image, rl_Color color, float threshold);                                  // Clear alpha channel to desired color
rl_RLAPI void rl_ImageAlphaMask(rl_Image *image, rl_Image alphaMask);                                                // Apply alpha mask to image
rl_RLAPI void rl_ImageAlphaPremultiply(rl_Image *image);                                                          // Premultiply alpha channel
rl_RLAPI void rl_ImageBlurGaussian(rl_Image *image, int blurSize);                                                // Apply Gaussian blur using a box blur approximation
rl_RLAPI void rl_ImageKernelConvolution(rl_Image *image, const float *kernel, int kernelSize);                    // Apply custom square convolution kernel to image
rl_RLAPI void rl_ImageResize(rl_Image *image, int newWidth, int newHeight);                                       // Resize image (Bicubic scaling algorithm)
rl_RLAPI void rl_ImageResizeNN(rl_Image *image, int newWidth, int newHeight);                                     // Resize image (Nearest-Neighbor scaling algorithm)
rl_RLAPI void rl_ImageResizeCanvas(rl_Image *image, int newWidth, int newHeight, int offsetX, int offsetY, rl_Color fill); // Resize canvas and fill with color
rl_RLAPI void rl_ImageMipmaps(rl_Image *image);                                                                   // Compute all mipmap levels for a provided image
rl_RLAPI void rl_ImageDither(rl_Image *image, int rBpp, int gBpp, int bBpp, int aBpp);                            // Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
rl_RLAPI void rl_ImageFlipVertical(rl_Image *image);                                                              // Flip image vertically
rl_RLAPI void rl_ImageFlipHorizontal(rl_Image *image);                                                            // Flip image horizontally
rl_RLAPI void rl_ImageRotate(rl_Image *image, int degrees);                                                       // Rotate image by input angle in degrees (-359 to 359)
rl_RLAPI void rl_ImageRotateCW(rl_Image *image);                                                                  // Rotate image clockwise 90deg
rl_RLAPI void rl_ImageRotateCCW(rl_Image *image);                                                                 // Rotate image counter-clockwise 90deg
rl_RLAPI void rl_ImageColorTint(rl_Image *image, rl_Color color);                                                    // Modify image color: tint
rl_RLAPI void rl_ImageColorInvert(rl_Image *image);                                                               // Modify image color: invert
rl_RLAPI void rl_ImageColorGrayscale(rl_Image *image);                                                            // Modify image color: grayscale
rl_RLAPI void rl_ImageColorContrast(rl_Image *image, float contrast);                                             // Modify image color: contrast (-100 to 100)
rl_RLAPI void rl_ImageColorBrightness(rl_Image *image, int brightness);                                           // Modify image color: brightness (-255 to 255)
rl_RLAPI void rl_ImageColorReplace(rl_Image *image, rl_Color color, rl_Color replace);                                  // Modify image color: replace color
rl_RLAPI rl_Color *rl_LoadImageColors(rl_Image image);                                                               // Load color data from image as a rl_Color array (RGBA - 32bit)
rl_RLAPI rl_Color *rl_LoadImagePalette(rl_Image image, int maxPaletteSize, int *colorCount);                         // Load colors palette from image as a rl_Color array (RGBA - 32bit)
rl_RLAPI void rl_UnloadImageColors(rl_Color *colors);                                                             // Unload color data loaded with rl_LoadImageColors()
rl_RLAPI void rl_UnloadImagePalette(rl_Color *colors);                                                            // Unload colors palette loaded with rl_LoadImagePalette()
rl_RLAPI rl_Rectangle rl_GetImageAlphaBorder(rl_Image image, float threshold);                                       // Get image alpha border rectangle
rl_RLAPI rl_Color rl_GetImageColor(rl_Image image, int x, int y);                                                    // Get image pixel color at (x, y) position

// rl_Image drawing functions
// NOTE: rl_Image software-rendering functions (CPU)
rl_RLAPI void rl_ImageClearBackground(rl_Image *dst, rl_Color color);                                                // Clear image background with given color
rl_RLAPI void rl_ImageDrawPixel(rl_Image *dst, int posX, int posY, rl_Color color);                                  // Draw pixel within an image
rl_RLAPI void rl_ImageDrawPixelV(rl_Image *dst, rl_Vector2 position, rl_Color color);                                   // Draw pixel within an image (Vector version)
rl_RLAPI void rl_ImageDrawLine(rl_Image *dst, int startPosX, int startPosY, int endPosX, int endPosY, rl_Color color); // Draw line within an image
rl_RLAPI void rl_ImageDrawLineV(rl_Image *dst, rl_Vector2 start, rl_Vector2 end, rl_Color color);                          // Draw line within an image (Vector version)
rl_RLAPI void rl_ImageDrawLineEx(rl_Image *dst, rl_Vector2 start, rl_Vector2 end, int thick, rl_Color color);              // Draw a line defining thickness within an image
rl_RLAPI void rl_ImageDrawCircle(rl_Image *dst, int centerX, int centerY, int radius, rl_Color color);               // Draw a filled circle within an image
rl_RLAPI void rl_ImageDrawCircleV(rl_Image *dst, rl_Vector2 center, int radius, rl_Color color);                        // Draw a filled circle within an image (Vector version)
rl_RLAPI void rl_ImageDrawCircleLines(rl_Image *dst, int centerX, int centerY, int radius, rl_Color color);          // Draw circle outline within an image
rl_RLAPI void rl_ImageDrawCircleLinesV(rl_Image *dst, rl_Vector2 center, int radius, rl_Color color);                   // Draw circle outline within an image (Vector version)
rl_RLAPI void rl_ImageDrawRectangle(rl_Image *dst, int posX, int posY, int width, int height, rl_Color color);       // Draw rectangle within an image
rl_RLAPI void rl_ImageDrawRectangleV(rl_Image *dst, rl_Vector2 position, rl_Vector2 size, rl_Color color);                 // Draw rectangle within an image (Vector version)
rl_RLAPI void rl_ImageDrawRectangleRec(rl_Image *dst, rl_Rectangle rec, rl_Color color);                                // Draw rectangle within an image
rl_RLAPI void rl_ImageDrawRectangleLines(rl_Image *dst, rl_Rectangle rec, int thick, rl_Color color);                   // Draw rectangle lines within an image
rl_RLAPI void rl_ImageDrawTriangle(rl_Image *dst, rl_Vector2 v1, rl_Vector2 v2, rl_Vector2 v3, rl_Color color);               // Draw triangle within an image
rl_RLAPI void rl_ImageDrawTriangleEx(rl_Image *dst, rl_Vector2 v1, rl_Vector2 v2, rl_Vector2 v3, rl_Color c1, rl_Color c2, rl_Color c3); // Draw triangle with interpolated colors within an image
rl_RLAPI void rl_ImageDrawTriangleLines(rl_Image *dst, rl_Vector2 v1, rl_Vector2 v2, rl_Vector2 v3, rl_Color color);          // Draw triangle outline within an image
rl_RLAPI void rl_ImageDrawTriangleFan(rl_Image *dst, const rl_Vector2 *points, int pointCount, rl_Color color);               // Draw a triangle fan defined by points within an image (first vertex is the center)
rl_RLAPI void rl_ImageDrawTriangleStrip(rl_Image *dst, const rl_Vector2 *points, int pointCount, rl_Color color);             // Draw a triangle strip defined by points within an image
rl_RLAPI void rl_ImageDraw(rl_Image *dst, rl_Image src, rl_Rectangle srcRec, rl_Rectangle dstRec, rl_Color tint);             // Draw a source image within a destination image (tint applied to source)
rl_RLAPI void rl_ImageDrawText(rl_Image *dst, const char *text, int posX, int posY, int fontSize, rl_Color color);   // Draw text (using default font) within an image (destination)
rl_RLAPI void rl_ImageDrawTextEx(rl_Image *dst, rl_Font font, const char *text, rl_Vector2 position, float fontSize, float spacing, rl_Color tint); // Draw text (custom sprite font) within an image (destination)

// rl_Texture loading functions
// NOTE: These functions require GPU access
rl_RLAPI rl_Texture2D rl_LoadTexture(const char *fileName);                                                       // Load texture from file into GPU memory (VRAM)
rl_RLAPI rl_Texture2D rl_LoadTextureFromImage(rl_Image image);                                                       // Load texture from image data
rl_RLAPI rl_TextureCubemap rl_LoadTextureCubemap(rl_Image image, int layout);                                        // Load cubemap from image, multiple image cubemap layouts supported
rl_RLAPI rl_RenderTexture2D rl_LoadRenderTexture(int width, int height);                                          // Load texture for rendering (framebuffer)
rl_RLAPI bool rl_IsTextureValid(rl_Texture2D texture);                                                            // Check if a texture is valid (loaded in GPU)
rl_RLAPI void rl_UnloadTexture(rl_Texture2D texture);                                                             // Unload texture from GPU memory (VRAM)
rl_RLAPI bool rl_IsRenderTextureValid(rl_RenderTexture2D target);                                                 // Check if a render texture is valid (loaded in GPU)
rl_RLAPI void rl_UnloadRenderTexture(rl_RenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)
rl_RLAPI void rl_UpdateTexture(rl_Texture2D texture, const void *pixels);                                         // Update GPU texture with new data (pixels should be able to fill texture)
rl_RLAPI void rl_UpdateTextureRec(rl_Texture2D texture, rl_Rectangle rec, const void *pixels);                       // Update GPU texture rectangle with new data (pixels and rec should fit in texture)

// rl_Texture configuration functions
rl_RLAPI void rl_GenTextureMipmaps(rl_Texture2D *texture);                                                        // Generate GPU mipmaps for a texture
rl_RLAPI void rl_SetTextureFilter(rl_Texture2D texture, int filter);                                              // Set texture scaling filter mode
rl_RLAPI void rl_SetTextureWrap(rl_Texture2D texture, int wrap);                                                  // Set texture wrapping mode

// rl_Texture drawing functions
rl_RLAPI void rl_DrawTexture(rl_Texture2D texture, int posX, int posY, rl_Color tint);                               // Draw a rl_Texture2D
rl_RLAPI void rl_DrawTextureV(rl_Texture2D texture, rl_Vector2 position, rl_Color tint);                                // Draw a rl_Texture2D with position defined as rl_Vector2
rl_RLAPI void rl_DrawTextureEx(rl_Texture2D texture, rl_Vector2 position, float rotation, float scale, rl_Color tint);  // Draw a rl_Texture2D with extended parameters
rl_RLAPI void rl_DrawTextureRec(rl_Texture2D texture, rl_Rectangle source, rl_Vector2 position, rl_Color tint);            // Draw a part of a texture defined by a rectangle
rl_RLAPI void rl_DrawTexturePro(rl_Texture2D texture, rl_Rectangle source, rl_Rectangle dest, rl_Vector2 origin, float rotation, rl_Color tint); // Draw a part of a texture defined by a rectangle with 'pro' parameters
rl_RLAPI void rl_DrawTextureNPatch(rl_Texture2D texture, rl_NPatchInfo nPatchInfo, rl_Rectangle dest, rl_Vector2 origin, float rotation, rl_Color tint); // Draws a texture (or part of it) that stretches or shrinks nicely

// rl_Color/pixel related functions
rl_RLAPI bool rl_ColorIsEqual(rl_Color col1, rl_Color col2);                            // Check if two colors are equal
rl_RLAPI rl_Color rl_Fade(rl_Color color, float alpha);                                 // Get color with alpha applied, alpha goes from 0.0f to 1.0f
rl_RLAPI int rl_ColorToInt(rl_Color color);                                          // Get hexadecimal value for a rl_Color (0xRRGGBBAA)
rl_RLAPI rl_Vector4 rl_ColorNormalize(rl_Color color);                                  // Get rl_Color normalized as float [0..1]
rl_RLAPI rl_Color rl_ColorFromNormalized(rl_Vector4 normalized);                        // Get rl_Color from normalized values [0..1]
rl_RLAPI rl_Vector3 rl_ColorToHSV(rl_Color color);                                      // Get HSV values for a rl_Color, hue [0..360], saturation/value [0..1]
rl_RLAPI rl_Color rl_ColorFromHSV(float hue, float saturation, float value);         // Get a rl_Color from HSV values, hue [0..360], saturation/value [0..1]
rl_RLAPI rl_Color rl_ColorTint(rl_Color color, rl_Color tint);                             // Get color multiplied with another color
rl_RLAPI rl_Color rl_ColorBrightness(rl_Color color, float factor);                     // Get color with brightness correction, brightness factor goes from -1.0f to 1.0f
rl_RLAPI rl_Color rl_ColorContrast(rl_Color color, float contrast);                     // Get color with contrast correction, contrast values between -1.0f and 1.0f
rl_RLAPI rl_Color rl_ColorAlpha(rl_Color color, float alpha);                           // Get color with alpha applied, alpha goes from 0.0f to 1.0f
rl_RLAPI rl_Color rl_ColorAlphaBlend(rl_Color dst, rl_Color src, rl_Color tint);              // Get src alpha-blended into dst color with tint
rl_RLAPI rl_Color rl_ColorLerp(rl_Color color1, rl_Color color2, float factor);            // Get color lerp interpolation between two colors, factor [0.0f..1.0f]
rl_RLAPI rl_Color rl_GetColor(unsigned int hexValue);                                // Get rl_Color structure from hexadecimal value
rl_RLAPI rl_Color rl_GetPixelColor(void *srcPtr, int format);                        // Get rl_Color from a source pixel pointer of certain format
rl_RLAPI void rl_SetPixelColor(void *dstPtr, rl_Color color, int format);            // Set color formatted into destination pixel pointer
rl_RLAPI int rl_GetPixelDataSize(int width, int height, int format);              // Get pixel data size in bytes for certain format

//------------------------------------------------------------------------------------
// rl_Font Loading and Text Drawing Functions (Module: text)
//------------------------------------------------------------------------------------

// rl_Font loading/unloading functions
rl_RLAPI rl_Font rl_GetFontDefault(void);                                                            // Get the default rl_Font
rl_RLAPI rl_Font rl_LoadFont(const char *fileName);                                                  // Load font from file into GPU memory (VRAM)
rl_RLAPI rl_Font rl_LoadFontEx(const char *fileName, int fontSize, const int *codepoints, int codepointCount); // Load font from file with extended parameters, use NULL for codepoints and 0 for codepointCount to load the default character set, font size is provided in pixels height
rl_RLAPI rl_Font rl_LoadFontFromImage(rl_Image image, rl_Color key, int firstChar);                        // Load font from rl_Image (XNA style)
rl_RLAPI rl_Font rl_LoadFontFromMemory(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, const int *codepoints, int codepointCount); // Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
rl_RLAPI bool rl_IsFontValid(rl_Font font);                                                          // Check if a font is valid (font data loaded, WARNING: GPU texture not checked)
rl_RLAPI rl_GlyphInfo *rl_LoadFontData(const unsigned char *fileData, int dataSize, int fontSize, const int *codepoints, int codepointCount, int type, int *glyphCount); // Load font data for further use
rl_RLAPI rl_Image rl_GenImageFontAtlas(const rl_GlyphInfo *glyphs, rl_Rectangle **glyphRecs, int glyphCount, int fontSize, int padding, int packMethod); // Generate image font atlas using chars info
rl_RLAPI void rl_UnloadFontData(rl_GlyphInfo *glyphs, int glyphCount);                               // Unload font chars info data (RAM)
rl_RLAPI void rl_UnloadFont(rl_Font font);                                                           // Unload font from GPU memory (VRAM)
rl_RLAPI bool rl_ExportFontAsCode(rl_Font font, const char *fileName);                               // Export font as code file, returns true on success

// Text drawing functions
rl_RLAPI void rl_DrawFPS(int posX, int posY);                                                     // Draw current FPS
rl_RLAPI void rl_DrawText(const char *text, int posX, int posY, int fontSize, rl_Color color);       // Draw text (using default font)
rl_RLAPI void rl_DrawTextEx(rl_Font font, const char *text, rl_Vector2 position, float fontSize, float spacing, rl_Color tint); // Draw text using font and additional parameters
rl_RLAPI void rl_DrawTextPro(rl_Font font, const char *text, rl_Vector2 position, rl_Vector2 origin, float rotation, float fontSize, float spacing, rl_Color tint); // Draw text using rl_Font and pro parameters (rotation)
rl_RLAPI void rl_DrawTextCodepoint(rl_Font font, int codepoint, rl_Vector2 position, float fontSize, rl_Color tint); // Draw one character (codepoint)
rl_RLAPI void rl_DrawTextCodepoints(rl_Font font, const int *codepoints, int codepointCount, rl_Vector2 position, float fontSize, float spacing, rl_Color tint); // Draw multiple character (codepoint)

// Text font info functions
rl_RLAPI void rl_SetTextLineSpacing(int spacing);                                                 // Set vertical line spacing when drawing with line-breaks
rl_RLAPI int rl_MeasureText(const char *text, int fontSize);                                      // Measure string width for default font
rl_RLAPI rl_Vector2 rl_MeasureTextEx(rl_Font font, const char *text, float fontSize, float spacing);    // Measure string size for rl_Font
rl_RLAPI int rl_GetGlyphIndex(rl_Font font, int codepoint);                                          // Get glyph index position in font for a codepoint (unicode character), fallback to '?' if not found
rl_RLAPI rl_GlyphInfo rl_GetGlyphInfo(rl_Font font, int codepoint);                                     // Get glyph font info data for a codepoint (unicode character), fallback to '?' if not found
rl_RLAPI rl_Rectangle rl_GetGlyphAtlasRec(rl_Font font, int codepoint);                                 // Get glyph rectangle in font atlas for a codepoint (unicode character), fallback to '?' if not found

// Text codepoints management functions (unicode characters)
rl_RLAPI char *rl_LoadUTF8(const int *codepoints, int length);                                    // Load UTF-8 text encoded from codepoints array
rl_RLAPI void rl_UnloadUTF8(char *text);                                                          // Unload UTF-8 text encoded from codepoints array
rl_RLAPI int *rl_LoadCodepoints(const char *text, int *count);                                    // Load all codepoints from a UTF-8 text string, codepoints count returned by parameter
rl_RLAPI void rl_UnloadCodepoints(int *codepoints);                                               // Unload codepoints data from memory
rl_RLAPI int rl_GetCodepointCount(const char *text);                                              // Get total number of codepoints in a UTF-8 encoded string
rl_RLAPI int rl_GetCodepoint(const char *text, int *codepointSize);                               // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
rl_RLAPI int rl_GetCodepointNext(const char *text, int *codepointSize);                           // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
rl_RLAPI int rl_GetCodepointPrevious(const char *text, int *codepointSize);                       // Get previous codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
rl_RLAPI const char *rl_CodepointToUTF8(int codepoint, int *utf8Size);                            // Encode one codepoint into UTF-8 byte array (array length returned as parameter)

// Text strings management functions (no UTF-8 strings, only byte chars)
// WARNING 1: Most of these functions use internal static buffers[], it's recommended to store returned data on user-side for re-use
// WARNING 2: Some strings allocate memory internally for the returned strings, those strings must be free by user using rl_MemFree()
rl_RLAPI char **rl_LoadTextLines(const char *text, int *count);                                   // Load text as separate lines ('\n')
rl_RLAPI void rl_UnloadTextLines(char **text, int lineCount);                                     // Unload text lines
rl_RLAPI int rl_TextCopy(char *dst, const char *src);                                             // Copy one string to another, returns bytes copied
rl_RLAPI bool rl_TextIsEqual(const char *text1, const char *text2);                               // Check if two text string are equal
rl_RLAPI unsigned int rl_TextLength(const char *text);                                            // Get text length, checks for '\0' ending
rl_RLAPI const char *rl_TextFormat(const char *text, ...);                                        // Text formatting with variables (sprintf() style)
rl_RLAPI const char *rl_TextSubtext(const char *text, int position, int length);                  // Get a piece of a text string
rl_RLAPI const char *rl_TextRemoveSpaces(const char *text);                                       // Remove text spaces, concat words
rl_RLAPI char *rl_GetTextBetween(const char *text, const char *begin, const char *end);           // Get text between two strings
rl_RLAPI char *rl_TextReplace(const char *text, const char *search, const char *replacement);     // Replace text string (WARNING: memory must be freed!)
rl_RLAPI char *rl_TextReplaceBetween(const char *text, const char *begin, const char *end, const char *replacement); // Replace text between two specific strings (WARNING: memory must be freed!)
rl_RLAPI char *rl_TextInsert(const char *text, const char *insert, int position);                 // Insert text in a position (WARNING: memory must be freed!)
rl_RLAPI char *rl_TextJoin(char **textList, int count, const char *delimiter);                    // Join text strings with delimiter
rl_RLAPI char **rl_TextSplit(const char *text, char delimiter, int *count);                       // Split text into multiple strings, using MAX_TEXTSPLIT_COUNT static strings
rl_RLAPI void rl_TextAppend(char *text, const char *append, int *position);                       // Append text at specific position and move cursor
rl_RLAPI int rl_TextFindIndex(const char *text, const char *search);                              // Find first text occurrence within a string, -1 if not found
rl_RLAPI char *rl_TextToUpper(const char *text);                                                  // Get upper case version of provided string
rl_RLAPI char *rl_TextToLower(const char *text);                                                  // Get lower case version of provided string
rl_RLAPI char *rl_TextToPascal(const char *text);                                                 // Get Pascal case notation version of provided string
rl_RLAPI char *rl_TextToSnake(const char *text);                                                  // Get Snake case notation version of provided string
rl_RLAPI char *rl_TextToCamel(const char *text);                                                  // Get Camel case notation version of provided string
rl_RLAPI int rl_TextToInteger(const char *text);                                                  // Get integer value from text
rl_RLAPI float rl_TextToFloat(const char *text);                                                  // Get float value from text

//------------------------------------------------------------------------------------
// Basic 3d Shapes Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// Basic geometric 3D shapes drawing functions
rl_RLAPI void rl_DrawLine3D(rl_Vector3 startPos, rl_Vector3 endPos, rl_Color color);                                    // Draw a line in 3D world space
rl_RLAPI void rl_DrawPoint3D(rl_Vector3 position, rl_Color color);                                                   // Draw a point in 3D space, actually a small line
rl_RLAPI void rl_DrawCircle3D(rl_Vector3 center, float radius, rl_Vector3 rotationAxis, float rotationAngle, rl_Color color); // Draw a circle in 3D world space
rl_RLAPI void rl_DrawTriangle3D(rl_Vector3 v1, rl_Vector3 v2, rl_Vector3 v3, rl_Color color);                              // Draw a color-filled triangle (vertex in counter-clockwise order!)
rl_RLAPI void rl_DrawTriangleStrip3D(const rl_Vector3 *points, int pointCount, rl_Color color);                      // Draw a triangle strip defined by points
rl_RLAPI void rl_DrawCube(rl_Vector3 position, float width, float height, float length, rl_Color color);             // Draw cube
rl_RLAPI void rl_DrawCubeV(rl_Vector3 position, rl_Vector3 size, rl_Color color);                                       // Draw cube (Vector version)
rl_RLAPI void rl_DrawCubeWires(rl_Vector3 position, float width, float height, float length, rl_Color color);        // Draw cube wires
rl_RLAPI void rl_DrawCubeWiresV(rl_Vector3 position, rl_Vector3 size, rl_Color color);                                  // Draw cube wires (Vector version)
rl_RLAPI void rl_DrawSphere(rl_Vector3 centerPos, float radius, rl_Color color);                                     // Draw sphere
rl_RLAPI void rl_DrawSphereEx(rl_Vector3 centerPos, float radius, int rings, int slices, rl_Color color);            // Draw sphere with extended parameters
rl_RLAPI void rl_DrawSphereWires(rl_Vector3 centerPos, float radius, int rings, int slices, rl_Color color);         // Draw sphere wires
rl_RLAPI void rl_DrawCylinder(rl_Vector3 position, float radiusTop, float radiusBottom, float height, int slices, rl_Color color); // Draw a cylinder/cone
rl_RLAPI void rl_DrawCylinderEx(rl_Vector3 startPos, rl_Vector3 endPos, float startRadius, float endRadius, int sides, rl_Color color); // Draw a cylinder with base at startPos and top at endPos
rl_RLAPI void rl_DrawCylinderWires(rl_Vector3 position, float radiusTop, float radiusBottom, float height, int slices, rl_Color color); // Draw a cylinder/cone wires
rl_RLAPI void rl_DrawCylinderWiresEx(rl_Vector3 startPos, rl_Vector3 endPos, float startRadius, float endRadius, int sides, rl_Color color); // Draw a cylinder wires with base at startPos and top at endPos
rl_RLAPI void rl_DrawCapsule(rl_Vector3 startPos, rl_Vector3 endPos, float radius, int slices, int rings, rl_Color color); // Draw a capsule with the center of its sphere caps at startPos and endPos
rl_RLAPI void rl_DrawCapsuleWires(rl_Vector3 startPos, rl_Vector3 endPos, float radius, int slices, int rings, rl_Color color); // Draw capsule wireframe with the center of its sphere caps at startPos and endPos
rl_RLAPI void rl_DrawPlane(rl_Vector3 centerPos, rl_Vector2 size, rl_Color color);                                      // Draw a plane XZ
rl_RLAPI void rl_DrawRay(rl_Ray ray, rl_Color color);                                                                // Draw a ray line
rl_RLAPI void rl_DrawGrid(int slices, float spacing);                                                          // Draw a grid (centered at (0, 0, 0))

//------------------------------------------------------------------------------------
// rl_Model 3d Loading and Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// rl_Model management functions
rl_RLAPI rl_Model rl_LoadModel(const char *fileName);                                                // Load model from files (meshes and materials)
rl_RLAPI rl_Model rl_LoadModelFromMesh(rl_Mesh mesh);                                                   // Load model from generated mesh (default material)
rl_RLAPI bool rl_IsModelValid(rl_Model model);                                                       // Check if a model is valid (loaded in GPU, VAO/VBOs)
rl_RLAPI void rl_UnloadModel(rl_Model model);                                                        // Unload model (including meshes) from memory (RAM and/or VRAM)
rl_RLAPI rl_BoundingBox rl_GetModelBoundingBox(rl_Model model);                                         // Compute model bounding box limits (considers all meshes)

// rl_Model drawing functions
rl_RLAPI void rl_DrawModel(rl_Model model, rl_Vector3 position, float scale, rl_Color tint);               // Draw a model (with texture if set)
rl_RLAPI void rl_DrawModelEx(rl_Model model, rl_Vector3 position, rl_Vector3 rotationAxis, float rotationAngle, rl_Vector3 scale, rl_Color tint); // Draw a model with extended parameters
rl_RLAPI void rl_DrawModelWires(rl_Model model, rl_Vector3 position, float scale, rl_Color tint);          // Draw a model wires (with texture if set)
rl_RLAPI void rl_DrawModelWiresEx(rl_Model model, rl_Vector3 position, rl_Vector3 rotationAxis, float rotationAngle, rl_Vector3 scale, rl_Color tint); // Draw a model wires (with texture if set) with extended parameters
rl_RLAPI void rl_DrawModelPoints(rl_Model model, rl_Vector3 position, float scale, rl_Color tint); // Draw a model as points
rl_RLAPI void rl_DrawModelPointsEx(rl_Model model, rl_Vector3 position, rl_Vector3 rotationAxis, float rotationAngle, rl_Vector3 scale, rl_Color tint); // Draw a model as points with extended parameters
rl_RLAPI void rl_DrawBoundingBox(rl_BoundingBox box, rl_Color color);                                   // Draw bounding box (wires)
rl_RLAPI void rl_DrawBillboard(rl_Camera camera, rl_Texture2D texture, rl_Vector3 position, float scale, rl_Color tint);   // Draw a billboard texture
rl_RLAPI void rl_DrawBillboardRec(rl_Camera camera, rl_Texture2D texture, rl_Rectangle source, rl_Vector3 position, rl_Vector2 size, rl_Color tint); // Draw a billboard texture defined by source
rl_RLAPI void rl_DrawBillboardPro(rl_Camera camera, rl_Texture2D texture, rl_Rectangle source, rl_Vector3 position, rl_Vector3 up, rl_Vector2 size, rl_Vector2 origin, float rotation, rl_Color tint); // Draw a billboard texture defined by source and rotation

// rl_Mesh management functions
rl_RLAPI void rl_UploadMesh(rl_Mesh *mesh, bool dynamic);                                            // Upload mesh vertex data in GPU and provide VAO/VBO ids
rl_RLAPI void rl_UpdateMeshBuffer(rl_Mesh mesh, int index, const void *data, int dataSize, int offset); // Update mesh vertex data in GPU for a specific buffer index
rl_RLAPI void rl_UnloadMesh(rl_Mesh mesh);                                                           // Unload mesh data from CPU and GPU
rl_RLAPI void rl_DrawMesh(rl_Mesh mesh, rl_Material material, rl_Matrix transform);                        // Draw a 3d mesh with material and transform
rl_RLAPI void rl_DrawMeshInstanced(rl_Mesh mesh, rl_Material material, const rl_Matrix *transforms, int instances); // Draw multiple mesh instances with material and different transforms
rl_RLAPI rl_BoundingBox rl_GetMeshBoundingBox(rl_Mesh mesh);                                            // Compute mesh bounding box limits
rl_RLAPI void rl_GenMeshTangents(rl_Mesh *mesh);                                                     // Compute mesh tangents
rl_RLAPI bool rl_ExportMesh(rl_Mesh mesh, const char *fileName);                                     // Export mesh data to file, returns true on success
rl_RLAPI bool rl_ExportMeshAsCode(rl_Mesh mesh, const char *fileName);                               // Export mesh as code file (.h) defining multiple arrays of vertex attributes

// rl_Mesh generation functions
rl_RLAPI rl_Mesh rl_GenMeshPoly(int sides, float radius);                                            // Generate polygonal mesh
rl_RLAPI rl_Mesh rl_GenMeshPlane(float width, float length, int resX, int resZ);                     // Generate plane mesh (with subdivisions)
rl_RLAPI rl_Mesh rl_GenMeshCube(float width, float height, float length);                            // Generate cuboid mesh
rl_RLAPI rl_Mesh rl_GenMeshSphere(float radius, int rings, int slices);                              // Generate sphere mesh (standard sphere)
rl_RLAPI rl_Mesh rl_GenMeshHemiSphere(float radius, int rings, int slices);                          // Generate half-sphere mesh (no bottom cap)
rl_RLAPI rl_Mesh rl_GenMeshCylinder(float radius, float height, int slices);                         // Generate cylinder mesh
rl_RLAPI rl_Mesh rl_GenMeshCone(float radius, float height, int slices);                             // Generate cone/pyramid mesh
rl_RLAPI rl_Mesh rl_GenMeshTorus(float radius, float size, int radSeg, int sides);                   // Generate torus mesh
rl_RLAPI rl_Mesh rl_GenMeshKnot(float radius, float size, int radSeg, int sides);                    // Generate trefoil knot mesh
rl_RLAPI rl_Mesh rl_GenMeshHeightmap(rl_Image heightmap, rl_Vector3 size);                                 // Generate heightmap mesh from image data
rl_RLAPI rl_Mesh rl_GenMeshCubicmap(rl_Image cubicmap, rl_Vector3 cubeSize);                               // Generate cubes-based map mesh from image data

// rl_Material loading/unloading functions
rl_RLAPI rl_Material *rl_LoadMaterials(const char *fileName, int *materialCount);                    // Load materials from model file
rl_RLAPI rl_Material rl_LoadMaterialDefault(void);                                                   // Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
rl_RLAPI bool rl_IsMaterialValid(rl_Material material);                                              // Check if a material is valid (shader assigned, map textures loaded in GPU)
rl_RLAPI void rl_UnloadMaterial(rl_Material material);                                               // Unload material from GPU memory (VRAM)
rl_RLAPI void rl_SetMaterialTexture(rl_Material *material, int mapType, rl_Texture2D texture);          // Set texture for a material map type (rl_MATERIAL_MAP_DIFFUSE, rl_MATERIAL_MAP_SPECULAR...)
rl_RLAPI void rl_SetModelMeshMaterial(rl_Model *model, int meshId, int materialId);                  // Set material for a mesh

// rl_Model animations loading/unloading functions
rl_RLAPI rl_ModelAnimation *rl_LoadModelAnimations(const char *fileName, int *animCount);            // Load model animations from file
rl_RLAPI void rl_UpdateModelAnimation(rl_Model model, rl_ModelAnimation anim, int frame);               // Update model animation pose (CPU)
rl_RLAPI void rl_UpdateModelAnimationBones(rl_Model model, rl_ModelAnimation anim, int frame);          // Update model animation mesh bone matrices (GPU skinning)
rl_RLAPI void rl_UnloadModelAnimation(rl_ModelAnimation anim);                                       // Unload animation data
rl_RLAPI void rl_UnloadModelAnimations(rl_ModelAnimation *animations, int animCount);                // Unload animation array data
rl_RLAPI bool rl_IsModelAnimationValid(rl_Model model, rl_ModelAnimation anim);                         // Check model animation skeleton match

// Collision detection functions
rl_RLAPI bool rl_CheckCollisionSpheres(rl_Vector3 center1, float radius1, rl_Vector3 center2, float radius2); // Check collision between two spheres
rl_RLAPI bool rl_CheckCollisionBoxes(rl_BoundingBox box1, rl_BoundingBox box2);                         // Check collision between two bounding boxes
rl_RLAPI bool rl_CheckCollisionBoxSphere(rl_BoundingBox box, rl_Vector3 center, float radius);          // Check collision between box and sphere
rl_RLAPI rl_RayCollision rl_GetRayCollisionSphere(rl_Ray ray, rl_Vector3 center, float radius);            // Get collision info between ray and sphere
rl_RLAPI rl_RayCollision rl_GetRayCollisionBox(rl_Ray ray, rl_BoundingBox box);                            // Get collision info between ray and box
rl_RLAPI rl_RayCollision rl_GetRayCollisionMesh(rl_Ray ray, rl_Mesh mesh, rl_Matrix transform);               // Get collision info between ray and mesh
rl_RLAPI rl_RayCollision rl_GetRayCollisionTriangle(rl_Ray ray, rl_Vector3 p1, rl_Vector3 p2, rl_Vector3 p3);    // Get collision info between ray and triangle
rl_RLAPI rl_RayCollision rl_GetRayCollisionQuad(rl_Ray ray, rl_Vector3 p1, rl_Vector3 p2, rl_Vector3 p3, rl_Vector3 p4); // Get collision info between ray and quad

//------------------------------------------------------------------------------------
// Audio Loading and Playing Functions (Module: audio)
//------------------------------------------------------------------------------------
typedef void (*AudioCallback)(void *bufferData, unsigned int frames);

// Audio device management functions
rl_RLAPI void rl_InitAudioDevice(void);                                     // Initialize audio device and context
rl_RLAPI void rl_CloseAudioDevice(void);                                    // Close the audio device and context
rl_RLAPI bool rl_IsAudioDeviceReady(void);                                  // Check if audio device has been initialized successfully
rl_RLAPI void rl_SetMasterVolume(float volume);                             // Set master volume (listener)
rl_RLAPI float rl_GetMasterVolume(void);                                    // Get master volume (listener)

// rl_Wave/rl_Sound loading/unloading functions
rl_RLAPI rl_Wave rl_LoadWave(const char *fileName);                            // Load wave data from file
rl_RLAPI rl_Wave rl_LoadWaveFromMemory(const char *fileType, const unsigned char *fileData, int dataSize); // Load wave from memory buffer, fileType refers to extension: i.e. '.wav'
rl_RLAPI bool rl_IsWaveValid(rl_Wave wave);                                    // Checks if wave data is valid (data loaded and parameters)
rl_RLAPI rl_Sound rl_LoadSound(const char *fileName);                          // Load sound from file
rl_RLAPI rl_Sound rl_LoadSoundFromWave(rl_Wave wave);                             // Load sound from wave data
rl_RLAPI rl_Sound rl_LoadSoundAlias(rl_Sound source);                             // Create a new sound that shares the same sample data as the source sound, does not own the sound data
rl_RLAPI bool rl_IsSoundValid(rl_Sound sound);                                 // Checks if a sound is valid (data loaded and buffers initialized)
rl_RLAPI void rl_UpdateSound(rl_Sound sound, const void *data, int sampleCount); // Update sound buffer with new data (default data format: 32 bit float, stereo)
rl_RLAPI void rl_UnloadWave(rl_Wave wave);                                     // Unload wave data
rl_RLAPI void rl_UnloadSound(rl_Sound sound);                                  // Unload sound
rl_RLAPI void rl_UnloadSoundAlias(rl_Sound alias);                             // Unload a sound alias (does not deallocate sample data)
rl_RLAPI bool rl_ExportWave(rl_Wave wave, const char *fileName);               // Export wave data to file, returns true on success
rl_RLAPI bool rl_ExportWaveAsCode(rl_Wave wave, const char *fileName);         // Export wave sample data to code (.h), returns true on success

// rl_Wave/rl_Sound management functions
rl_RLAPI void rl_PlaySound(rl_Sound sound);                                    // Play a sound
rl_RLAPI void rl_StopSound(rl_Sound sound);                                    // Stop playing a sound
rl_RLAPI void rl_PauseSound(rl_Sound sound);                                   // Pause a sound
rl_RLAPI void rl_ResumeSound(rl_Sound sound);                                  // Resume a paused sound
rl_RLAPI bool rl_IsSoundPlaying(rl_Sound sound);                               // Check if a sound is currently playing
rl_RLAPI void rl_SetSoundVolume(rl_Sound sound, float volume);                 // Set volume for a sound (1.0 is max level)
rl_RLAPI void rl_SetSoundPitch(rl_Sound sound, float pitch);                   // Set pitch for a sound (1.0 is base level)
rl_RLAPI void rl_SetSoundPan(rl_Sound sound, float pan);                       // Set pan for a sound (-1.0 left, 0.0 center, 1.0 right)
rl_RLAPI rl_Wave rl_WaveCopy(rl_Wave wave);                                       // Copy a wave to a new wave
rl_RLAPI void rl_WaveCrop(rl_Wave *wave, int initFrame, int finalFrame);       // Crop a wave to defined frames range
rl_RLAPI void rl_WaveFormat(rl_Wave *wave, int sampleRate, int sampleSize, int channels); // Convert wave data to desired format
rl_RLAPI float *rl_LoadWaveSamples(rl_Wave wave);                              // Load samples data from wave as a 32bit float data array
rl_RLAPI void rl_UnloadWaveSamples(float *samples);                         // Unload samples data loaded with rl_LoadWaveSamples()

// rl_Music management functions
rl_RLAPI rl_Music rl_LoadMusicStream(const char *fileName);                    // Load music stream from file
rl_RLAPI rl_Music rl_LoadMusicStreamFromMemory(const char *fileType, const unsigned char *data, int dataSize); // Load music stream from data
rl_RLAPI bool rl_IsMusicValid(rl_Music music);                                 // Checks if a music stream is valid (context and buffers initialized)
rl_RLAPI void rl_UnloadMusicStream(rl_Music music);                            // Unload music stream
rl_RLAPI void rl_PlayMusicStream(rl_Music music);                              // Start music playing
rl_RLAPI bool rl_IsMusicStreamPlaying(rl_Music music);                         // Check if music is playing
rl_RLAPI void rl_UpdateMusicStream(rl_Music music);                            // Updates buffers for music streaming
rl_RLAPI void rl_StopMusicStream(rl_Music music);                              // Stop music playing
rl_RLAPI void rl_PauseMusicStream(rl_Music music);                             // Pause music playing
rl_RLAPI void rl_ResumeMusicStream(rl_Music music);                            // Resume playing paused music
rl_RLAPI void rl_SeekMusicStream(rl_Music music, float position);              // Seek music to a position (in seconds)
rl_RLAPI void rl_SetMusicVolume(rl_Music music, float volume);                 // Set volume for music (1.0 is max level)
rl_RLAPI void rl_SetMusicPitch(rl_Music music, float pitch);                   // Set pitch for a music (1.0 is base level)
rl_RLAPI void rl_SetMusicPan(rl_Music music, float pan);                       // Set pan for a music (-1.0 left, 0.0 center, 1.0 right)
rl_RLAPI float rl_GetMusicTimeLength(rl_Music music);                          // Get music time length (in seconds)
rl_RLAPI float rl_GetMusicTimePlayed(rl_Music music);                          // Get current music time played (in seconds)

// rl_AudioStream management functions
rl_RLAPI rl_AudioStream rl_LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels); // Load audio stream (to stream raw audio pcm data)
rl_RLAPI bool rl_IsAudioStreamValid(rl_AudioStream stream);                    // Checks if an audio stream is valid (buffers initialized)
rl_RLAPI void rl_UnloadAudioStream(rl_AudioStream stream);                     // Unload audio stream and free memory
rl_RLAPI void rl_UpdateAudioStream(rl_AudioStream stream, const void *data, int frameCount); // Update audio stream buffers with data
rl_RLAPI bool rl_IsAudioStreamProcessed(rl_AudioStream stream);                // Check if any audio stream buffers requires refill
rl_RLAPI void rl_PlayAudioStream(rl_AudioStream stream);                       // Play audio stream
rl_RLAPI void rl_PauseAudioStream(rl_AudioStream stream);                      // Pause audio stream
rl_RLAPI void rl_ResumeAudioStream(rl_AudioStream stream);                     // Resume audio stream
rl_RLAPI bool rl_IsAudioStreamPlaying(rl_AudioStream stream);                  // Check if audio stream is playing
rl_RLAPI void rl_StopAudioStream(rl_AudioStream stream);                       // Stop audio stream
rl_RLAPI void rl_SetAudioStreamVolume(rl_AudioStream stream, float volume);    // Set volume for audio stream (1.0 is max level)
rl_RLAPI void rl_SetAudioStreamPitch(rl_AudioStream stream, float pitch);      // Set pitch for audio stream (1.0 is base level)
rl_RLAPI void rl_SetAudioStreamPan(rl_AudioStream stream, float pan);          // Set pan for audio stream (0.5 is centered)
rl_RLAPI void rl_SetAudioStreamBufferSizeDefault(int size);                 // Default size for new audio streams
rl_RLAPI void rl_SetAudioStreamCallback(rl_AudioStream stream, AudioCallback callback); // Audio thread callback to request new data

rl_RLAPI void rl_AttachAudioStreamProcessor(rl_AudioStream stream, AudioCallback processor); // Attach audio stream processor to stream, receives frames x 2 samples as 'float' (stereo)
rl_RLAPI void rl_DetachAudioStreamProcessor(rl_AudioStream stream, AudioCallback processor); // Detach audio stream processor from stream

rl_RLAPI void rl_AttachAudioMixedProcessor(AudioCallback processor); // Attach audio stream processor to the entire audio pipeline, receives frames x 2 samples as 'float' (stereo)
rl_RLAPI void rl_DetachAudioMixedProcessor(AudioCallback processor); // Detach audio stream processor from the entire audio pipeline

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_H
