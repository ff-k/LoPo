kadabra::vec4i
kadabra::Vec4i(s32 x, s32 y, s32 z, s32 w){
    kadabra::vec4i Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4i 
kadabra::Vec4i(kadabra::vec2i xy, s32 z, s32 w){
    kadabra::vec4i Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4i 
kadabra::Vec4i(s32 x, kadabra::vec2i yz, s32 w){
    kadabra::vec4i Result;
    
    Result.x = x;
    Result.y = yz.x;
    Result.z = yz.y;
    Result.w = w;
    
    return Result;
}

kadabra::vec4i 
kadabra::Vec4i(s32 x, s32 y, kadabra::vec2i zw){
    kadabra::vec4i Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = zw.x;
    Result.w = zw.y;
    
    return Result;
}

kadabra::vec4i 
kadabra::Vec4i(kadabra::vec2i xy, kadabra::vec2i zw){
    kadabra::vec4i Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = zw.x;
    Result.w = zw.y;
    
    return Result;
}

kadabra::vec4i 
kadabra::Vec4i(kadabra::vec3i xyz, s32 w){
    kadabra::vec4i Result;
    
    Result.x = xyz.x;
    Result.y = xyz.y;
    Result.z = xyz.z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4i 
kadabra::Vec4i(s32 x, kadabra::vec3i yzw){
    kadabra::vec4i Result;
    
    Result.x = x;
    Result.y = yzw.x;
    Result.z = yzw.y;
    Result.w = yzw.z;
    
    return Result;
}

kadabra::vec4i 
kadabra::operator+(kadabra::vec4i A, kadabra::vec4i B) {
    kadabra::vec4i Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return Result;
}

kadabra::vec4i 
kadabra::operator+(kadabra::vec4i A, s32 B) {
    kadabra::vec4i Result;

    Result.x = A.x + B;
    Result.y = A.y + B;
    Result.z = A.z + B;

    return Result;
}

kadabra::vec4i 
kadabra::operator+(s32 A, kadabra::vec4i B) {
    kadabra::vec4i Result;

    Result.x = A + B.x;
    Result.y = A + B.y;
    Result.z = A + B.z;

    return Result;
}

kadabra::vec4i
kadabra::operator+=(kadabra::vec4i& A, kadabra::vec4i B) {
    kadabra::vec4i Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec4i
kadabra::operator+=(kadabra::vec4i& A, s32 B) {
    kadabra::vec4i Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec4i 
kadabra::operator-(kadabra::vec4i A, kadabra::vec4i B) {
    kadabra::vec4i Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    
    return Result;
}

kadabra::vec4i 
kadabra::operator-(kadabra::vec4i A, s32 B) {
    kadabra::vec4i Result;

    Result.x = A.x - B;
    Result.y = A.y - B;
    Result.z = A.z - B;

    return Result;
}

kadabra::vec4i 
kadabra::operator-(s32 A, kadabra::vec4i B) {
    kadabra::vec4i Result;

    Result.x = A - B.x;
    Result.y = A - B.y;
    Result.z = A - B.z;

    return Result;
}

kadabra::vec4i
kadabra::operator-=(kadabra::vec4i& A, kadabra::vec4i B) {
    kadabra::vec4i Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec4i
kadabra::operator-=(kadabra::vec4i& A, s32 B) {
    kadabra::vec4i Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec4i
kadabra::operator*(s32 A, kadabra::vec4i B) {
    kadabra::vec4i Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;

    return Result;
}

kadabra::vec4i
kadabra::operator*(kadabra::vec4i A, s32 B) {
    kadabra::vec4i Result;

    Result.x = A.x * B;
    Result.y = A.y * B;
    Result.z = A.z * B;

    return Result;
}

kadabra::vec4i
kadabra::operator*=(kadabra::vec4i& A, s32 B) {
    kadabra::vec4i Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec4i
kadabra::operator-(kadabra::vec4i &v1){
    return kadabra::Vec4i(-v1.x, -v1.y, -v1.z, -v1.w);
}

kadabra::vec4i
kadabra::Hadamard(kadabra::vec4i A, kadabra::vec4i B){
    kadabra::vec4i Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    Result.z = A.z * B.z;
    Result.w = A.w * B.w;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec4i V){
    f32 Result;

    Result = Sqrt((f32)(V.x*V.x + V.y*V.y + V.z*V.z + V.w*V.w));

    return Result;
}

kadabra::vec4
kadabra::Normalize(kadabra::vec4i V){
    kadabra::vec4 Result;

    f32 L = Length(V);

    if(L != 0.0f){
        Result.x = V.x/L;
        Result.y = V.y/L;
        Result.z = V.z/L;
        Result.w = V.w/L;
    } else {
        Warning("Normalize() is called for a null kadabra::vector");
        Result = kadabra::Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    return Result;
}

s32
kadabra::Dot(kadabra::vec4i A, kadabra::vec4i B){
    s32 Result;

    Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

    return Result;
}