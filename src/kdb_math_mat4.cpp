kadabra::mat4
kadabra::operator*(kadabra::mat4 A, kadabra::mat4 B) {
    kadabra::mat4 Result;

    for(u32 i=0; i<4; i++){
        for(u32 j=0; j<4; j++){
            f32 Val = 0.0f;
            
            Val += A.E[i][0] * B.E[0][j];
            Val += A.E[i][1] * B.E[1][j];
            Val += A.E[i][2] * B.E[2][j];
            Val += A.E[i][3] * B.E[3][j];
            
            Result.E[i][j] = Val;
        }
    }

    return Result;
}

kadabra::vec4 
kadabra::operator*(kadabra::mat4 A, kadabra::vec4 B){
    kadabra::vec4 Result;

    Result.x = A.E[0][0]*B.x + 
               A.E[0][1]*B.y + 
               A.E[0][2]*B.z + 
               A.E[0][3]*B.w;

    Result.y = A.E[1][0]*B.x + 
               A.E[1][1]*B.y + 
               A.E[1][2]*B.z + 
               A.E[1][3]*B.w;

    Result.z = A.E[2][0]*B.x + 
               A.E[2][1]*B.y + 
               A.E[2][2]*B.z + 
               A.E[2][3]*B.w;

    Result.w = A.E[3][0]*B.x + 
               A.E[3][1]*B.y + 
               A.E[3][2]*B.z + 
               A.E[3][3]*B.w;

    return Result;
}

kadabra::mat4
kadabra::operator-(kadabra::mat4 &M){
    mat4 Result = M;
    
    for(u32 Idx=0; Idx<16; Idx++){
        Result.Ptr[Idx] = -Result.Ptr[Idx];
    }
    
    return Result;
}

kadabra::mat4 
kadabra::Mat4Identity(){
    kadabra::mat4 Result;
    ZeroStruct(&Result);

    Result.E[0][0] = 1.0f;
    Result.E[1][1] = 1.0f;
    Result.E[2][2] = 1.0f;
    Result.E[3][3] = 1.0f;

    return Result;
}

b32
kadabra::Mat4Inverse(kadabra::mat4 M, kadabra::mat4 *Inverse){
    // NOTE(furkan): Adapted from the code written for
    // "The Mesa 3D Graphics Library"
    b32 Result = false;

    kadabra::mat4 Inv;

    Inv.Ptr[0] =   M.Ptr[5]  * M.Ptr[10] * M.Ptr[15] - 
                   M.Ptr[5]  * M.Ptr[11] * M.Ptr[14] - 
                   M.Ptr[9]  * M.Ptr[6]  * M.Ptr[15] + 
                   M.Ptr[9]  * M.Ptr[7]  * M.Ptr[14] +
                   M.Ptr[13] * M.Ptr[6]  * M.Ptr[11] - 
                   M.Ptr[13] * M.Ptr[7]  * M.Ptr[10];

    Inv.Ptr[4] = - M.Ptr[4]  * M.Ptr[10] * M.Ptr[15] + 
                   M.Ptr[4]  * M.Ptr[11] * M.Ptr[14] + 
                   M.Ptr[8]  * M.Ptr[6]  * M.Ptr[15] - 
                   M.Ptr[8]  * M.Ptr[7]  * M.Ptr[14] - 
                   M.Ptr[12] * M.Ptr[6]  * M.Ptr[11] + 
                   M.Ptr[12] * M.Ptr[7]  * M.Ptr[10];

    Inv.Ptr[8] =   M.Ptr[4]  * M.Ptr[9]  * M.Ptr[15] - 
                   M.Ptr[4]  * M.Ptr[11] * M.Ptr[13] - 
                   M.Ptr[8]  * M.Ptr[5]  * M.Ptr[15] + 
                   M.Ptr[8]  * M.Ptr[7]  * M.Ptr[13] + 
                   M.Ptr[12] * M.Ptr[5]  * M.Ptr[11] - 
                   M.Ptr[12] * M.Ptr[7]  * M.Ptr[9];

    Inv.Ptr[12] = -M.Ptr[4]  * M.Ptr[9]  * M.Ptr[14] + 
                   M.Ptr[4]  * M.Ptr[10] * M.Ptr[13] +
                   M.Ptr[8]  * M.Ptr[5]  * M.Ptr[14] - 
                   M.Ptr[8]  * M.Ptr[6]  * M.Ptr[13] - 
                   M.Ptr[12] * M.Ptr[5]  * M.Ptr[10] + 
                   M.Ptr[12] * M.Ptr[6]  * M.Ptr[9];

    Inv.Ptr[1] =  -M.Ptr[1]  * M.Ptr[10] * M.Ptr[15] + 
                   M.Ptr[1]  * M.Ptr[11] * M.Ptr[14] + 
                   M.Ptr[9]  * M.Ptr[2]  * M.Ptr[15] - 
                   M.Ptr[9]  * M.Ptr[3]  * M.Ptr[14] - 
                   M.Ptr[13] * M.Ptr[2]  * M.Ptr[11] + 
                   M.Ptr[13] * M.Ptr[3]  * M.Ptr[10];

    Inv.Ptr[5] =   M.Ptr[0]  * M.Ptr[10] * M.Ptr[15] - 
                   M.Ptr[0]  * M.Ptr[11] * M.Ptr[14] - 
                   M.Ptr[8]  * M.Ptr[2]  * M.Ptr[15] + 
                   M.Ptr[8]  * M.Ptr[3]  * M.Ptr[14] + 
                   M.Ptr[12] * M.Ptr[2]  * M.Ptr[11] - 
                   M.Ptr[12] * M.Ptr[3]  * M.Ptr[10];

    Inv.Ptr[9] =  -M.Ptr[0]  * M.Ptr[9]  * M.Ptr[15] + 
                   M.Ptr[0]  * M.Ptr[11] * M.Ptr[13] + 
                   M.Ptr[8]  * M.Ptr[1]  * M.Ptr[15] - 
                   M.Ptr[8]  * M.Ptr[3]  * M.Ptr[13] - 
                   M.Ptr[12] * M.Ptr[1]  * M.Ptr[11] + 
                   M.Ptr[12] * M.Ptr[3]  * M.Ptr[9];

    Inv.Ptr[13] =  M.Ptr[0]  * M.Ptr[9]  * M.Ptr[14] - 
                   M.Ptr[0]  * M.Ptr[10] * M.Ptr[13] - 
                   M.Ptr[8]  * M.Ptr[1]  * M.Ptr[14] + 
                   M.Ptr[8]  * M.Ptr[2]  * M.Ptr[13] + 
                   M.Ptr[12] * M.Ptr[1]  * M.Ptr[10] - 
                   M.Ptr[12] * M.Ptr[2]  * M.Ptr[9];

    Inv.Ptr[2] =   M.Ptr[1]  * M.Ptr[6] * M.Ptr[15] - 
                   M.Ptr[1]  * M.Ptr[7] * M.Ptr[14] - 
                   M.Ptr[5]  * M.Ptr[2] * M.Ptr[15] + 
                   M.Ptr[5]  * M.Ptr[3] * M.Ptr[14] + 
                   M.Ptr[13] * M.Ptr[2] * M.Ptr[7]  - 
                   M.Ptr[13] * M.Ptr[3] * M.Ptr[6];

    Inv.Ptr[6] =  -M.Ptr[0]  * M.Ptr[6] * M.Ptr[15] + 
                   M.Ptr[0]  * M.Ptr[7] * M.Ptr[14] + 
                   M.Ptr[4]  * M.Ptr[2] * M.Ptr[15] - 
                   M.Ptr[4]  * M.Ptr[3] * M.Ptr[14] - 
                   M.Ptr[12] * M.Ptr[2] * M.Ptr[7]  + 
                   M.Ptr[12] * M.Ptr[3] * M.Ptr[6];

    Inv.Ptr[10] =  M.Ptr[0]  * M.Ptr[5] * M.Ptr[15] - 
                   M.Ptr[0]  * M.Ptr[7] * M.Ptr[13] - 
                   M.Ptr[4]  * M.Ptr[1] * M.Ptr[15] + 
                   M.Ptr[4]  * M.Ptr[3] * M.Ptr[13] + 
                   M.Ptr[12] * M.Ptr[1] * M.Ptr[7]  - 
                   M.Ptr[12] * M.Ptr[3] * M.Ptr[5];

    Inv.Ptr[14] = -M.Ptr[0]  * M.Ptr[5] * M.Ptr[14] + 
                   M.Ptr[0]  * M.Ptr[6] * M.Ptr[13] + 
                   M.Ptr[4]  * M.Ptr[1] * M.Ptr[14] - 
                   M.Ptr[4]  * M.Ptr[2] * M.Ptr[13] - 
                   M.Ptr[12] * M.Ptr[1] * M.Ptr[6]  + 
                   M.Ptr[12] * M.Ptr[2] * M.Ptr[5];

    Inv.Ptr[3] =  -M.Ptr[1] * M.Ptr[6] * M.Ptr[11] + 
                   M.Ptr[1] * M.Ptr[7] * M.Ptr[10] + 
                   M.Ptr[5] * M.Ptr[2] * M.Ptr[11] - 
                   M.Ptr[5] * M.Ptr[3] * M.Ptr[10] - 
                   M.Ptr[9] * M.Ptr[2] * M.Ptr[7]  + 
                   M.Ptr[9] * M.Ptr[3] * M.Ptr[6];

    Inv.Ptr[7] =   M.Ptr[0] * M.Ptr[6] * M.Ptr[11] - 
                   M.Ptr[0] * M.Ptr[7] * M.Ptr[10] - 
                   M.Ptr[4] * M.Ptr[2] * M.Ptr[11] + 
                   M.Ptr[4] * M.Ptr[3] * M.Ptr[10] + 
                   M.Ptr[8] * M.Ptr[2] * M.Ptr[7]  - 
                   M.Ptr[8] * M.Ptr[3] * M.Ptr[6];

    Inv.Ptr[11] = -M.Ptr[0] * M.Ptr[5] * M.Ptr[11] + 
                   M.Ptr[0] * M.Ptr[7] * M.Ptr[9]  + 
                   M.Ptr[4] * M.Ptr[1] * M.Ptr[11] - 
                   M.Ptr[4] * M.Ptr[3] * M.Ptr[9]  - 
                   M.Ptr[8] * M.Ptr[1] * M.Ptr[7]  + 
                   M.Ptr[8] * M.Ptr[3] * M.Ptr[5];

    Inv.Ptr[15] =  M.Ptr[0] * M.Ptr[5] * M.Ptr[10] - 
                   M.Ptr[0] * M.Ptr[6] * M.Ptr[9]  - 
                   M.Ptr[4] * M.Ptr[1] * M.Ptr[10] + 
                   M.Ptr[4] * M.Ptr[2] * M.Ptr[9]  + 
                   M.Ptr[8] * M.Ptr[1] * M.Ptr[6]  - 
                   M.Ptr[8] * M.Ptr[2] * M.Ptr[5];

    f32 Det = M.Ptr[0] * Inv.Ptr[0] + 
              M.Ptr[1] * Inv.Ptr[4] + 
              M.Ptr[2] * Inv.Ptr[8] + 
              M.Ptr[3] * Inv.Ptr[12];

    if(Det != 0.0f){
        Result = true;

        f32 InvDet = 1.0f / Det;

        for(u32 E = 0; E<16; E++){
            Inverse->Ptr[E] = Inv.Ptr[E] * InvDet;
        }
    }
    
    return Result;
}

kadabra::mat4 
kadabra::Mat4Transpose(kadabra::mat4 Mat){
    kadabra::mat4 Result;

    Result.E[0][0] = Mat.E[0][0];
    Result.E[0][1] = Mat.E[1][0];
    Result.E[0][2] = Mat.E[2][0];
    Result.E[0][3] = Mat.E[3][0];

    Result.E[1][0] = Mat.E[0][1];
    Result.E[1][1] = Mat.E[1][1];
    Result.E[1][2] = Mat.E[2][1];
    Result.E[1][3] = Mat.E[3][1];

    Result.E[2][0] = Mat.E[0][2];
    Result.E[2][1] = Mat.E[1][2];
    Result.E[2][2] = Mat.E[2][2];
    Result.E[2][3] = Mat.E[3][2];

    Result.E[3][0] = Mat.E[0][3];
    Result.E[3][1] = Mat.E[1][3];
    Result.E[3][2] = Mat.E[2][3];
    Result.E[3][3] = Mat.E[3][3];

    return Result;
}

kadabra::mat4 
kadabra::Mat4Translate(kadabra::mat4 Mat, kadabra::vec3 Translation){
    kadabra::mat4 Result;

    if(Translation.x == 0.0f &&
       Translation.y == 0.0f &&
       Translation.z == 0.0f){

       Result = Mat;
    } else {

        kadabra::mat4 TranslationMatrix = kadabra::Mat4Identity();
        TranslationMatrix.E[0][3] = Translation.x;
        TranslationMatrix.E[1][3] = Translation.y;
        TranslationMatrix.E[2][3] = Translation.z;

        Result = Mat * TranslationMatrix;
    }

    return Result;
}

kadabra::mat4 
kadabra::Mat4Rotate(kadabra::mat4 Mat, f32 Angle, rotation_axis RotateAxis){
    kadabra::mat4 Result;

    // TODO(furkan) : Quaternions

    if(Angle == 0.0f){

        Result = Mat;
    } else {

        f32 Radian = DegreeToRadian(Angle);

        f32 SinAngle = Sin(Radian);
        f32 CosAngle = Cos(Radian);

        kadabra::mat4 RotationMatrix = kadabra::Mat4Identity();
        
        switch(RotateAxis){
            case RotationAxis_X:{
                RotationMatrix.E[1][1] =  CosAngle;
                RotationMatrix.E[1][2] = -SinAngle;
                RotationMatrix.E[2][1] =  SinAngle;
                RotationMatrix.E[2][2] =  CosAngle;
            } break;
            case RotationAxis_Y:{
                RotationMatrix.E[0][0] =  CosAngle;
                RotationMatrix.E[0][2] =  SinAngle;
                RotationMatrix.E[2][0] = -SinAngle;
                RotationMatrix.E[2][2] =  CosAngle;
            } break;
            case RotationAxis_Z:{
                RotationMatrix.E[0][0] =  CosAngle;
                RotationMatrix.E[0][1] = -SinAngle;
                RotationMatrix.E[1][0] =  SinAngle;
                RotationMatrix.E[1][1] =  CosAngle;
            } break;
            UnexpectedDefaultCase;
        }

        Result = Mat * RotationMatrix;
    }

    return Result;
}

kadabra::mat4 
kadabra::Mat4Scale(kadabra::mat4 Mat, kadabra::vec3 Scale){
    kadabra::mat4 Result;

    if(Scale.x == 1.0f &&
       Scale.y == 1.0f &&
       Scale.z == 1.0f){

       Result = Mat;
    } else {

        kadabra::mat4 ScaleMatrix = kadabra::Mat4Identity();
        ScaleMatrix.E[0][0] = Scale.x;
        ScaleMatrix.E[1][1] = Scale.y;
        ScaleMatrix.E[2][2] = Scale.z;

        Result = Mat * ScaleMatrix;
    }

    return Result;
}

kadabra::mat4 
kadabra::Mat4Orthographic(f32 Left, f32 Bottom, 
                          f32 Right, f32 Top, 
                          f32 NearZ, f32 FarZ){
    kadabra::mat4 Result;
    ZeroStruct(&Result);

    f32 Width  = Right-Left;
    f32 Height =   Top-Bottom;
    f32 Depth  =  FarZ-NearZ;

    Result.E[0][0] =  2.0f/Width;
    Result.E[1][1] =  2.0f/Height;
    Result.E[2][2] = -2.0f/Depth;

    Result.E[0][3] = -(Right+Left  )/Width;
    Result.E[1][3] = -(  Top+Bottom)/Height;
    Result.E[2][3] = -( FarZ+NearZ )/Depth;

    Result.E[3][3] = 1.0f;


    return Result;
}

kadabra::mat4 
kadabra::Mat4Perspective(f32 FOVAngle, f32 Width, f32 Height, f32 NearZ, f32 FarZ){
    kadabra::mat4 Result;
    ZeroStruct(&Result);

    f32 FOVRadian = DegreeToRadian(FOVAngle);
    f32 TanHalfFOV = Tan(FOVRadian / 2.0f);

    f32 Aspect = Width/Height;

    f32 Depth = NearZ - FarZ;

    Result.E[0][0] =           1.0f / (TanHalfFOV * Aspect);
    Result.E[1][1] =           1.0f /  TanHalfFOV;
    Result.E[2][2] = (NearZ + FarZ) / Depth;

    Result.E[2][3] = (2.0f * NearZ * FarZ) / Depth;
    Result.E[3][2] = -1.0f;

    return Result;
}