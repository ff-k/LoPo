kadabra::vec2
kadabra::Vec2(f32 x, f32 y){
    kadabra::vec2 Result;
    
    Result.x = x;
    Result.y = y;
    
    return Result;
}

kadabra::vec2 
kadabra::operator+(kadabra::vec2 A, kadabra::vec2 B) {
    kadabra::vec2 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return Result;
}

kadabra::vec2 
kadabra::operator+(kadabra::vec2 A, f32 B) {
    kadabra::vec2 Result;

    Result.x = A.x + B;
    Result.y = A.y + B;

    return Result;
}

kadabra::vec2 
kadabra::operator+(f32 A, kadabra::vec2 B) {
    kadabra::vec2 Result;

    Result.x = A + B.x;
    Result.y = A + B.y;

    return Result;
}

kadabra::vec2
kadabra::operator+=(kadabra::vec2& A, kadabra::vec2 B) {
    kadabra::vec2 Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec2
kadabra::operator+=(kadabra::vec2& A, f32 B) {
    kadabra::vec2 Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec2 
kadabra::operator-(kadabra::vec2 A, kadabra::vec2 B) {
    kadabra::vec2 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    
    return Result;
}

kadabra::vec2 
kadabra::operator-(kadabra::vec2 A, f32 B) {
    kadabra::vec2 Result;

    Result.x = A.x - B;
    Result.y = A.y - B;

    return Result;
}

kadabra::vec2 
kadabra::operator-(f32 A, kadabra::vec2 B) {
    kadabra::vec2 Result;

    Result.x = A - B.x;
    Result.y = A - B.y;

    return Result;
}

kadabra::vec2
kadabra::operator-=(kadabra::vec2& A, kadabra::vec2 B) {
    kadabra::vec2 Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec2
kadabra::operator-=(kadabra::vec2& A, f32 B) {
    kadabra::vec2 Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec2
kadabra::operator*(f32 A, kadabra::vec2 B) {
    kadabra::vec2 Result;

    Result.x = A * B.x;
    Result.y = A * B.y;

    return Result;
}

kadabra::vec2
kadabra::operator*(kadabra::vec2 A, f32 B) {
    kadabra::vec2 Result;

    Result.x = A.x * B;
    Result.y = A.y * B;

    return Result;
}

kadabra::vec2
kadabra::operator*=(kadabra::vec2& A, f32 B) {
    kadabra::vec2 Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec2
kadabra::operator-(kadabra::vec2 &v1){
    return kadabra::Vec2(-v1.x, -v1.y);
}

kadabra::vec2
kadabra::Hadamard(kadabra::vec2 A, kadabra::vec2 B){
    kadabra::vec2 Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec2 V){
    f32 Result;

    Result = Sqrt(
                  V.x*V.x + 
                  V.y*V.y
                  );

    return Result;
}

kadabra::vec2
kadabra::Normalize(kadabra::vec2 V){
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

f32
kadabra::Dot(kadabra::vec2 A, kadabra::vec2 B){
    f32 Result;

    Result = A.x*B.x + A.y*B.y;

    return Result;
}

kadabra::vec2
kadabra::Reflect(kadabra::vec2 V, kadabra::vec2 R){
    // NOTE(furkan):
    // \     |     /
    //  V    R    Result
    //   \   |   /
    // _____________

    kadabra::vec2 Result;

    Result = ((2.0f * Dot(V, R)) * R) - V;

    return Result;
}