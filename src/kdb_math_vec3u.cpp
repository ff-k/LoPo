kadabra::vec3u
kadabra::Vec3u(u32 x, u32 y, u32 z){
    kadabra::vec3u Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = z;
    
    return Result;
}

kadabra::vec3u
kadabra::Vec3u(kadabra::vec2u xy, u32 z){
    kadabra::vec3u Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = z;
    
    return Result;
}

kadabra::vec3u
kadabra::Vec3u(u32 x, kadabra::vec2u yz){
    kadabra::vec3u Result;
    
    Result.x = x;
    Result.y = yz.x;
    Result.z = yz.y;
    
    return Result;
}

kadabra::vec3u 
kadabra::operator+(kadabra::vec3u A, kadabra::vec3u B) {
    kadabra::vec3u Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return Result;
}

kadabra::vec3u 
kadabra::operator+(kadabra::vec3u A, u32 B) {
    kadabra::vec3u Result;

    Result.x = A.x + B;
    Result.y = A.y + B;
    Result.z = A.z + B;

    return Result;
}

kadabra::vec3u 
kadabra::operator+(u32 A, kadabra::vec3u B) {
    kadabra::vec3u Result;

    Result.x = A + B.x;
    Result.y = A + B.y;
    Result.z = A + B.z;

    return Result;
}

kadabra::vec3u
kadabra::operator+=(kadabra::vec3u& A, kadabra::vec3u B) {
    kadabra::vec3u Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec3u
kadabra::operator+=(kadabra::vec3u& A, u32 B) {
    kadabra::vec3u Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec3u 
kadabra::operator-(kadabra::vec3u A, kadabra::vec3u B) {
    kadabra::vec3u Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    
    return Result;
}

kadabra::vec3u 
kadabra::operator-(kadabra::vec3u A, u32 B) {
    kadabra::vec3u Result;

    Result.x = A.x - B;
    Result.y = A.y - B;
    Result.z = A.z - B;

    return Result;
}

kadabra::vec3u 
kadabra::operator-(u32 A, kadabra::vec3u B) {
    kadabra::vec3u Result;

    Result.x = A - B.x;
    Result.y = A - B.y;
    Result.z = A - B.z;

    return Result;
}

kadabra::vec3u
kadabra::operator-=(kadabra::vec3u& A, kadabra::vec3u B) {
    kadabra::vec3u Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec3u
kadabra::operator-=(kadabra::vec3u& A, u32 B) {
    kadabra::vec3u Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec3u
kadabra::operator*(u32 A, kadabra::vec3u B) {
    kadabra::vec3u Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;

    return Result;
}

kadabra::vec3u
kadabra::operator*(kadabra::vec3u A, u32 B) {
    kadabra::vec3u Result;

    Result.x = A.x * B;
    Result.y = A.y * B;
    Result.z = A.z * B;

    return Result;
}

kadabra::vec3u
kadabra::operator*=(kadabra::vec3u& A, u32 B) {
    kadabra::vec3u Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec3u
kadabra::Hadamard(kadabra::vec3u A, kadabra::vec3u B){
    kadabra::vec3u Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    Result.z = A.z * B.z;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec3u V){
    f32 Result;

    Result = Sqrt((f32)(V.x*V.x + V.y*V.y + V.z*V.z));

    return Result;
}

kadabra::vec3
kadabra::Normalize(kadabra::vec3u V){
    kadabra::vec3 Result;

    f32 L = Length(V);

    if(L != 0.0f){
        Result.x = V.x/L;
        Result.y = V.y/L;
        Result.z = V.z/L;
    } else {
        Warning("Normalize() is called for a null kadabra::vector");
        Result = kadabra::Vec3(0.0f, 0.0f, 0.0f);
    }

    return Result;
}

u32
kadabra::Dot(kadabra::vec3u A, kadabra::vec3u B){
    u32 Result;

    Result = A.x*B.x + A.y*B.y + A.z*B.z;

    return Result;
}

kadabra::vec3u
kadabra::Cross(kadabra::vec3u A, kadabra::vec3u B){
    kadabra::vec3u Result;

    Result = kadabra::Vec3u(A.y*B.z - A.z*B.y,
                   A.z*B.x - A.x*B.z,
                   A.x*B.y - A.y*B.x);

    return Result;
}