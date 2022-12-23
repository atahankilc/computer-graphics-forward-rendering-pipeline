cmake_minimum_required(VERSION 3.21)
project(477_hw2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(rasterizer Camera.cpp Camera.h Color.cpp Color.h Vector/Helpers.cpp Vector/Helpers.h Main.cpp Matrix4.cpp Matrix4.h
        Mesh.cpp Mesh.h Transformation/Rotation.cpp Transformation/Rotation.h Transformation/Scaling.cpp Transformation/Scaling.h Scene.cpp Scene.h tinyxml2.cpp tinyxml2.h
        Transformation/Translation.cpp Transformation/Translation.h Triangle.cpp Triangle.h Vector/Vec3.cpp Vector/Vec3.h Vector/Vec4.cpp Vector/Vec4.h Transformation/Base.cpp Transformation/Base.h)

