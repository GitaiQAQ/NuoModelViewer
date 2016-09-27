

#import "NuoMathUtilities.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static matrix_float4x4& to_matrix(glm::mat4x4& gmat)
{
    matrix_float4x4* result = (matrix_float4x4*)(&gmat);
    return *result;
}

matrix_float4x4 matrix_float4x4_translation(vector_float3 t)
{
    glm::vec3 gt(t.x, t.y, t.z);
    glm::mat4x4 gmat = glm::translate(glm::mat4x4(1.0), gt);
    
    return to_matrix(gmat);
}

matrix_float4x4 matrix_float4x4_uniform_scale(float scale)
{
    glm::mat4x4 gmat = glm::scale(glm::mat4x4(1.0), glm::vec3(scale));
    return to_matrix(gmat);
}

matrix_float4x4 matrix_float4x4_rotation(vector_float3 axis, float angle)
{
    glm::vec3 gaxis(axis.x, axis.y, axis.z);
    glm::mat4x4 gmat = glm::rotate(glm::mat4x4(1.0), -angle, gaxis);
    
    return to_matrix(gmat);
}

matrix_float4x4 matrix_float4x4_perspective(float aspect, float fovy, float near, float far)
{
    // NOT use OpenGL persepctive!
    // Metal uses a 2x2x1 canonical cube (z in [0,1]), rather than the 2x2x2 one in OpenGL.
    
    // glm::mat4x4 gmat = glm::perspective(fovy, aspect, near, far);
    /*
     T const tanHalfFovy = tan(fovy / static_cast<T>(2));
     
     tmat4x4<T, defaultp> Result(static_cast<T>(0));
     Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
     Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
     Result[2][2] = - (zFar + zNear) / (zFar - zNear);
     Result[2][3] = - static_cast<T>(1);
     Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
     return Result;
     */
    
    float yScale = 1 / tan(fovy * 0.5);
    float xScale = yScale / aspect;
    float zRange = far - near;
    float zScale = -(far) / zRange;
    float wzScale = - far * near / zRange;
    
    vector_float4 P = { xScale, 0, 0, 0 };
    vector_float4 Q = { 0, yScale, 0, 0 };
    vector_float4 R = { 0, 0, zScale, -1 };
    vector_float4 S = { 0, 0, wzScale, 0 };
    
    matrix_float4x4 mat = { P, Q, R, S };
    return mat;
}

matrix_float3x3 matrix_float4x4_extract_linear(matrix_float4x4 m)
{
    vector_float3 X = m.columns[0].xyz;
    vector_float3 Y = m.columns[1].xyz;
    vector_float3 Z = m.columns[2].xyz;
    matrix_float3x3 l = { X, Y, Z };
    return l;
}
