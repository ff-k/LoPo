kadabra::vec4
kadabra::Vec4(f32 x, f32 y, f32 z, f32 w){
    kadabra::vec4 Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4 
kadabra::Vec4(kadabra::vec2 xy, f32 z, f32 w){
    kadabra::vec4 Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4 
kadabra::Vec4(f32 x, kadabra::vec2 yz, f32 w){
    kadabra::vec4 Result;
    
    Result.x = x;
    Result.y = yz.x;
    Result.z = yz.y;
    Result.w = w;
    
    return Result;
}

kadabra::vec4 
kadabra::Vec4(f32 x, f32 y, kadabra::vec2 zw){
    kadabra::vec4 Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = zw.x;
    Result.w = zw.y;
    
    return Result;
}

kadabra::vec4 
kadabra::Vec4(kadabra::vec2 xy, kadabra::vec2 zw){
    kadabra::vec4 Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = zw.x;
    Result.w = zw.y;
    
    return Result;
}

kadabra::vec4 
kadabra::Vec4(kadabra::vec3 xyz, f32 w){
    kadabra::vec4 Result;
    
    Result.x = xyz.x;
    Result.y = xyz.y;
    Result.z = xyz.z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4 
kadabra::Vec4(f32 x, kadabra::vec3 yzw){
    kadabra::vec4 Result;
    
    Result.x = x;
    Result.y = yzw.x;
    Result.z = yzw.y;
    Result.w = yzw.z;
    
    return Result;
}

kadabra::vec4 
kadabra::operator+(kadabra::vec4 A, kadabra::vec4 B) {
    kadabra::vec4 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return Result;
}

kadabra::vec4 
kadabra::operator+(kadabra::vec4 A, f32 B) {
    kadabra::vec4 Result;

    Result.x = A.x + B;
    Result.y = A.y + B;
    Result.z = A.z + B;

    return Result;
}

kadabra::vec4 
kadabra::operator+(f32 A, kadabra::vec4 B) {
    kadabra::vec4 Result;

    Result.x = A + B.x;
    Result.y = A + B.y;
    Result.z = A + B.z;

    return Result;
}

kadabra::vec4
kadabra::operator+=(kadabra::vec4& A, kadabra::vec4 B) {
    kadabra::vec4 Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec4
kadabra::operator+=(kadabra::vec4& A, f32 B) {
    kadabra::vec4 Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec4 
kadabra::operator-(kadabra::vec4 A, kadabra::vec4 B) {
    kadabra::vec4 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    
    return Result;
}

kadabra::vec4 
kadabra::operator-(kadabra::vec4 A, f32 B) {
    kadabra::vec4 Result;

    Result.x = A.x - B;
    Result.y = A.y - B;
    Result.z = A.z - B;

    return Result;
}

kadabra::vec4 
kadabra::operator-(f32 A, kadabra::vec4 B) {
    kadabra::vec4 Result;

    Result.x = A - B.x;
    Result.y = A - B.y;
    Result.z = A - B.z;

    return Result;
}

kadabra::vec4
kadabra::operator-=(kadabra::vec4& A, kadabra::vec4 B) {
    kadabra::vec4 Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec4
kadabra::operator-=(kadabra::vec4& A, f32 B) {
    kadabra::vec4 Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec4
kadabra::operator*(f32 A, kadabra::vec4 B) {
    kadabra::vec4 Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;

    return Result;
}

kadabra::vec4
kadabra::operator*(kadabra::vec4 A, f32 B) {
    kadabra::vec4 Result;

    Result.x = A.x * B;
    Result.y = A.y * B;
    Result.z = A.z * B;

    return Result;
}

kadabra::vec4
kadabra::operator*=(kadabra::vec4& A, f32 B) {
    kadabra::vec4 Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec4
kadabra::operator-(kadabra::vec4 &v1){
    return kadabra::Vec4(-v1.x, -v1.y, -v1.z, -v1.w);
}

kadabra::vec4
kadabra::Hadamard(kadabra::vec4 A, kadabra::vec4 B){
    kadabra::vec4 Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    Result.z = A.z * B.z;
    Result.w = A.w * B.w;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec4 V){
    f32 Result;

    Result = Sqrt(
                  V.x*V.x + 
                  V.y*V.y + 
                  V.z*V.z + 
                  V.w*V.w
                  );

    return Result;
}

kadabra::vec4
kadabra::Normalize(kadabra::vec4 V){
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

f32
kadabra::Dot(kadabra::vec4 A, kadabra::vec4 B){
    f32 Result;

    Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

    return Result;
}