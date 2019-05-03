kadabra::vec3i
kadabra::Vec3i(s32 x, s32 y, s32 z){
    kadabra::vec3i Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = z;
    
    return Result;
}

kadabra::vec3i
kadabra::Vec3i(kadabra::vec2i xy, s32 z){
    kadabra::vec3i Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = z;
    
    return Result;
}

kadabra::vec3i
kadabra::Vec3i(s32 x, kadabra::vec2i yz){
    kadabra::vec3i Result;
    
    Result.x = x;
    Result.y = yz.x;
    Result.z = yz.y;
    
    return Result;
}

kadabra::vec3i 
kadabra::operator+(kadabra::vec3i A, kadabra::vec3i B) {
    kadabra::vec3i Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return Result;
}

kadabra::vec3i 
kadabra::operator+(kadabra::vec3i A, s32 B) {
    kadabra::vec3i Result;

    Result.x = A.x + B;
    Result.y = A.y + B;
    Result.z = A.z + B;

    return Result;
}

kadabra::vec3i 
kadabra::operator+(s32 A, kadabra::vec3i B) {
    kadabra::vec3i Result;

    Result.x = A + B.x;
    Result.y = A + B.y;
    Result.z = A + B.z;

    return Result;
}

kadabra::vec3i
kadabra::operator+=(kadabra::vec3i& A, kadabra::vec3i B) {
    kadabra::vec3i Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec3i
kadabra::operator+=(kadabra::vec3i& A, s32 B) {
    kadabra::vec3i Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec3i 
kadabra::operator-(kadabra::vec3i A, kadabra::vec3i B) {
    kadabra::vec3i Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    
    return Result;
}

kadabra::vec3i 
kadabra::operator-(kadabra::vec3i A, s32 B) {
    kadabra::vec3i Result;

    Result.x = A.x - B;
    Result.y = A.y - B;
    Result.z = A.z - B;

    return Result;
}

kadabra::vec3i 
kadabra::operator-(s32 A, kadabra::vec3i B) {
    kadabra::vec3i Result;

    Result.x = A - B.x;
    Result.y = A - B.y;
    Result.z = A - B.z;

    return Result;
}

kadabra::vec3i
kadabra::operator-=(kadabra::vec3i& A, kadabra::vec3i B) {
    kadabra::vec3i Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec3i
kadabra::operator-=(kadabra::vec3i& A, s32 B) {
    kadabra::vec3i Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec3i
kadabra::operator*(s32 A, kadabra::vec3i B) {
    kadabra::vec3i Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;

    return Result;
}

kadabra::vec3i
kadabra::operator*(kadabra::vec3i A, s32 B) {
    kadabra::vec3i Result;

    Result.x = A.x * B;
    Result.y = A.y * B;
    Result.z = A.z * B;

    return Result;
}

kadabra::vec3i
kadabra::operator*=(kadabra::vec3i& A, s32 B) {
    kadabra::vec3i Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec3i
kadabra::operator-(kadabra::vec3i &v1){
    return kadabra::Vec3i(-v1.x, -v1.y, -v1.z);
}

kadabra::vec3i
kadabra::Hadamard(kadabra::vec3i A, kadabra::vec3i B){
    kadabra::vec3i Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    Result.z = A.z * B.z;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec3i V){
    f32 Result;

    Result = Sqrt((f32)(V.x*V.x + V.y*V.y + V.z*V.z));

    return Result;
}

kadabra::vec3
kadabra::Normalize(kadabra::vec3i V){
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

s32
kadabra::Dot(kadabra::vec3i A, kadabra::vec3i B){
    s32 Result;

    Result = A.x*B.x + A.y*B.y + A.z*B.z;

    return Result;
}

kadabra::vec3i
kadabra::Cross(kadabra::vec3i A, kadabra::vec3i B){
    kadabra::vec3i Result;

    Result = kadabra::Vec3i(A.y*B.z - A.z*B.y,
                   A.z*B.x - A.x*B.z,
                   A.x*B.y - A.y*B.x);

    return Result;
}