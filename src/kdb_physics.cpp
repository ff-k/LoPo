#include "kdb_physics.h"

void 
kadabra::particle::Integrate(f32 DeltaTime){
    
    Position += Velocity*DeltaTime;
    
    vec3 Acceleration = Gravity + InverseMass*TotalForce;
    Velocity = (Velocity + Acceleration*DeltaTime) * Power(Damping, DeltaTime);
    
    TotalForce = Vec3(0.0f, 0.0f, 0.0f);
}
