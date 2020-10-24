/*!
 * @file
 * @brief This file contains implementation of cpu side for phong shading.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include <assert.h>
#include <math.h>

#include <student/buffer.h>
#include <student/bunny.h>
#include <student/camera.h>
#include <student/gpu.h>
#include <student/linearAlgebra.h>
#include <student/mouseCamera.h>
#include <student/student_cpu.h>
#include <student/student_pipeline.h>
#include <student/student_shader.h>
#include <student/swapBuffers.h>
#include <student/uniforms.h>
#include <student/vertexPuller.h>

#include <student/globals.h>

/**
 * @brief This structure contains all global variables for this method.
 */
struct PhongVariables {
  /// This variable contains GPU handle.
  GPU gpu;
  /// This variable contains light poistion in world-space.
  Vec3 lightPosition;

  ProgramID program;
  BufferID verticesBuffer;
  BufferID indicesBuffer;
  VertexPullerID vertexPuller;

} phong;  ///<instance of all global variables for triangle example.

/// \addtogroup cpu_side Úkoly v cpu části
/// @{

void phong_onInit(int32_t width, int32_t height) {
  // create gpu
  phong.gpu = cpu_createGPU();
  // set viewport size
  cpu_setViewportSize(phong.gpu, (size_t)width, (size_t)height);
  // init matrices
  cpu_initMatrices(width, height);
  // init lightPosition
  init_Vec3(&phong.lightPosition, 1000.f, 1000.f, 1000.f);

  // Reserve uniform variables
  cpu_reserveUniform(phong.gpu, "viewMatrix", UNIFORM_MAT4);
  cpu_reserveUniform(phong.gpu, "projectionMatrix", UNIFORM_MAT4);
  cpu_reserveUniform(phong.gpu, "lightPosition", UNIFORM_VEC3);
  cpu_reserveUniform(phong.gpu, "cameraPosition", UNIFORM_VEC3);

  // Create program
  phong.program = cpu_createProgram(phong.gpu);

  // Attach shaders to program
  cpu_attachVertexShader(phong.gpu, phong.program, phong_vertexShader);
  cpu_attachFragmentShader(phong.gpu, phong.program, phong_fragmentShader);

  // Set attributes interpolation
  cpu_setAttributeInterpolation(phong.gpu, phong.program, 0, ATTRIB_VEC3, SMOOTH);
  cpu_setAttributeInterpolation(phong.gpu, phong.program, 1, ATTRIB_VEC3, SMOOTH);

  // Create buffers
  cpu_createBuffers(phong.gpu, 2, &phong.verticesBuffer);
  cpu_createBuffers(phong.gpu, 1, &phong.indicesBuffer);

  // Set buffers data
  cpu_bufferData(phong.gpu, phong.verticesBuffer, sizeof(bunnyVertices), bunnyVertices);
  cpu_bufferData(phong.gpu, phong.indicesBuffer, sizeof(bunnyIndices), bunnyIndices);

  // Create Vertex pullers
  cpu_createVertexPullers(phong.gpu, 1, &phong.vertexPuller);

  // Set & enable Vertex puller heads
  cpu_setVertexPullerHead(phong.gpu, phong.vertexPuller, 0, phong.verticesBuffer, sizeof(float) * 0, sizeof(float) * 6);
  cpu_setVertexPullerHead(phong.gpu, phong.vertexPuller, 1, phong.verticesBuffer, sizeof(float) * 3, sizeof(float) * 6);
  cpu_enableVertexPullerHead(phong.gpu, phong.vertexPuller, 0);
  cpu_enableVertexPullerHead(phong.gpu, phong.vertexPuller, 1);

  // Set indexing
  cpu_setIndexing(phong.gpu, phong.vertexPuller, phong.indicesBuffer, sizeof(float));

}

/// @}

void phong_onExit() { cpu_destroyGPU(phong.gpu); }

/// \addtogroup cpu_side
/// @{

void phong_onDraw(SDL_Surface* surface) {
  assert(surface != NULL);

  // clear depth buffer
  cpu_clearDepth(phong.gpu, +INFINITY);
  Vec4 color;
  init_Vec4(&color, .1f, .1f, .1f, 1.f);
  // clear color buffer
  cpu_clearColor(phong.gpu, &color);

  // Initialize program
  cpu_useProgram(phong.gpu, phong.program);

  // Bind Vertex pullers
  cpu_bindVertexPuller(phong.gpu, phong.vertexPuller);

  // Set 'projectionMatrix' uniform variable
  const UniformLocation projMatrixLoc = getUniformLocation(phong.gpu, "projectionMatrix");
  cpu_uniformMatrix4fv(
    phong.gpu,
    projMatrixLoc,
    (float *) &projectionMatrix
  );

  // Set 'viewMatrix' uniform variable
  const UniformLocation viewMatrixLoc = getUniformLocation(phong.gpu, "viewMatrix");
  cpu_uniformMatrix4fv(
    phong.gpu,
    viewMatrixLoc,
    (float *) &viewMatrix
  );

  // Set 'cameraPosition' uniform variable
  const UniformLocation camPositionLoc = getUniformLocation(phong.gpu, "cameraPosition");
  cpu_uniform3f(
    phong.gpu,
    camPositionLoc,
    cameraPosition.data[0],
    cameraPosition.data[1],
    cameraPosition.data[2]
  );

  // Set 'lightPosition' uniform variable
  const UniformLocation lightPositionLoc = getUniformLocation(phong.gpu, "lightPosition");
  cpu_uniform3f(
    phong.gpu,
    lightPositionLoc,
    phong.lightPosition.data[0],
    phong.lightPosition.data[1],
    phong.lightPosition.data[2]
  );

  // Draw triangles
  cpu_drawTriangles(phong.gpu, 3 * 2092);

  // copy image from gpu to SDL surface
  cpu_swapBuffers(surface, phong.gpu);
}

/// @}
