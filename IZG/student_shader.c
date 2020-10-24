/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {

  // Get uniforms handle
  Uniforms const uniformsHandle = gpu_getUniformsHandle(gpu);

  // Get View and Projection Matrices locations
  UniformLocation const viewMatrixLocation = getUniformLocation(gpu, "viewMatrix");
  UniformLocation const projectionMatrixLocation = getUniformLocation(gpu, "projectionMatrix");

  // Get View and Projection Matices values
  Mat4 const *const viewMat = shader_interpretUniformAsMat4(uniformsHandle, viewMatrixLocation);
  Mat4 const *const projectionMat = shader_interpretUniformAsMat4(uniformsHandle, projectionMatrixLocation);

  // Get position
  Vec3 const *const position = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
  Vec3 const *const normal = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);

  Mat4 mvp;
  multiply_Mat4_Mat4(&mvp, projectionMat, viewMat);

  Vec4 pos4;
  copy_Vec3Float_To_Vec4(&pos4, position, 1.f);

  multiply_Mat4_Vec4(&output->gl_Position, &mvp, &pos4);

  Vec3 *const positionOutput = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
  Vec3 *const normalOutput = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);

  // Init Vec3 position & normal
  init_Vec3(positionOutput, position->data[0], position->data[1], position->data[2]);
  init_Vec3(normalOutput, normal->data[0], normal->data[1], normal->data[2]);
}

float retVal(float v, float lo, float hi) {
	if (v < hi) return v > lo ? v : lo;
	else return v < hi ? v : hi;
}

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {

  // Get uniforms handle
	Uniforms const uniformsHandle = gpu_getUniformsHandle(gpu);

  // Get values of cameraPosition & lightPosition
	Vec3 const*const cameraVec = shader_interpretUniformAsVec3(uniformsHandle, getUniformLocation(gpu, "cameraPosition"));
  Vec3 const*const lightVec = shader_interpretUniformAsVec3(uniformsHandle, getUniformLocation(gpu, "lightPosition"));

  // Interpret attributes as Vec3
	Vec3 const*const attrPosition = fs_interpretInputAttributeAsVec3(gpu, input, 0);
  Vec3 const*const attrNormal = fs_interpretInputAttributeAsVec3(gpu, input, 1);
  Vec3 n_normal; normalize_Vec3(&n_normal, attrNormal);
  float shinessFactor = 40.f;

  Vec3 toView;
  sub_Vec3(&toView, attrPosition, cameraVec);
  normalize_Vec3(&toView, &toView);

  Vec3 lightDir;
  sub_Vec3(&lightDir, lightVec, attrPosition);
  normalize_Vec3(&lightDir, &lightDir);

  Vec3 reflection;
  reflect(&reflection, &lightDir, &n_normal);

  float refDot = max(dot_Vec3(&reflection, &toView), 0.0f);
  output->color.data[0] = output->color.data[2] = (float) pow(refDot, shinessFactor);
  output->color.data[1] = max(dot_Vec3(&lightDir, &n_normal), 0.0f);
  output->color.data[3] = 0.0f;

  // FragmentShader použitý od github.com/iwitrag
  // https://github.com/Iwitrag/FIT-VUT/blob/master/Z%C3%A1klady%20po%C4%8D%C3%ADta%C4%8Dov%C3%A9%20grafiky%20(C)/student_shader.c
}

/// @}
