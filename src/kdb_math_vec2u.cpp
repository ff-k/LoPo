kadabra::vec2u
kadabra::Vec2u(u32 x, u32 y){
    kadabra::vec2u Result;
    
    Result.x = x;
    Result.y = y;
    
    return Result;
}

kadabra::vec2u 
kadabra::operator+(kadabra::vec2u A, kadabra::vec2u B) {
    kadabra::vec2u Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return Result;
}

kadabra::vec2u 
kadabra::operator+(kadabra::vec2u A, u32 B) {
    kadabra::vec2u Result;

    Result.x = A.x + B;
    Result.y = A.y + B;

    return Result;
}

kadabra::vec2u 
kadabra::operator+(u32 A, kadabra::vec2u B) {
    kadabra::vec2u Result;

    Result.x = A + B.x;
    Result.y = A + B.y;

    return Result;
}

kadabra::vec2u
kadabra::operator+=(kadabra::vec2u& A, kadabra::vec2u B) {
    kadabra::vec2u Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec2u
kadabra::operator+=(kadabra::vec2u& A, u32 B) {
    kadabra::vec2u Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec2u 
kadabra::operator-(kadabra::vec2u A, kadabra::vec2u B) {
    kadabra::vec2u Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    
    return Result;
}

kadabra::vec2u 
kadabra::operator-(kadabra::vec2u A, u32 B) {
    kadabra::vec2u Result;

    Result.x = A.x - B;
    Result.y = A.y - B;

    return Result;
}

kadabra::vec2u 
kadabra::operator-(u32 A, kadabra::vec2u B) {
    kadabra::vec2u Result;

    Result.x = A - B.x;
    Result.y = A - B.y;

    return Result;
}

kadabra::vec2u
kadabra::operator-=(kadabra::vec2u& A, kadabra::vec2u B) {
    kadabra::vec2u Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec2u
kadabra::operator-=(kadabra::vec2u& A, u32 B) {
    kadabra::vec2u Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec2u
kadabra::operator*(u32 A, kadabra::vec2u B) {
    kadabra::vec2u Result;

    Result.x = A * B.x;
    Result.y = A * B.y;

    return Result;
}

kadabra::vec2u
kadabra::operator*(kadabra::vec2u A, u32 B) {
    kadabra::vec2u Result;

    Result.x = A.x * B;
    Result.y = A.y * B;

    return Result;
}

kadabra::vec2u
kadabra::operator*=(kadabra::vec2u& A, u32 B) {
    kadabra::vec2u Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec2u
kadabra::Hadamard(kadabra::vec2u A, kadabra::vec2u B){
    kadabra::vec2u Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec2u V){
    f32 Result;

    Result = Sqrt((f32)(V.x*V.x + V.y*V.y));

    return Result;
}

kadabra::vec2
kadabra::Normalize(kadabra::vec2u V){
    kadabra::vec2 Result;

    f32 L = Length(V);

    if(L != 0.0f){
        Result.x = V.x/L;
        Result.y = V.y/L;
    } else {
        Warning("Normalize() is called for a null kadabra::vector");
        Result = kadabra::Vec2(0.0f, 0.0f);
    }

    return Result;
}

u32
kadabra::Dot(kadabra::vec2u A, kadabra::vec2u B){
    u32 Result;

    Result = A.x*B.x + A.y*B.y;

    return Result;
}