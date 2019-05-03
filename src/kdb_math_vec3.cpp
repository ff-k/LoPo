kadabra::vec3
kadabra::Vec3(f32 x, f32 y, f32 z){
    kadabra::vec3 Result;
    
    Result.x = x;
    Result.y = y;
    Result.z = z;
    
    return Result;
}

kadabra::vec3
kadabra::Vec3(kadabra::vec2 xy, f32 z){
    kadabra::vec3 Result;
    
    Result.x = xy.x;
    Result.y = xy.y;
    Result.z = z;
    
    return Result;
}

kadabra::vec3
kadabra::Vec3(f32 x, kadabra::vec2 yz){
    kadabra::vec3 Result;
    
    Result.x = x;
    Result.y = yz.x;
    Result.z = yz.y;
    
    return Result;
}

kadabra::vec3 
kadabra::operator+(kadabra::vec3 A, kadabra::vec3 B) {
    kadabra::vec3 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return Result;
}

kadabra::vec3 
kadabra::operator+(kadabra::vec3 A, f32 B) {
    kadabra::vec3 Result;

    Result.x = A.x + B;
    Result.y = A.y + B;
    Result.z = A.z + B;

    return Result;
}

kadabra::vec3 
kadabra::operator+(f32 A, kadabra::vec3 B) {
    kadabra::vec3 Result;

    Result.x = A + B.x;
    Result.y = A + B.y;
    Result.z = A + B.z;

    return Result;
}

kadabra::vec3
kadabra::operator+=(kadabra::vec3& A, kadabra::vec3 B) {
    kadabra::vec3 Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec3
kadabra::operator+=(kadabra::vec3& A, f32 B) {
    kadabra::vec3 Result;

    Result = A + B;

    A = Result;
    
    return Result;
}

kadabra::vec3 
kadabra::operator-(kadabra::vec3 A, kadabra::vec3 B) {
    kadabra::vec3 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    
    return Result;
}

kadabra::vec3 
kadabra::operator-(kadabra::vec3 A, f32 B) {
    kadabra::vec3 Result;

    Result.x = A.x - B;
    Result.y = A.y - B;
    Result.z = A.z - B;

    return Result;
}

kadabra::vec3 
kadabra::operator-(f32 A, kadabra::vec3 B) {
    kadabra::vec3 Result;

    Result.x = A - B.x;
    Result.y = A - B.y;
    Result.z = A - B.z;

    return Result;
}

kadabra::vec3
kadabra::operator-=(kadabra::vec3& A, kadabra::vec3 B) {
    kadabra::vec3 Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec3
kadabra::operator-=(kadabra::vec3& A, f32 B) {
    kadabra::vec3 Result;

    Result = A - B;

    A = Result;
    
    return Result;
}

kadabra::vec3
kadabra::operator*(f32 A, kadabra::vec3 B) {
    kadabra::vec3 Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;

    return Result;
}

kadabra::vec3
kadabra::operator*(kadabra::vec3 A, f32 B) {
    kadabra::vec3 Result;

    Result.x = A.x * B;
    Result.y = A.y * B;
    Result.z = A.z * B;

    return Result;
}

kadabra::vec3
kadabra::operator*=(kadabra::vec3& A, f32 B) {
    kadabra::vec3 Result;

    Result = A * B;

    A = Result;
    
    return Result;
}

kadabra::vec3
kadabra::operator-(kadabra::vec3 &v1){
    return kadabra::Vec3(-v1.x, -v1.y, -v1.z);
}

kadabra::vec3
kadabra::Hadamard(kadabra::vec3 A, kadabra::vec3 B){
    kadabra::vec3 Result;
    
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;
    Result.z = A.z * B.z;
    
    return Result;
}

f32
kadabra::Length(kadabra::vec3 V){
    f32 Result;

    Result = Sqrt(
                  V.x*V.x + 
                  V.y*V.y + 
                  V.z*V.z
                  );

    return Result;
}

kadabra::vec3
kadabra::Normalize(kadabra::vec3 V){
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

f32
kadabra::Dot(kadabra::vec3 A, kadabra::vec3 B){
    f32 Result;

    Result = A.x*B.x + A.y*B.y + A.z*B.z;

    return Result;
}

kadabra::vec3
kadabra::Cross(kadabra::vec3 A, kadabra::vec3 B){
    kadabra::vec3 Result;

    Result = kadabra::Vec3(A.y*B.z - A.z*B.y,
                  A.z*B.x - A.x*B.z,
                  A.x*B.y - A.y*B.x);

    return Result;
}

kadabra::vec3
kadabra::Reflect(kadabra::vec3 V, kadabra::vec3 R){
    // NOTE(furkan):
    // \     |     /
    //  V    R    Result
    //   \   |   /
    // _____________

    kadabra::vec3 Result;

    Result = ((2.0f * Dot(V, R)) * R) - V;

    return Result;
}

kadabra::vec3 
kadabra::RotateAround(kadabra::vec3 V, f32 AngleDegrees, kadabra::vec3 Axis){

    kadabra::vec3 Result = V;

    if(AngleDegrees != 0.0f){

        kadabra::vec3 Projected = Dot(V, Axis) * Normalize(Axis);
        kadabra::vec3 RotationAxisX = V - Projected;
        kadabra::vec3 RotationAxisY = Cross(Axis, RotationAxisX);

        f32 RotationAxisXLength = Length(RotationAxisX);
        f32 RotationAxisYLength = Length(RotationAxisY);
 
        f32 AngleRadians = DegreeToRadian(AngleDegrees);
   
        kadabra::vec3 RotatedOnAxisX = (Cos(AngleRadians) / RotationAxisXLength) * 
                              RotationAxisX;
        kadabra::vec3 RotatedOnAxisY = (Sin(AngleRadians) / RotationAxisYLength) *
                              RotationAxisY;
    
        Result = RotationAxisXLength * (RotatedOnAxisX + 
                                        RotatedOnAxisY);
        
        Result += Projected;
    }
    
    return Result;
}
