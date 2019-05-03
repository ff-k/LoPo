kadabra::vec4u
kadabra::Vec4u(u32 x, u32 y, u32 z, u32 w){
    kadabra::vec4u Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4u 
kadabra::Vec4u(kadabra::vec2u xy, u32 z, u32 w){
    kadabra::vec4u Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4u 
kadabra::Vec4u(u32 x, kadabra::vec2u yz, u32 w){
    kadabra::vec4u Result;
    
    Result.x = x;
    Result.y = yz.x;
    Result.z = yz.y;
    Result.w = w;
    
    return Result;
}

kadabra::vec4u 
kadabra::Vec4u(u32 x, u32 y, kadabra::vec2u zw){
    kadabra::vec4u Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = zw.x;
    Result.w = zw.y;
    
    return Result;
}

kadabra::vec4u 
kadabra::Vec4u(kadabra::vec2u xy, kadabra::vec2u zw){
    kadabra::vec4u Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = zw.x;
    Result.w = zw.y;
    
    return Result;
}

kadabra::vec4u 
kadabra::Vec4u(kadabra::vec3u xyz, u32 w){
    kadabra::vec4u Result;
    
    Result.x = xyz.x;
    Result.y = xyz.y;
    Result.z = xyz.z;
    Result.w = w;
    
    return Result;
}

kadabra::vec4u 
kadabra::Vec4u(u32 x, kadabra::vec3u yzw){
    kadabra::vec4u Result;
    
    Result.x = x;
    Result.y = yzw.x;
    Result.z = yzw.y;
    Result.w = yzw.z;
    
    return Result;
}

kadabra::vec4u 
kadabra::operator+(kadabra::vec4u A, kadabra::vec4u B) {
    kadabra::vec4u Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return Result;
}

kadabra::vec4u 
kadabra::operator+(kadabra::vec4u A, u32 B) {
    kadabra::vec4u Result;

    Result.x = A.x + B;
    Result.y = A.y + B;
    Result.z = A.z + B;

    return Result;
}

kadabra::vec4u 
kadabra::operator+(u32 A, kadabra::vec4u B) {
    kadabra::vec4u Result;

    Result.x = A + B.x;
    Result.y = A + B.y;
    Result.z = A + B.z;

    return Result;
}

kadabra::vec4u
kadabra::operator+=(kadabra::vec4u& A, kadabra::vec4u B) {
    kadabra::vec4u Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec4u
kadabra::operator+=(kadabra::vec4u& A, u32 B) {
    kadabra::vec4u Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec4u 
kadabra::operator-(kadabra::vec4u A, kadabra::vec4u B) {
    kadabra::vec4u Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    
    return Result;
}

kadabra::vec4u 
kadabra::operator-(kadabra::vec4u A, u32 B) {
    kadabra::vec4u Result;

    Result.x = A.x - B;
    Result.y = A.y - B;
    Result.z = A.z - B;

    return Result;
}

kadabra::vec4u 
kadabra::operator-(u32 A, kadabra::vec4u B) {
    kadabra::vec4u Result;

    Result.x = A - B.x;
    Result.y = A - B.y;
    Result.z = A - B.z;

    return Result;
}

kadabra::vec4u
kadabra::operator-=(kadabra::vec4u& A, kadabra::vec4u B) {
    kadabra::vec4u Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec4u
kadabra::operator-=(kadabra::vec4u& A, u32 B) {
    kadabra::vec4u Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec4u
kadabra::operator*(u32 A, kadabra::vec4u B) {
    kadabra::vec4u Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;

    return Result;
}

kadabra::vec4u
kadabra::operator*(kadabra::vec4u A, u32 B) {
    kadabra::vec4u Result;

    Result.x = A.x * B;
    Result.y = A.y * B;
    Result.z = A.z * B;

    return Result;
}

kadabra::vec4u
kadabra::operator*=(kadabra::vec4u& A, u32 B) {
    kadabra::vec4u Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec4u
kadabra::Hadamard(kadabra::vec4u A, kadabra::vec4u B){
    kadabra::vec4u Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    Result.z = A.z * B.z;
    Result.w = A.w * B.w;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec4u V){
    f32 Result;

Result = Sqrt((f32)(V.x*V.x + V.y*V.y + V.z*V.z + V.w*V.w));

    return Result;
}

kadabra::vec4
kadabra::Normalize(kadabra::vec4u V){
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

u32
kadabra::Dot(kadabra::vec4u A, kadabra::vec4u B){
    u32 Result;

    Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

    return Result;
}