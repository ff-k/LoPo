#include "kdb_renderer_common.h"
#include "kdb_renderer_opengl.h"

#include "kdb_string.h"

#define OpenGLSetShaderAttribLocation(Program, VertexLayout, Variable) \
          AttribLocation(&(VertexLayout->Loc##Variable),               \
                         Program, #Variable)

#define OpenGLSetShaderUniformLocation(Program, UniformLayout, Variable) \
          UniformLocation(&(UniformLayout->Loc##Variable),               \
                          Program, #Variable)

#define OpenGLEnableAttrib(VertexLayout, Components, Variable) \
          Attrib(VertexLayout->Loc##Variable, Components,      \
                 (void *)OffsetOf(vertex, Variable))

namespace kadabra {
    b32
    renderer_opengl::AttribLocation(GLint *Loc, GLuint Program, char *Name){
        b32 Success = false;
    
        GLint L = glGetAttribLocation(Program, Name);
        if(L != -1){
            *Loc = L;
            Success = true;
        } else {
            Error("glGetAttribLocation failed for %s", Name);
        }
    
        return Success;
    }
    
    b32
    renderer_opengl::UniformLocation(GLint *Loc, GLuint Program, char *Name){
        b32 Success = false;
    
        GLint L = glGetUniformLocation(Program, Name);
        if(L != -1){
            *Loc = L;
            Success = true;
        } else {
            Error("glGetUniformLocation failed for %s", Name);
        }
    
        return Success;
    }
    
    b32
    renderer_opengl::Attrib(GLint Loc, GLint Components, GLvoid *Ptr){
        b32 Success = true;
    
        if(Loc != -1){
            glVertexAttribPointer(Loc, Components, GL_FLOAT, GL_FALSE, 
                                sizeof(vertex), Ptr);
            glEnableVertexAttribArray(Loc);
        } else {
            Success = false;
        }
    
        return Success;
    }
    
    b32
    renderer_opengl::LinkDrawableToStdShader(opengl_drawable *Drawable, 
                                             opengl_shader_standard_vertex_layout *VertLt){
        b32 Success = true;
        // TODO(furkan): Parse shaders and detect per-vertex data. Currently, 
        // it will be a nightmare to change a thing. Even changing a variable
        // type from vec3 and vec4 will be a pain in the butt.
    
        glBindVertexArray(Drawable->VertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, Drawable->VertexBuffer);
    
        OpenGLEnableAttrib(VertLt, 3, Position);
        OpenGLEnableAttrib(VertLt, 3, Color);
        OpenGLEnableAttrib(VertLt, 3, Normal);
        OpenGLEnableAttrib(VertLt, 2, TexCoord0);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
        return Success;
    }
    
    b32
    renderer_opengl::CompileShader(opengl_shader *Shader,
                                   char *VSCode, size_t VSCodeLength,
                                   char *FSCode, size_t FSCodeLength){
        b32 Success = false;
    
        char OperationInfo[512];
        s32 OperationSuccess = 0;
    
        u32 VS = glCreateShader(GL_VERTEX_SHADER);
        if(glIsShader(VS)){
            glShaderSource(VS, 1, &VSCode, (GLint *)&VSCodeLength);
            glCompileShader(VS);
        
            glGetShaderiv(VS, GL_COMPILE_STATUS, &OperationSuccess);
            if(!OperationSuccess){
                glGetShaderInfoLog(VS, StaticArraySize(OperationInfo), 
                                0, OperationInfo);
                Error("Vertex shader compilation failed : %s", OperationInfo);
                VS = 0;
            }
        } else {
            Error("glCreateShader(VS) failed.");
        }
    
        u32 FS = glCreateShader(GL_FRAGMENT_SHADER);
        if(glIsShader(FS)){
            glShaderSource(FS, 1, &FSCode, (GLint *)&FSCodeLength);
            glCompileShader(FS);
    
            glGetShaderiv(FS, GL_COMPILE_STATUS, &OperationSuccess);
            if(!OperationSuccess){
                glGetShaderInfoLog(FS, StaticArraySize(OperationInfo), 
                                0, OperationInfo);
                Error("Fragment shader compilation failed : %s", OperationInfo);
                FS = 0;
            }
        } else {
            Error("glCreateShader(FS) failed.");
        }
    
        if(VS && FS){
            
            u32 ShaderProgram = glCreateProgram();
            if(glIsProgram(ShaderProgram)){
                glAttachShader(ShaderProgram, VS);
                glAttachShader(ShaderProgram, FS);
                glLinkProgram(ShaderProgram);
        
                glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &OperationSuccess);
                if(!OperationSuccess) {
                    glGetProgramInfoLog(ShaderProgram, 
                                        StaticArraySize(OperationInfo), 
                                        0, OperationInfo);
                    Error("Shader program linkage failed : %s", OperationInfo);
                    ShaderProgram = 0;
                } else {
                    glValidateProgram(ShaderProgram);
    
                    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, 
                                &OperationSuccess);
                    if(!OperationSuccess){
                        glGetProgramInfoLog(ShaderProgram, 
                                            StaticArraySize(OperationInfo), 
                                            0, OperationInfo);
                        Error("Shader program validation failed : %s", 
                                                                OperationInfo);
    
                        ShaderProgram = 0;
                    } else {
                        glDetachShader(ShaderProgram, VS);
                        glDetachShader(ShaderProgram, FS);
    
                        glDeleteShader(VS);
                        glDeleteShader(FS);
                        
                        Shader->Program = ShaderProgram;
    
                        Success = true;
                    }
                }       
            } else {
                Error("glCreateProgram() failed.");       
            }
        }
    
        return Success;
    }
    
    b32 
    renderer_opengl::CompileStandardShader(opengl_shader_standard *Shader){
        
        b32 Success = false;
        
        char *VSCode = 0;
        size_t VSCodeLength = 0;
        char *FSCode = 0;
        size_t FSCodeLength = 0;
        if(ReadEntireFile("..\\..\\assets\\shaders\\Standard_VS.vert",
                        (void **)&VSCode, &VSCodeLength) && 
           ReadEntireFile("..\\..\\assets\\shaders\\Standard_FS.frag",
                        (void **)&FSCode, &FSCodeLength)){
            if(CompileShader(&Shader->Common, 
                             VSCode, VSCodeLength,
                             FSCode, FSCodeLength)){
                Success = true;
    
                u32 Program = Shader->Common.Program;
                opengl_shader_standard_vertex_layout  *VL = &Shader->VertexLayout;
                opengl_shader_standard_uniform_layout *UL = &Shader->UniformLayout;
    
                OpenGLSetShaderAttribLocation (Program, VL, Position );
                OpenGLSetShaderAttribLocation (Program, VL, Color    );
                OpenGLSetShaderAttribLocation (Program, VL, Normal   );
                OpenGLSetShaderAttribLocation (Program, VL, TexCoord0);
                OpenGLSetShaderAttribLocation (Program, VL, Position );
    
                OpenGLSetShaderUniformLocation(Program, UL, AmbientLight);
                OpenGLSetShaderUniformLocation(Program, UL, Texture0    );
                OpenGLSetShaderUniformLocation(Program, UL, ModelMat    );
                OpenGLSetShaderUniformLocation(Program, UL, ViewMat     );
                OpenGLSetShaderUniformLocation(Program, UL, ProjMat     );
    
                if(!Success){
                    glDeleteProgram(Shader->Common.Program);
                    Shader->Common.Program = 0;
                }
            }
        }
    
        if(VSCode){
            if(!platform::MemoryFree((void **)&VSCode)){
                Warning("VSCode could not be freed!");
            }
        }
        
        if(FSCode){
            if(!platform::MemoryFree((void **)&FSCode)){
                Warning("FSCode could not be freed!");       
            }
        }
    
        return Success;
    }
    
    opengl_texture *
    renderer_opengl::GetTexture(asset_image *Image){
        opengl_texture *Result = 0;
        for(u32 i=0; 
                i < TextureManager.TextureCount;
                i++){
            opengl_texture *T = TextureManager.Textures + i;
            if(!StringCompareN(T->Identifier, 
                               StaticArraySize(T->Identifier),
                               Image->Path, 
                               Image->PathLength)){
                Result = T;
                break;
            }
        }
    
        if(!Result){
            if(TextureManager.TextureCount < OpenGLTextureManagerSize){
                
                s32 InternalFormat = GL_RGB;
                switch(Image->Channels){
                    case 3: {
                        InternalFormat = GL_RGB;
                    } break;
                    case 4: {
                        InternalFormat = GL_RGBA;
                    } break;
                    UnexpectedDefaultCase;
                }
                
                opengl_texture *T = TextureManager.Textures + 
                                    TextureManager.TextureCount;
                if(CreateTexture(T, Image, 
                                 TextureFilter_LinearMipmapLinear, 
                                 TextureFilter_Linear, 
                                 TextureWrap_ClampToEdge, 
                                 TextureWrap_ClampToEdge, 
                                 InternalFormat)){
                    Result = T;
                    TextureManager.TextureCount++;
                }
            } else {
                Warning("TextureManager is full (%u, %u)", 
                                                    TextureManager.TextureCount,
                                                    OpenGLTextureManagerSize);
            }
        }
    
        return Result;
    }
    
    opengl_drawable *
    renderer_opengl::GetDrawable(component_renderable *Renderable){
        opengl_drawable *Result = 0;
        for(u32 i=0;
                i < DrawableManager.DrawableCount;
                i++){
            opengl_drawable *Drawable = DrawableManager.Drawables + i;
            if(Drawable->ID == Renderable->Mesh->ID){
                Result = Drawable;
                break;
            }
        }
    
        if(!Result){
            if(DrawableManager.DrawableCount < OpenGLDrawableManagerSize){
                opengl_drawable *Drawable = DrawableManager.Drawables + 
                                            DrawableManager.DrawableCount;
                if(CreateDrawable(Drawable, Renderable->Mesh, 0)){
                    switch(Renderable->Material->Shader){
                        case ShaderType_Standard:{
                            opengl_shader_standard_vertex_layout *VertLt;
                            VertLt = &StandardShader.VertexLayout;
                            if(LinkDrawableToStdShader(Drawable, VertLt)){ 
                                Result = Drawable;
                                DrawableManager.DrawableCount++;
                            }
                        } break;
                        UnexpectedDefaultCase;
                    }
                }
            }
        }
    
        return Result;
    }
    
    b32 
    renderer_opengl::CreateTexture(opengl_texture *OpenGLTexture, 
                                   asset_image *Image, 
                                   texture_filter MinFilterType, 
                                   texture_filter MagFilterType, 
                                   texture_wrap HorizontalWrap, 
                                   texture_wrap VerticalWrap, 
                                   s32 InternalFormat, 
                                   b32 GenerateMipmap, s32 MipmapLevel){
    
        b32 Success = true;
    
        u32 Texture = 0;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        if(glIsTexture(Texture)){
            GLint MinFilter = GL_LINEAR_MIPMAP_LINEAR;
            switch(MinFilterType){
                case TextureFilter_Nearest:{
                    MinFilter = GL_NEAREST;
                } break;
                case TextureFilter_Linear:{
                    MinFilter = GL_LINEAR;
                } break;
                case TextureFilter_NearestMipmapNearest:{
                    MinFilter = GL_NEAREST_MIPMAP_NEAREST;
                } break;
                case TextureFilter_LinearMipmapNearest:{
                    MinFilter = GL_LINEAR_MIPMAP_NEAREST;
                } break;
                case TextureFilter_NearestMipmapLinear:{
                    MinFilter = GL_NEAREST_MIPMAP_LINEAR;
                } break;
                case TextureFilter_LinearMipmapLinear:{
                    MinFilter = GL_LINEAR_MIPMAP_LINEAR;
                } break;
                UnexpectedDefaultCase;
            }
    
            if(!GenerateMipmap && 
            (MinFilter == TextureFilter_NearestMipmapNearest ||
                MinFilter == TextureFilter_LinearMipmapNearest  ||
                MinFilter == TextureFilter_NearestMipmapLinear  ||
                MinFilter == TextureFilter_LinearMipmapLinear)){
                Warning("Mipmap filter is detected when mipmap generation is disabled");
            }
    
            GLint MagFilter = GL_LINEAR;
            switch(MagFilterType){
                case TextureFilter_Nearest:{
                    MagFilter = GL_NEAREST;
                } break;
                case TextureFilter_Linear:{
                    MagFilter = GL_LINEAR;
                } break;
                // NOTE(furkan): GL_X_MIPMAP_Y filters are not
                // available for magnification.
                UnexpectedDefaultCase;
            }
    
            glTexParameteri(GL_TEXTURE_2D, 
                            GL_TEXTURE_MIN_FILTER, MinFilter);
            glTexParameteri(GL_TEXTURE_2D, 
                            GL_TEXTURE_MAG_FILTER, MagFilter);
    
            GLint WrapHorizontal = GL_CLAMP_TO_EDGE;
            switch(HorizontalWrap){
                case TextureWrap_ClampToEdge:{
                    WrapHorizontal = GL_CLAMP_TO_EDGE;
                } break;
                case TextureWrap_ClampToBorder:{
                    WrapHorizontal = GL_CLAMP_TO_BORDER;
                } break;
                case TextureWrap_MirroredRepeat:{
                    WrapHorizontal = GL_MIRRORED_REPEAT;
                } break;
                case TextureWrap_Repeat:{
                    WrapHorizontal = GL_REPEAT;
                } break;
                UnexpectedDefaultCase;
            }
    
            GLint WrapVertical = GL_CLAMP_TO_EDGE;
            switch(VerticalWrap){
                case TextureWrap_ClampToEdge:{
                    WrapVertical = GL_CLAMP_TO_EDGE;
                } break;
                case TextureWrap_ClampToBorder:{
                    WrapVertical = GL_CLAMP_TO_BORDER;
                } break;
                case TextureWrap_MirroredRepeat:{
                    WrapVertical = GL_MIRRORED_REPEAT;
                } break;
                case TextureWrap_Repeat:{
                    WrapVertical = GL_REPEAT;
                } break;
                UnexpectedDefaultCase;
            }
    
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapHorizontal);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapVertical);
    
            GLenum TexFormat = GL_RGB;
            switch(Image->Channels){
                case 1:{
                    TexFormat = GL_RED;
                } break;
                case 2:{
                    TexFormat = GL_RG;
                } break;
                case 3:{
                    TexFormat = GL_RGB;
                    if(Image->ReverseComponents){
                        TexFormat = GL_BGR;
                    }
                } break;
                case 4:{
                    TexFormat = GL_RGBA;
                    if(Image->ReverseComponents){
                        TexFormat = GL_BGRA;
                    }
                } break;
                UnexpectedDefaultCase;
            }
    
            glTexImage2D(GL_TEXTURE_2D, MipmapLevel, InternalFormat, 
                         Image->Width, Image->Height, 
                         0, TexFormat, GL_UNSIGNED_BYTE, 
                         Image->Pixels);
                        
            if(GenerateMipmap){
                glGenerateMipmap(GL_TEXTURE_2D);
            }
    
            if(Success){
                if(!platform::MemoryCopy(OpenGLTexture->Identifier, 
                                    StaticArraySize(OpenGLTexture->Identifier), 
                                    Image->Path, 
                                    Image->PathLength)){
                    Warning("An error occured while copying OpenGLTextureID");
                }
                OpenGLTexture->Handle = Texture;
            }
        } else {
            Success = false;
        }
    
        return Success;
    }
    
    b32
    renderer_opengl::CreateDrawable(opengl_drawable *Drawable, 
                                    asset_mesh *Mesh, u32 Flags){
        b32 Success = true;
    
        u32 VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        if(glIsVertexArray(VAO)){
    
            u32 VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            if(glIsBuffer(VBO)){
    
                //NOTE(furkan): Default VBO usage
                GLenum VBOUsage = GL_STATIC_DRAW;
    
                if(Flags & OpenGLDrawable_Stream){
                    if(Flags & OpenGLDrawable_Draw){
                        VBOUsage = GL_STREAM_DRAW;
                    } else if(Flags & OpenGLDrawable_Read){
                        VBOUsage = GL_STREAM_READ;
                    } else if(Flags & OpenGLDrawable_Copy){
                        VBOUsage = GL_STREAM_COPY;
                    } else {
                        //NOTE(furkan): Default stream VBO usage
                        VBOUsage = GL_STREAM_DRAW;
                    }
                } else if(Flags & OpenGLDrawable_Static){
                    if(Flags & OpenGLDrawable_Draw){
                        VBOUsage = GL_STATIC_DRAW;
                    } else if(Flags & OpenGLDrawable_Read){
                        VBOUsage = GL_STATIC_READ;
                    } else if(Flags & OpenGLDrawable_Copy){
                        VBOUsage = GL_STATIC_COPY;
                    } else {
                        //NOTE(furkan): Default static VBO usage
                        VBOUsage = GL_STATIC_DRAW;       
                    }
                } else if(Flags & OpenGLDrawable_Dynamic){
                    if(Flags & OpenGLDrawable_Draw){
                        VBOUsage = GL_DYNAMIC_DRAW;
                    } else if(Flags & OpenGLDrawable_Read){
                        VBOUsage = GL_DYNAMIC_READ;
                    } else if(Flags & OpenGLDrawable_Copy){
                        VBOUsage = GL_DYNAMIC_COPY;
                    } else {
                        //NOTE(furkan): Default dynamic VBO usage
                        VBOUsage = GL_DYNAMIC_DRAW;       
                    }
                }
                
                const u32 AABBVertexCount = 16;
    
                vec3 Min = Mesh->AABB.Min;
                vec3 Max = Mesh->AABB.Max;
    
                vertex AABBVertices[AABBVertexCount] = { 0 };
                AABBVertices[ 0].Position = Vec3(Max.x, Min.y, Max.z);
                AABBVertices[ 1].Position = Vec3(Min.x, Min.y, Max.z);
                AABBVertices[ 2].Position = Vec3(Min.x, Min.y, Min.z);
                AABBVertices[ 3].Position = Vec3(Max.x, Min.y, Min.z);
                AABBVertices[ 4].Position = Vec3(Max.x, Min.y, Max.z);
                AABBVertices[ 5].Position = Vec3(Max.x, Max.y, Max.z);
                AABBVertices[ 6].Position = Vec3(Min.x, Max.y, Max.z);
                AABBVertices[ 7].Position = Vec3(Min.x, Max.y, Min.z);
                AABBVertices[ 8].Position = Vec3(Max.x, Max.y, Min.z);
                AABBVertices[ 9].Position = Vec3(Max.x, Max.y, Max.z);
                AABBVertices[10].Position = Vec3(Min.x, Max.y, Max.z);
                AABBVertices[11].Position = Vec3(Min.x, Min.y, Max.z);
                AABBVertices[12].Position = Vec3(Min.x, Max.y, Min.z);
                AABBVertices[13].Position = Vec3(Min.x, Min.y, Min.z);
                AABBVertices[14].Position = Vec3(Max.x, Max.y, Min.z);
                AABBVertices[15].Position = Vec3(Max.x, Min.y, Min.z);
                AABBVertices[ 0].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 1].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 2].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 3].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 4].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 5].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 6].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 7].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 8].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[ 9].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[10].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[11].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[12].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[13].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[14].Color = Vec3(1.0f, 0.0f, 1.0f);
                AABBVertices[15].Color = Vec3(1.0f, 0.0f, 1.0f);
    
                glBufferData(GL_ARRAY_BUFFER, 
                            sizeof(vertex) * (Mesh->VertexCount + 
                                            AABBVertexCount), 
                            0, VBOUsage);
    
                glBufferSubData(GL_ARRAY_BUFFER, 
                                0, 
                                sizeof(vertex)*Mesh->VertexCount, 
                                Mesh->Vertices);
    
                glBufferSubData(GL_ARRAY_BUFFER, 
                                sizeof(vertex)*Mesh->VertexCount, 
                                sizeof(vertex)*AABBVertexCount,
                                AABBVertices);
    
                Drawable->VertexArrayObject = VAO;
                Drawable->VertexBuffer = VBO;
    
                switch(Mesh->DrawPrimitive){
                    case DrawPrimitive_Point:{
                        Drawable->DrawMode = GL_POINTS;
                    } break;
                    case DrawPrimitive_Line:{
                        Drawable->DrawMode = GL_LINES;
                    } break;
                    case DrawPrimitive_Triangle:{
                        Drawable->DrawMode = GL_TRIANGLES;
                    } break;
                    UnexpectedDefaultCase;
                }
    
                Drawable->DrawElementCount = Mesh->VertexCount;
                Drawable->DrawOffset = 0;
                
                if(Mesh->Indices && Mesh->IndexCount > 0){
                    // TODO(furkan): Make IndexBufferUsage a separate thing?
                    GLenum IndexBufferUsage = VBOUsage;
        
                    u32 IndexBuffer;
                    glGenBuffers(1, &IndexBuffer);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
                    if(glIsBuffer(IndexBuffer)){
                        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                                    sizeof(Mesh->Indices[0]) * Mesh->IndexCount, 
                                    Mesh->Indices, IndexBufferUsage);
                        Drawable->IndexBuffer = IndexBuffer;
        
                        Drawable->DrawElementCount = Mesh->IndexCount;
                        Drawable->DrawOffset = 0;               
                    }
                } else {
                    Drawable->IndexBuffer = 0;
                }
        
                Drawable->ID = Mesh->ID;
                Drawable->Flags = Flags; 
            } else {
                glDeleteVertexArrays(1, &VAO);
                Success = false;
            }
        } else {
            Success = false;
        }
    
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
        return Success;
    }
    
    void
    renderer_opengl::RenderRenderable(component_renderable *Renderable, 
                                      component_transform *Transform, 
                                      mat4 *ViewMatrixTran, 
                                      mat4 *ProjMatrixTran){
        if(Renderable->ToBeRendered &&
           Renderable->Mesh && Renderable->Mesh->ID){
            mat4 ModelMatrix = Mat4Identity();        
            ModelMatrix = Mat4Translate(ModelMatrix, Transform->Position);
            ModelMatrix = Mat4Rotate(ModelMatrix, Transform->EulerRotation.z,
                                    RotationAxis_Z);
            ModelMatrix = Mat4Rotate(ModelMatrix, Transform->EulerRotation.y,
                                    RotationAxis_Y);
            ModelMatrix = Mat4Rotate(ModelMatrix, Transform->EulerRotation.x,
                                    RotationAxis_X);
            ModelMatrix = Mat4Scale(ModelMatrix, Transform->Scale);
        
            OpenGLAssertNoError();
        
            asset_material *Material = Renderable->Material;
            Assert(Material);
        
            opengl_drawable *Drawable = GetDrawable(Renderable);
            OpenGLAssertNoError();                                                       
            if(Drawable){
                glBindVertexArray(Drawable->VertexArrayObject);
                OpenGLAssertNoError();
        
                opengl_texture *Texture = 0;
                if(Material->Texture){
                    Texture = GetTexture(Material->Texture);
                }
        
                if(Texture){
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, Texture->Handle);
                }
        
                OpenGLAssertNoError();
        
                switch(Material->Shader){
                    case ShaderType_Standard:{
                        vec4 A = Material->AmbientLight;
    
                        opengl_shader_standard_uniform_layout *UniLt;
                        UniLt = &StandardShader.UniformLayout;
                        glUniform1i(UniLt->LocTexture0, 0);
                        glUniform4f(UniLt->LocAmbientLight, 
                                    A.x, A.y, A.z, A.w);
                        s32 LViewM  = UniLt->LocViewMat;
                        s32 LProjM  = UniLt->LocProjMat;
                        s32 LModelM = UniLt->LocModelMat;
                        glUniformMatrix4fv(LViewM, 1, GL_FALSE, 
                                           ViewMatrixTran->Ptr);
                        glUniformMatrix4fv(LProjM, 1, GL_FALSE, 
                                           ProjMatrixTran->Ptr);
                        glUniformMatrix4fv(LModelM, 1, GL_TRUE, 
                                           ModelMatrix.Ptr);
                    } break;
                    UnexpectedDefaultCase;
                }
        
                OpenGLAssertNoError();
    
                asset_mesh *Mesh = Renderable->Mesh;
                switch(Mesh->DrawPrimitive){
                    case DrawPrimitive_Point:{
                        glPointSize(Mesh->PointSize);
                    } break;
                    case DrawPrimitive_Line:{
                        glLineWidth(Mesh->LineWidth);
                    } break;
                    case DrawPrimitive_Triangle:{
                        // NOTE(furkan) : Do nothing
                    } break;
                    UnexpectedDefaultCase;
                }
                
                if(Drawable->IndexBuffer){
                    glDrawElements(Drawable->DrawMode, 
                                   Drawable->DrawElementCount,
                                   GL_UNSIGNED_INT, 
                                   (void *)Drawable->DrawOffset);
                } else {
                    glDrawArrays(Drawable->DrawMode, 
                                 Drawable->DrawOffset, 
                                 Drawable->DrawElementCount);
                }
    
                if(Renderable->RenderMeshAABB){
                    glDrawArrays(GL_LINE_STRIP, 
                                 Mesh->VertexCount,
                                 10);
                    glDrawArrays(GL_LINES, 
                                 Mesh->VertexCount+10,
                                 6);
                }
                
                if(Mesh->CollidingFaceIdx != u32_Max){
                    
                    Assert(Drawable->IndexBuffer);
                    
                    static f32 Acc = 0.0f;
                    f32 Blip = (Sin(Acc)+1.0f)*0.5f;
                    Acc += 0.01f;
                    
                    opengl_shader_standard_uniform_layout *UniLt;
                    UniLt = &StandardShader.UniformLayout;
                    glUniform4f(UniLt->LocAmbientLight, 
                                Blip, Blip, Blip, 1.0f);
                    
                    GLint PolyMode = u32_Max;
                    glGetIntegerv(GL_POLYGON_MODE, &PolyMode);
                    
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glDrawElements(Drawable->DrawMode, 
                                   3,
                                   GL_UNSIGNED_INT, 
                                   (void *)(sizeof(u32)*3*Mesh->CollidingFaceIdx));
                    
                    glPolygonMode(GL_FRONT_AND_BACK, PolyMode);
                }
    
                OpenGLAssertNoError();
            } else {
                Warning("OpenGLGetDrawable returned null");
            }
        }
    }
    
    b32 
    renderer_opengl::RenderScene(asset_manager *AssetManager,
                                 scene *Scene, camera *Camera, 
                                 window *Window, 
                                 renderer_polygon_mode PolygonMode){
        b32 Success = true;
    
        switch(PolygonMode){
            case RendererPolygonMode_Point:{
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            } break;
            case RendererPolygonMode_Wireframe:{
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } break;
            case RendererPolygonMode_Fill:{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } break;
            UnexpectedDefaultCase;
        }
        
        if(Scene){
            // NOTE(furkan):
            //  X+: Right
            //  Y+: Up
            //  Z+: Out
    
            vec2 ViewportSize = Vec2((f32)Window->ClientSize.x, 
                                    (f32)Window->ClientSize.y);
    
            mat4 ViewMatrix = Camera->GetViewMatrix();
            mat4 ProjMatrix = Camera->GetProjectionMatrix(ViewportSize);
            mat4 ViewMatrixTran = Mat4Transpose(ViewMatrix);
            mat4 ProjMatrixTran = Mat4Transpose(ProjMatrix);
    
            OpenGLAssertNoError();
            
            glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
            // glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            // glClearColor(0.043137f, 0.047059f, 0.058824f, 1.0f);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
            glUseProgram(StandardShader.Common.Program);
    
            OpenGLAssertNoError();
    
            for(u32 EntityIndex=0; 
                    EntityIndex < Scene->EntityCount; 
                    EntityIndex++){
                entity *Entity = Scene->Entities + EntityIndex;
                if(Entity->IsActive){
                    component_renderable *Renderable = Entity->Renderable;
                    RenderRenderable(Renderable, &Entity->Transform, 
                                     &ViewMatrixTran, &ProjMatrixTran);
                }
            }
            
            OpenGLAssertNoError();
    
            if(Scene->ShowGizmo && 
               Scene->GizmoRenderable.Mesh && 
               Scene->GizmoRenderable.Mesh->ID){

                f32 Beta  = -DegreeToRadian(Camera->Transform.EulerRotation.x);
                f32 Alpha = -DegreeToRadian(Camera->Transform.EulerRotation.y);
                f32 GizmoCamPosX = Cos(Beta)*Sin(Alpha);
                f32 GizmoCamPosY = Sin(Beta);
                f32 GizmoCamPosZ = Cos(Beta)*Cos(Alpha);
        
                camera GizmoCamera(45.0f, 0.1f, 1000.0f, Camera->ProjectionType, 
                                Scene->GizmoSphereRadius * Vec3(GizmoCamPosX, 
                                                                GizmoCamPosY, 
                                                                GizmoCamPosZ),
                                Camera->Transform.EulerRotation);
        
                mat4 GizmoViewMatrix = GizmoCamera.GetViewMatrix();
                mat4 GizmoViewMatrixTran = Mat4Transpose(GizmoViewMatrix);
        
                vec2 GizmoViewportSize = Vec2((f32)Scene->GizmoViewportSize.x, 
                                            (f32)Scene->GizmoViewportSize.y);
        
                mat4 GizmoProjMatrix = GizmoCamera.GetProjectionMatrix(GizmoViewportSize);
                mat4 GizmoProjMatrixTran = Mat4Transpose(GizmoProjMatrix);
        
                glViewport(Scene->GizmoViewportOffset.x, Scene->GizmoViewportOffset.y,
                           Scene->GizmoViewportSize.x, Scene->GizmoViewportSize.y);
        
                glClear(GL_DEPTH_BUFFER_BIT);
        
                RenderRenderable(&Scene->GizmoRenderable, &Scene->GizmoTransform, 
                                &GizmoViewMatrixTran, &GizmoProjMatrixTran);
        
                glViewport(0, 0, Window->ClientSize.x, Window->ClientSize.y);
            }
        } else {
            Warning("Did you just pass a null scene to renderer?");
        }
    
        OpenGLAssertNoError();
    
        return Success;
    }
        
    void 
    renderer_opengl::LoadFunctions() {
        PlatformOpenGLFuncLoad(glBlendEquationSeparate);
        PlatformOpenGLFuncLoad(glDrawBuffers);
        PlatformOpenGLFuncLoad(glStencilOpSeparate);
        PlatformOpenGLFuncLoad(glStencilFuncSeparate);
        PlatformOpenGLFuncLoad(glStencilMaskSeparate);
        
        PlatformOpenGLFuncLoad(glAttachShader);
        PlatformOpenGLFuncLoad(glBindAttribLocation);
        PlatformOpenGLFuncLoad(glCompileShader);
        PlatformOpenGLFuncLoad(glCreateProgram);
        PlatformOpenGLFuncLoad(glCreateShader);
        PlatformOpenGLFuncLoad(glDeleteProgram);
        PlatformOpenGLFuncLoad(glDeleteShader);
        PlatformOpenGLFuncLoad(glDetachShader);
        
        PlatformOpenGLFuncLoad(glGetProgramiv);
        PlatformOpenGLFuncLoad(glGetProgramInfoLog);
        PlatformOpenGLFuncLoad(glGetShaderiv);
        PlatformOpenGLFuncLoad(glGetShaderInfoLog);
        PlatformOpenGLFuncLoad(glGetShaderSource);
        PlatformOpenGLFuncLoad(glLinkProgram);
        PlatformOpenGLFuncLoad(glShaderSource);
        PlatformOpenGLFuncLoad(glUseProgram);
        PlatformOpenGLFuncLoad(glValidateProgram);
        
        PlatformOpenGLFuncLoad(glGetActiveAttrib);
        PlatformOpenGLFuncLoad(glGetActiveUniform);
        PlatformOpenGLFuncLoad(glGetAttachedShaders);
        PlatformOpenGLFuncLoad(glGetUniformfv);
        PlatformOpenGLFuncLoad(glGetUniformiv);
        PlatformOpenGLFuncLoad(glGetAttribLocation);
        PlatformOpenGLFuncLoad(glGetUniformLocation);
        PlatformOpenGLFuncLoad(glIsProgram);
        PlatformOpenGLFuncLoad(glIsShader);
        
        PlatformOpenGLFuncLoad(glDisableVertexAttribArray);
        PlatformOpenGLFuncLoad(glEnableVertexAttribArray);
        
        PlatformOpenGLFuncLoad(glGetVertexAttribdv);
        PlatformOpenGLFuncLoad(glGetVertexAttribfv);
        PlatformOpenGLFuncLoad(glGetVertexAttribiv);
        PlatformOpenGLFuncLoad(glGetVertexAttribPointerv);
        
        PlatformOpenGLFuncLoad(glUniform1f);
        PlatformOpenGLFuncLoad(glUniform2f);
        PlatformOpenGLFuncLoad(glUniform3f);
        PlatformOpenGLFuncLoad(glUniform4f);
        PlatformOpenGLFuncLoad(glUniform1i);
        PlatformOpenGLFuncLoad(glUniform2i);
        PlatformOpenGLFuncLoad(glUniform3i);
        PlatformOpenGLFuncLoad(glUniform4i);
        
        PlatformOpenGLFuncLoad(glUniform1fv);
        PlatformOpenGLFuncLoad(glUniform2fv);
        PlatformOpenGLFuncLoad(glUniform3fv);
        PlatformOpenGLFuncLoad(glUniform4fv);
        PlatformOpenGLFuncLoad(glUniform1iv);
        PlatformOpenGLFuncLoad(glUniform2iv);
        PlatformOpenGLFuncLoad(glUniform3iv);
        PlatformOpenGLFuncLoad(glUniform4iv);
        
        PlatformOpenGLFuncLoad(glUniformMatrix2fv);
        PlatformOpenGLFuncLoad(glUniformMatrix3fv);
        PlatformOpenGLFuncLoad(glUniformMatrix4fv);
        
        PlatformOpenGLFuncLoad(glVertexAttribPointer);
        
        PlatformOpenGLFuncLoad(glVertexAttrib1d);
        PlatformOpenGLFuncLoad(glVertexAttrib1f);
        PlatformOpenGLFuncLoad(glVertexAttrib1s);
        PlatformOpenGLFuncLoad(glVertexAttrib2d);
        PlatformOpenGLFuncLoad(glVertexAttrib2f);
        PlatformOpenGLFuncLoad(glVertexAttrib2s);
        PlatformOpenGLFuncLoad(glVertexAttrib3d);
        PlatformOpenGLFuncLoad(glVertexAttrib3f);
        PlatformOpenGLFuncLoad(glVertexAttrib3s);
        PlatformOpenGLFuncLoad(glVertexAttrib4Nub);
        PlatformOpenGLFuncLoad(glVertexAttrib4d);
        PlatformOpenGLFuncLoad(glVertexAttrib4f);
        PlatformOpenGLFuncLoad(glVertexAttrib4s);
        PlatformOpenGLFuncLoad(glVertexAttrib1dv);
        PlatformOpenGLFuncLoad(glVertexAttrib1fv);
        PlatformOpenGLFuncLoad(glVertexAttrib1sv);
        PlatformOpenGLFuncLoad(glVertexAttrib2dv);
        PlatformOpenGLFuncLoad(glVertexAttrib2fv);
        PlatformOpenGLFuncLoad(glVertexAttrib2sv);
        PlatformOpenGLFuncLoad(glVertexAttrib3dv);
        PlatformOpenGLFuncLoad(glVertexAttrib3fv);
        PlatformOpenGLFuncLoad(glVertexAttrib3sv);
        PlatformOpenGLFuncLoad(glVertexAttrib4Nbv);
        PlatformOpenGLFuncLoad(glVertexAttrib4Niv);
        PlatformOpenGLFuncLoad(glVertexAttrib4Nsv);
        PlatformOpenGLFuncLoad(glVertexAttrib4Nubv);
        PlatformOpenGLFuncLoad(glVertexAttrib4Nuiv);
        PlatformOpenGLFuncLoad(glVertexAttrib4Nusv);
        PlatformOpenGLFuncLoad(glVertexAttrib4bv);
        PlatformOpenGLFuncLoad(glVertexAttrib4dv);
        PlatformOpenGLFuncLoad(glVertexAttrib4fv);
        PlatformOpenGLFuncLoad(glVertexAttrib4iv);
        PlatformOpenGLFuncLoad(glVertexAttrib4sv);
        PlatformOpenGLFuncLoad(glVertexAttrib4ubv);
        PlatformOpenGLFuncLoad(glVertexAttrib4uiv);
        PlatformOpenGLFuncLoad(glVertexAttrib4usv);
        
        PlatformOpenGLFuncLoad(glBindVertexArray);
        PlatformOpenGLFuncLoad(glDeleteVertexArrays);
        PlatformOpenGLFuncLoad(glGenVertexArrays);
        PlatformOpenGLFuncLoad(glIsVertexArray);
        
        PlatformOpenGLFuncLoad(glGenQueries);
        PlatformOpenGLFuncLoad(glDeleteQueries);
        PlatformOpenGLFuncLoad(glIsQuery);
        PlatformOpenGLFuncLoad(glBeginQuery);
        PlatformOpenGLFuncLoad(glEndQuery);
        PlatformOpenGLFuncLoad(glGetQueryiv);
        PlatformOpenGLFuncLoad(glGetQueryObjectiv);
        PlatformOpenGLFuncLoad(glGetQueryObjectuiv);
        
        PlatformOpenGLFuncLoad(glBindBuffer);
        PlatformOpenGLFuncLoad(glDeleteBuffers);
        PlatformOpenGLFuncLoad(glGenBuffers);
        PlatformOpenGLFuncLoad(glBufferData);
        PlatformOpenGLFuncLoad(glBufferSubData);
        PlatformOpenGLFuncLoad(glGetBufferSubData);
        PlatformOpenGLFuncLoad(glGetBufferParameteriv);
        PlatformOpenGLFuncLoad(glGetBufferPointerv);
        PlatformOpenGLFuncLoad(glMapBuffer);
        PlatformOpenGLFuncLoad(glIsBuffer);
        PlatformOpenGLFuncLoad(glUnmapBuffer);
    
        PlatformOpenGLFuncLoad(glActiveTexture);
        PlatformOpenGLFuncLoad(glGenerateMipmap);
    
        PlatformOpenGLFuncLoad(glBlendEquation);
        PlatformOpenGLFuncLoad(glBlendFuncSeparate);
    }
    
    b32
    renderer_opengl::OnWindowResize(window *Window){
        b32 Success = true;
    
        glViewport(0, 0, Window->ClientSize.x, Window->ClientSize.y);
    
        return Success;
    }
    
    b32
    renderer_opengl::OnInitialise(window *Window){
        b32 Success = true;
    
        glViewport(0, 0, Window->ClientSize.x, Window->ClientSize.y);
        glEnable(GL_DEPTH_TEST);
    
        // TODO(furkan): Treat translucent objects in a special way
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_MULTISAMPLE_ARB);
        
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    
        if(!CompileStandardShader(&StandardShader)){
            Warning("Standard shader could not be compiled");
        }
        // TODO(furkan): Custom shaders which allow you to create shaders
        // without hardcoding variable names in the compilation phase.
        // Basically, custom shaders will parse the code and extract variable
        // locations and store them in a dictionary as <var_name, location> pairs.
        // Also, materials should be created in this phase.
    
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
        // NOTE(furkan): MSAA Information
        // GLboolean Multisample = glIsEnabled(GL_MULTISAMPLE_ARB);
        // GLint     SampleBuffers = 0x7FFFFFFF;
        // GLint     Samples = 0x7FFFFFFF;
        // 
        // GLboolean SampleAlphaToCoverage = glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
        // GLboolean SampleAlphaToOne = glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE_ARB);
        // 
        // GLboolean SampleCoverage = glIsEnabled(GL_SAMPLE_COVERAGE_ARB);
        // GLboolean SampleCoverageInvert = false;
        // GLfloat   SampleCoverageValue = 596.0f;
        // 
        // glGetFloatv(GL_SAMPLE_COVERAGE_VALUE_ARB, &SampleCoverageValue);
        // glGetBooleanv(GL_SAMPLE_COVERAGE_INVERT_ARB, &SampleCoverageInvert);
        // glGetIntegerv(GL_SAMPLE_BUFFERS_ARB, &SampleBuffers);
        // glGetIntegerv(GL_SAMPLES_ARB, &Samples);
        // 
        // Log("Multisample           :%s", Multisample?"true":"false");
        // Log("SampleBuffers         :%d", SampleBuffers);
        // Log("Samples               :%d", Samples);
        // Log("SampleAlphaToCoverage :%s", SampleAlphaToCoverage?"true":"false");
        // Log("SampleAlphaToOne      :%s", SampleAlphaToOne?"true":"false");
        // Log("SampleCoverage        :%s", SampleCoverage?"true":"false");
        // Log("SampleCoverageInvert  :%s", SampleCoverageInvert?"true":"false");
        // Log("SampleCoverageValue   :%f", SampleCoverageValue);
    
        return Success;
    }
    
    b32 
    renderer_opengl::Initialise(window *Window){
        b32 Success = true;
        return Success;
    }
    
    b32 
    renderer_opengl::Finalise(){
        b32 Success = true;
        return Success;
    }
}
