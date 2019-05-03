kadabra::vec2i
kadabra::Vec2i(s32 x, s32 y){
    kadabra::vec2i Result;
    
    Result.x = x;
    Result.y = y;
    
    return Result;
}

kadabra::vec2i 
kadabra::operator+(kadabra::vec2i A, kadabra::vec2i B) {
    kadabra::vec2i Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return Result;
}

kadabra::vec2i 
kadabra::operator+(kadabra::vec2i A, s32 B) {
    kadabra::vec2i Result;

    Result.x = A.x + B;
    Result.y = A.y + B;

    return Result;
}

kadabra::vec2i 
kadabra::operator+(s32 A, kadabra::vec2i B) {
    kadabra::vec2i Result;

    Result.x = A + B.x;
    Result.y = A + B.y;

    return Result;
}

kadabra::vec2i
kadabra::operator+=(kadabra::vec2i& A, kadabra::vec2i B) {
    kadabra::vec2i Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec2i
kadabra::operator+=(kadabra::vec2i& A, s32 B) {
    kadabra::vec2i Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec2i 
kadabra::operator-(kadabra::vec2i A, kadabra::vec2i B) {
    kadabra::vec2i Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    
    return Result;
}

kadabra::vec2i 
kadabra::operator-(kadabra::vec2i A, s32 B) {
    kadabra::vec2i Result;

    Result.x = A.x - B;
    Result.y = A.y - B;

    return Result;
}

kadabra::vec2i 
kadabra::operator-(s32 A, kadabra::vec2i B) {
    kadabra::vec2i Result;

    Result.x = A - B.x;
    Result.y = A - B.y;

    return Result;
}

kadabra::vec2i
kadabra::operator-=(kadabra::vec2i& A, kadabra::vec2i B) {
    kadabra::vec2i Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec2i
kadabra::operator-=(kadabra::vec2i& A, s32 B) {
    kadabra::vec2i Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec2i
kadabra::operator*(s32 A, kadabra::vec2i B) {
    kadabra::vec2i Result;

    Result.x = A * B.x;
    Result.y = A * B.y;

    return Result;
}

kadabra::vec2i
kadabra::operator*(kadabra::vec2i A, s32 B) {
    kadabra::vec2i Result;

    Result.x = A.x * B;
    Result.y = A.y * B;

    return Result;
}

kadabra::vec2i
kadabra::operator*=(kadabra::vec2i& A, s32 B) {
    kadabra::vec2i Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec2i
kadabra::operator-(kadabra::vec2i &v1){
    return kadabra::Vec2i(-v1.x, -v1.y);
}

kadabra::vec2i
kadabra::Hadamard(kadabra::vec2i A, kadabra::vec2i B){
    kadabra::vec2i Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec2i V){
    f32 Result;

    Result = Sqrt((f32)(V.x*V.x + V.y*V.y));

    return Result;
}

kadabra::vec2
kadabra::Normalize(kadabra::vec2i V){
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

s32
kadabra::Dot(kadabra::vec2i A, kadabra::vec2i B){
    s32 Result;

    Result = A.x*B.x + A.y*B.y;

    return Result;
}