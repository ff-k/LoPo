#include "kdb_ray.h"

b32
kadabra::RayTriangleIntersection(ray_hit *Hit, ray3 Ray, 
                                 vec3 P0, vec3 P1, vec3 P2){
    vec3 ZeroVec = Vec3(0.0f, 0.0f, 0.0f);
    return RayTriangleIntersection(Hit, Ray, P0, P1, P2,
                                   ZeroVec, ZeroVec, ZeroVec);
}

b32
kadabra::RayTriangleIntersection(ray_hit *Hit, ray3 Ray, 
                                 vec3 P0, vec3 P1, vec3 P2,
                                 vec3 N0, vec3 N1, vec3 N2){
    b32 Result = false;
    
    f32 A = (P0.x - P1.x);
    f32 B = (P0.x - P2.x);
    f32 C = Ray.Direction.x;

    f32 D = (P0.y - P1.y);
    f32 E = (P0.y - P2.y);
    f32 F = Ray.Direction.y;

    f32 G = (P0.z - P1.z);
    f32 H = (P0.z - P2.z);
    f32 I = Ray.Direction.z;
    
    f32 DetM       = (A*E*I) + (D*H*C) + (G*B*F) - 
                     (C*E*G) - (F*H*A) - (I*B*D);

    f32 K = (P0.x - Ray.Origin.x);
    f32 M = (P0.y - Ray.Origin.y);
    f32 N = (P0.z - Ray.Origin.z);

    f32 DetM_Beta  = (K*E*I) + (M*H*C) + (N*B*F) - 
                     (C*E*N) - (F*H*K) - (I*B*M);
    
    f32 LowerLimit = -0.000001f;
    f32 Beta = DetM_Beta / DetM;
    if(Beta > LowerLimit && Beta <= 1.0f){
        f32 DetM_Gamma = (A*M*I) + (D*N*C) + (G*K*F) - 
                         (C*M*G) - (F*N*A) - (I*K*D);
 
        f32 Gamma = DetM_Gamma / DetM;
        if(Gamma > LowerLimit && Gamma <= 1.0f){
            f32 DetM_t = (A*E*N) + (D*H*K) + (G*B*M) - 
                         (K*E*G) - (M*H*A) - (N*B*D);
            f32 Alpha = 1.0f - Beta - Gamma;
            if(Alpha > LowerLimit){

                f32 t = DetM_t / DetM;
                if(t > 0.0f && t < Hit->t){
                    Hit->P = Ray.Origin + t*Ray.Direction;
                    Hit->Normal = Alpha*N0 + Beta*N1 + Gamma*N2;
                    Hit->t = t;
                    Result = true; 
                }
            }
        }
    }

    return Result;
}
