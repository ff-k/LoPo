#ifndef KDB_HEADER_RENDERER_OPENGL
#define KDB_HEADER_RENDERER_OPENGL

#include "kdb_common.h"
#include "kdb_util.h"

#include "kdb_asset.h"
#include "kdb_scene.h"
#include "kdb_camera.h"
#include "kdb_window.h"


#if _WIN32
    #include "kdb_renderer_opengl_win32.h"
    
    #define PlatformOpenGLFuncDeclare(ReturnType, FuncName, ...)    Win32OpenGLFuncDeclare(ReturnType, FuncName, __VA_ARGS__)
    #define PlatformOpenGLFuncLoad(FuncName)                        Win32OpenGLFuncLoad(FuncName)
#else
    #error "OpenGL is not supported on this platform"
#endif
    
#define GL_INVALID_FRAMEBUFFER_OPERATION   0x0506
#define GL_CONTEXT_LOST                    0x0507

#define GL_MULTISAMPLE_ARB                 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB    0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB         0x809F
#define GL_SAMPLE_COVERAGE_ARB             0x80A0
#define GL_SAMPLE_BUFFERS_ARB              0x80A8
#define GL_SAMPLES_ARB                     0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB       0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB      0x80AB

#define GL_BGR                             0x80E0
#define GL_BGRA                            0x80E1

#define GL_CLAMP_TO_BORDER                 0x812D
#define GL_CLAMP_TO_EDGE                   0x812F

#define GL_TEXTURE_MIN_LOD                 0x813A
#define GL_TEXTURE_MAX_LOD                 0x813B
#define GL_TEXTURE_BASE_LEVEL              0x813C
#define GL_TEXTURE_MAX_LEVEL               0x813D

#define GL_RG                              0x8227
#define GL_RG_INTEGER                      0x8228
#define GL_R8                              0x8229
#define GL_R16                             0x822A
#define GL_RG8                             0x822B
#define GL_RG16                            0x822C
#define GL_R16F                            0x822D
#define GL_R32F                            0x822E
#define GL_RG16F                           0x822F
#define GL_RG32F                           0x8230
#define GL_R8I                             0x8231
#define GL_R8UI                            0x8232
#define GL_R16I                            0x8233
#define GL_R16UI                           0x8234
#define GL_R32I                            0x8235
#define GL_R32UI                           0x8236
#define GL_RG8I                            0x8237
#define GL_RG8UI                           0x8238
#define GL_RG16I                           0x8239
#define GL_RG16UI                          0x823A
#define GL_RG32I                           0x823B
#define GL_RG32UI                          0x823C

#define GL_MIRRORED_REPEAT                 0x8370

#define GL_TEXTURE0                        0x84C0
#define GL_TEXTURE1                        0x84C1
#define GL_TEXTURE2                        0x84C2
#define GL_TEXTURE3                        0x84C3
#define GL_TEXTURE4                        0x84C4
#define GL_TEXTURE5                        0x84C5
#define GL_TEXTURE6                        0x84C6
#define GL_TEXTURE7                        0x84C7
#define GL_TEXTURE8                        0x84C8
#define GL_TEXTURE9                        0x84C9
#define GL_TEXTURE10                       0x84CA
#define GL_TEXTURE11                       0x84CB
#define GL_TEXTURE12                       0x84CC
#define GL_TEXTURE13                       0x84CD
#define GL_TEXTURE14                       0x84CE
#define GL_TEXTURE15                       0x84CF
#define GL_TEXTURE16                       0x84D0
#define GL_TEXTURE17                       0x84D1
#define GL_TEXTURE18                       0x84D2
#define GL_TEXTURE19                       0x84D3
#define GL_TEXTURE20                       0x84D4
#define GL_TEXTURE21                       0x84D5
#define GL_TEXTURE22                       0x84D6
#define GL_TEXTURE23                       0x84D7
#define GL_TEXTURE24                       0x84D8
#define GL_TEXTURE25                       0x84D9
#define GL_TEXTURE26                       0x84DA
#define GL_TEXTURE27                       0x84DB
#define GL_TEXTURE28                       0x84DC
#define GL_TEXTURE29                       0x84DD
#define GL_TEXTURE30                       0x84DE
#define GL_TEXTURE31                       0x84DF
#define GL_ACTIVE_TEXTURE                  0x84E0

#define GL_ARRAY_BUFFER                    0x8892
#define GL_ELEMENT_ARRAY_BUFFER            0x8893
#define GL_ARRAY_BUFFER_BINDING            0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING    0x8895

#define GL_STREAM_DRAW                     0x88E0
#define GL_STREAM_READ                     0x88E1
#define GL_STREAM_COPY                     0x88E2
#define GL_STATIC_DRAW                     0x88E4
#define GL_STATIC_READ                     0x88E5
#define GL_STATIC_COPY                     0x88E6
#define GL_DYNAMIC_DRAW                    0x88E8
#define GL_DYNAMIC_READ                    0x88E9
#define GL_DYNAMIC_COPY                    0x88EA

#define GL_FRAGMENT_SHADER                 0x8B30
#define GL_VERTEX_SHADER                   0x8B31

#define GL_BLEND_DST_RGB                   0x80C8
#define GL_BLEND_SRC_RGB                   0x80C9
#define GL_BLEND_DST_ALPHA                 0x80CA
#define GL_BLEND_SRC_ALPHA                 0x80CB
#define GL_BLEND_EQUATION_ALPHA            0x883D
#define GL_BLEND_EQUATION_RGB              0x8009
#define GL_DELETE_STATUS                   0x8B80
#define GL_COMPILE_STATUS                  0x8B81
#define GL_LINK_STATUS                     0x8B82
#define GL_VALIDATE_STATUS                 0x8B83
#define GL_INFO_LOG_LENGTH                 0x8B84
#define GL_ATTACHED_SHADERS                0x8B85
#define GL_ACTIVE_UNIFORMS                 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH       0x8B87
#define GL_SHADER_SOURCE_LENGTH            0x8B88
#define GL_ACTIVE_ATTRIBUTES               0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH     0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION        0x8B8C
#define GL_CURRENT_PROGRAM                 0x8B8D
#define GL_BLEND_COLOR                     0x8005
#define GL_BLEND_EQUATION                  0x8009
#define GL_CONSTANT_COLOR                  0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR        0x8002
#define GL_CONSTANT_ALPHA                  0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA        0x8004
#define GL_FUNC_ADD                        0x8006
#define GL_FUNC_REVERSE_SUBTRACT           0x800B
#define GL_FUNC_SUBTRACT                   0x800A
#define GL_MIN                             0x8007
#define GL_MAX                             0x8008
#define GL_VERTEX_ARRAY_BINDING            0x85B5
    
#if KDBDevelopmentBuild
#define OpenGLAssertNoError()                                              \
{                                                                          \
    GLenum Error = glGetError();                                           \
    if(Error != GL_NO_ERROR){                                              \
        char *ErrorString = OpenGLErrorString(Error);                      \
        Error("OpenGL error: %s (Error code: %d)", ErrorString, Error);    \
        Assert(0);                                                         \
    }                                                                      \
}

inline char *
OpenGLErrorString(GLenum Error){
    char *Result = 0;

    switch(Error){
        case GL_NO_ERROR:{
            Result = "NoError";
        } break;
        case GL_INVALID_ENUM:{
            Result = "InvalidEnum";
        } break;
        case GL_INVALID_VALUE:{
            Result = "InvalidValue";
        } break;
        case GL_INVALID_OPERATION:{
            Result = "InvalidOp";
        } break;
        case GL_STACK_OVERFLOW:{
            Result = "StackOverflow";
        } break;
        case GL_STACK_UNDERFLOW:{
            Result = "StackUnderflow";
        } break;
        case GL_OUT_OF_MEMORY:{
            Result = "OutOfMemory";
        } break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:{
            Result = "InvalidFramebufferOp";
        } break;
        case GL_CONTEXT_LOST:{
            Result = "ContextLost";
        } break;
        UnexpectedDefaultCase;
    }

    return Result;
}
#else
#define OpenGLAssertNoError()
#endif

#define DeclareOpenGLPlatformAPI(PlatformPrefix);                        \
        extern opengl_platform_initialise   PlatformPrefix##Initialise;  \
        extern opengl_platform_swap_buffers PlatformPrefix##SwapBuffers;
    
#define SetOpenGLPlatformAPI(PlatformPrefix);                                                      \
        static constexpr opengl_platform_initialise   *Initialise  = &PlatformPrefix##Initialise;  \
        static constexpr opengl_platform_swap_buffers *SwapBuffers = &PlatformPrefix##SwapBuffers;

typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

typedef char GLchar;
typedef short GLshort;
typedef signed char GLbyte;
typedef unsigned short GLushort;

typedef unsigned short GLhalf;

typedef uint64_t GLuint64;
typedef int64_t GLint64;

PlatformOpenGLFuncDeclare(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha);
PlatformOpenGLFuncDeclare(void, glDrawBuffers, GLsizei n, const GLenum *bufs);
PlatformOpenGLFuncDeclare(void, glStencilOpSeparate, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
PlatformOpenGLFuncDeclare(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask);
PlatformOpenGLFuncDeclare(void, glStencilMaskSeparate, GLenum face, GLuint mask);

PlatformOpenGLFuncDeclare(void, glAttachShader, GLuint program, GLuint shader);
PlatformOpenGLFuncDeclare(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name);
PlatformOpenGLFuncDeclare(void, glCompileShader, GLuint shader);

PlatformOpenGLFuncDeclare(GLuint, glCreateProgram, void);
PlatformOpenGLFuncDeclare(GLuint, glCreateShader, GLenum type);

PlatformOpenGLFuncDeclare(void, glDeleteProgram, GLuint program);
PlatformOpenGLFuncDeclare(void, glDeleteShader, GLuint shader);
PlatformOpenGLFuncDeclare(void, glDetachShader, GLuint program, GLuint shader);
PlatformOpenGLFuncDeclare(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params);
PlatformOpenGLFuncDeclare(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
PlatformOpenGLFuncDeclare(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params);
PlatformOpenGLFuncDeclare(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
PlatformOpenGLFuncDeclare(void, glGetShaderSource, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
PlatformOpenGLFuncDeclare(void, glLinkProgram, GLuint program);
PlatformOpenGLFuncDeclare(void, glShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
PlatformOpenGLFuncDeclare(void, glUseProgram, GLuint program);
PlatformOpenGLFuncDeclare(void, glValidateProgram, GLuint program);

PlatformOpenGLFuncDeclare(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
PlatformOpenGLFuncDeclare(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
PlatformOpenGLFuncDeclare(void, glGetAttachedShaders, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
PlatformOpenGLFuncDeclare(void, glGetUniformfv, GLuint program, GLint location, GLfloat *params);
PlatformOpenGLFuncDeclare(void, glGetUniformiv, GLuint program, GLint location, GLint *params);

PlatformOpenGLFuncDeclare(GLint, glGetAttribLocation, GLuint program, const GLchar *name);
PlatformOpenGLFuncDeclare(GLint, glGetUniformLocation, GLuint program, const GLchar *name);

PlatformOpenGLFuncDeclare(GLboolean, glIsProgram, GLuint program);
PlatformOpenGLFuncDeclare(GLboolean, glIsShader, GLuint shader);

PlatformOpenGLFuncDeclare(void, glDisableVertexAttribArray, GLuint index);
PlatformOpenGLFuncDeclare(void, glEnableVertexAttribArray, GLuint index);
PlatformOpenGLFuncDeclare(void, glGetVertexAttribdv, GLuint index, GLenum pname, GLdouble *params);
PlatformOpenGLFuncDeclare(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat *params);
PlatformOpenGLFuncDeclare(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint *params);
PlatformOpenGLFuncDeclare(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, void **pointer);

PlatformOpenGLFuncDeclare(void, glUniform1f, GLint location, GLfloat v0);
PlatformOpenGLFuncDeclare(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1);
PlatformOpenGLFuncDeclare(void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
PlatformOpenGLFuncDeclare(void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

PlatformOpenGLFuncDeclare(void, glUniform1i, GLint location, GLint v0);
PlatformOpenGLFuncDeclare(void, glUniform2i, GLint location, GLint v0, GLint v1);
PlatformOpenGLFuncDeclare(void, glUniform3i, GLint location, GLint v0, GLint v1, GLint v2);
PlatformOpenGLFuncDeclare(void, glUniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

PlatformOpenGLFuncDeclare(void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value);
PlatformOpenGLFuncDeclare(void, glUniform2fv, GLint location, GLsizei count, const GLfloat *value);
PlatformOpenGLFuncDeclare(void, glUniform3fv, GLint location, GLsizei count, const GLfloat *value);
PlatformOpenGLFuncDeclare(void, glUniform4fv, GLint location, GLsizei count, const GLfloat *value);

PlatformOpenGLFuncDeclare(void, glUniform1iv, GLint location, GLsizei count, const GLint *value);
PlatformOpenGLFuncDeclare(void, glUniform2iv, GLint location, GLsizei count, const GLint *value);
PlatformOpenGLFuncDeclare(void, glUniform3iv, GLint location, GLsizei count, const GLint *value);
PlatformOpenGLFuncDeclare(void, glUniform4iv, GLint location, GLsizei count, const GLint *value);

PlatformOpenGLFuncDeclare(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
PlatformOpenGLFuncDeclare(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
PlatformOpenGLFuncDeclare(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PlatformOpenGLFuncDeclare(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

PlatformOpenGLFuncDeclare(void, glVertexAttrib1d, GLuint index, GLdouble x);
PlatformOpenGLFuncDeclare(void, glVertexAttrib1f, GLuint index, GLfloat x);
PlatformOpenGLFuncDeclare(void, glVertexAttrib1s, GLuint index, GLshort x);
PlatformOpenGLFuncDeclare(void, glVertexAttrib2d, GLuint index, GLdouble x, GLdouble y);
PlatformOpenGLFuncDeclare(void, glVertexAttrib2f, GLuint index, GLfloat x, GLfloat y);
PlatformOpenGLFuncDeclare(void, glVertexAttrib2s, GLuint index, GLshort x, GLshort y);
PlatformOpenGLFuncDeclare(void, glVertexAttrib3d, GLuint index, GLdouble x, GLdouble y, GLdouble z);
PlatformOpenGLFuncDeclare(void, glVertexAttrib3f, GLuint index, GLfloat x, GLfloat y, GLfloat z);
PlatformOpenGLFuncDeclare(void, glVertexAttrib3s, GLuint index, GLshort x, GLshort y, GLshort z);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4Nub, GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4f, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4s, GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);

PlatformOpenGLFuncDeclare(void, glVertexAttrib1dv, GLuint index, const GLdouble *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib1fv, GLuint index, const GLfloat *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib1sv, GLuint index, const GLshort *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib2dv, GLuint index, const GLdouble *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib2fv, GLuint index, const GLfloat *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib2sv, GLuint index, const GLshort *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib3dv, GLuint index, const GLdouble *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib3fv, GLuint index, const GLfloat *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib3sv, GLuint index, const GLshort *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4Nbv, GLuint index, const GLbyte *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4Niv, GLuint index, const GLint *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4Nsv, GLuint index, const GLshort *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4Nubv, GLuint index, const GLubyte *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4Nuiv, GLuint index, const GLuint *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4Nusv, GLuint index, const GLushort *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4bv, GLuint index, const GLbyte *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4dv, GLuint index, const GLdouble *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4fv, GLuint index, const GLfloat *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4iv, GLuint index, const GLint *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4sv, GLuint index, const GLshort *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4ubv, GLuint index, const GLubyte *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4uiv, GLuint index, const GLuint *v);
PlatformOpenGLFuncDeclare(void, glVertexAttrib4usv, GLuint index, const GLushort *v);

PlatformOpenGLFuncDeclare(void, glBindVertexArray, GLuint array);
PlatformOpenGLFuncDeclare(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays);
PlatformOpenGLFuncDeclare(void, glGenVertexArrays, GLsizei n, GLuint *arrays);

PlatformOpenGLFuncDeclare(GLboolean, glIsVertexArray, GLuint array);

PlatformOpenGLFuncDeclare(void, glGenQueries, GLsizei n, GLuint *ids);
PlatformOpenGLFuncDeclare(void, glDeleteQueries, GLsizei n, const GLuint *ids);

PlatformOpenGLFuncDeclare(GLboolean, glIsQuery, GLuint id);

PlatformOpenGLFuncDeclare(void, glBeginQuery, GLenum target, GLuint id);
PlatformOpenGLFuncDeclare(void, glEndQuery, GLenum target);
PlatformOpenGLFuncDeclare(void, glGetQueryiv, GLenum target, GLenum pname, GLint *params);
PlatformOpenGLFuncDeclare(void, glGetQueryObjectiv, GLuint id, GLenum pname, GLint *params);
PlatformOpenGLFuncDeclare(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params);

PlatformOpenGLFuncDeclare(void, glBindBuffer, GLenum target, GLuint buffer);
PlatformOpenGLFuncDeclare(void, glDeleteBuffers, GLsizei n, const GLuint *buffers);
PlatformOpenGLFuncDeclare(void, glGenBuffers, GLsizei n, GLuint *buffers);
PlatformOpenGLFuncDeclare(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage);
PlatformOpenGLFuncDeclare(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
PlatformOpenGLFuncDeclare(void, glGetBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, void *data);
PlatformOpenGLFuncDeclare(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint *params);
PlatformOpenGLFuncDeclare(void, glGetBufferPointerv, GLenum target, GLenum pname, void **params);

PlatformOpenGLFuncDeclare(void *, glMapBuffer, GLenum target, GLenum access);

PlatformOpenGLFuncDeclare(GLboolean, glIsBuffer, GLuint buffer);
PlatformOpenGLFuncDeclare(GLboolean, glUnmapBuffer, GLenum target);

PlatformOpenGLFuncDeclare(void, glActiveTexture, GLenum texture);
PlatformOpenGLFuncDeclare(void, glGenerateMipmap, GLenum target);
PlatformOpenGLFuncDeclare(void, glBlendEquation , GLenum mode);
PlatformOpenGLFuncDeclare(void, glBlendFuncSeparate , 
                          GLenum sfactorRGB, GLenum dfactorRGB, 
                          GLenum sfactorAlpha, GLenum dfactorAlpha);

namespace kadabra {
    
    typedef b32 opengl_platform_initialise(renderer_opengl_platform *Platform, window *Window);
    typedef b32 opengl_platform_swap_buffers(window *Window);
    
    DeclareOpenGLPlatformAPI(Win32);
    
    class opengl_platform {
        public:
        #if _WIN32
            SetOpenGLPlatformAPI(Win32);
        #elif
            #error "OpenGL's platform layer is not implemented for this platform!"
        #endif
    };
    
    enum opengl_drawable_flags {
        OpenGLDrawable_Stream          = 0x01,
        OpenGLDrawable_Static          = 0x02,
        OpenGLDrawable_Dynamic         = 0x04,
    
        OpenGLDrawable_Draw            = 0x08,
        OpenGLDrawable_Read            = 0x10,
        OpenGLDrawable_Copy            = 0x20,
    };
    
    struct opengl_shader {
        u32 Program;
    };
    
    struct opengl_shader_standard_vertex_layout {
        s32 LocPosition;
        s32 LocColor;
        s32 LocNormal;
        s32 LocTexCoord0;
    };
    
    struct opengl_shader_standard_uniform_layout {
        s32 LocAmbientLight;
        s32 LocTexture0;
        s32 LocModelMat;
        s32 LocViewMat;
        s32 LocProjMat;
    };
    
    struct opengl_shader_standard {
        opengl_shader_standard_vertex_layout VertexLayout;
        opengl_shader_standard_uniform_layout UniformLayout;
        opengl_shader Common;
    };
    
    struct opengl_texture {
        char Identifier[64];
        u32 Handle;
    };
    
    struct opengl_texture_manager {
        opengl_texture Textures[OpenGLTextureManagerSize];
        u32 TextureCount;
    };
    
    struct opengl_drawable {
        GLenum DrawMode;
    
        u32 ID;
        u32 Flags;
    
        // TODO(furkan): Separate VAO
        u32 VertexArrayObject;
        u32 VertexBuffer;
        u32 IndexBuffer;
        
        s32 DrawElementCount;
        s32 DrawOffset;
    };
    
    struct opengl_drawable_manager {
        opengl_drawable Drawables[OpenGLDrawableManagerSize];
        u32 DrawableCount;
    };
    
    class renderer_opengl {
        public:
        renderer_opengl_platform Platform;
        
        void LoadFunctions();
        b32 OnWindowResize(window *Window);
        b32 OnInitialise(window *Window);
        b32 Initialise(window *Window);
        b32 Finalise();
        b32 RenderScene(asset_manager *AssetManager, 
                        scene *Scene, camera *Camera, 
                        window *Window, renderer_polygon_mode PolygonMode);
        
        private:
        opengl_shader_standard StandardShader;
        
        opengl_texture_manager TextureManager;
        opengl_drawable_manager DrawableManager;
        
        b32 AttribLocation(GLint *Loc, GLuint Program, char *Name);
        b32 UniformLocation(GLint *Loc, GLuint Program, char *Name);
        b32 Attrib(GLint Loc, GLint Components, GLvoid *Ptr);
        
        b32 LinkDrawableToStdShader(opengl_drawable *Drawable, 
                                    opengl_shader_standard_vertex_layout *VertLt);
        
        b32 CompileShader(opengl_shader *Shader, 
                          char *VSCode, size_t VSCodeLength, 
                          char *FSCode, size_t FSCodeLength);
        b32 CompileStandardShader(opengl_shader_standard *Shader);
        
        
        opengl_texture *GetTexture(asset_image *Image);
        opengl_drawable *GetDrawable(component_renderable *Renderable);
        
        b32 CreateDrawable(opengl_drawable *Drawable, 
                           asset_mesh *Mesh, u32 Flags);
        b32 CreateTexture(opengl_texture *OpenGLTexture, 
                          asset_image *Image, 
                          texture_filter MinFilterType, 
                          texture_filter MagFilterType, 
                          texture_wrap HorizontalWrap, 
                          texture_wrap VerticalWrap, 
                          s32 InternalFormat, 
                          b32 GenerateMipmap=true, 
                          s32 MipmapLevel=0);
        
        void RenderRenderable(component_renderable *Renderable, 
                              component_transform *Transform, 
                              mat4 *ViewMatrixTran, 
                              mat4 *ProjMatrixTran);
    };
    
    
}

#endif