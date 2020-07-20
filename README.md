# LoPo
A 3D "game" written to get familiar with game physics concepts in less than a month.

# Features
* Broad phase collision detection using AABB tests
* Narrow phase collision detection using GJK
* Uses BVH to accelerate collision detection
* Springs
* Two types of integrators (explicit and semi-implicit Euler)
* Ability to simulate forces (such as gravity)
* Uses a homebrew engine with OpenGL 3.2 as rendering backend

# Notes
* Ceiling and floor meshes are generated using Perlin textures
* stb_image and stb_image_write to read/write images. No other third-party library is used

# Known Issues
* Handling of asset management and scene management is pretty lousy
* Springs are instable due to integrators used. Using RK4 would probably give more pleasant results
* GJK does not produce contact data properly
