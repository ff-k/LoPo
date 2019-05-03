#include "kdb_physics.h"

void 
kadabra::particle::Integrate(f32 DeltaTime){
    
    Position += Velocity*DeltaTime;
    
    vec3 Acceleration = Gravity;
    Velocity = (Velocity + Acceleration*DeltaTime) * Power(Damping, DeltaTime);
}
