#include "kdb_physics.h"

void 
kadabra::component_particle::Integrate(f32 DeltaTime){
    
    Prev_Position = Position;
    Prev_Velocity = Velocity;
    
    Position += Velocity*DeltaTime;
    
    vec3 Acceleration = Gravity;
    Velocity = (Velocity + Acceleration*DeltaTime) * Power(Damping, DeltaTime);
}

void 
kadabra::component_particle::UndoLastIntegration(){
    Position = Prev_Position;
    Velocity = Prev_Velocity;
}
