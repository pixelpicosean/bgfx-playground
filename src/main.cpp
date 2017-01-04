#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <bgfx_utils.h>
#include <common.h>

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

#include "vs_sprite.bin.h"
#include "fs_sprite.bin.h"

static const bgfx::EmbeddedShader shaders[] = {
  BGFX_EMBEDDED_SHADER(vs_sprite),
  BGFX_EMBEDDED_SHADER(fs_sprite),

  BGFX_EMBEDDED_SHADER_END()
};

struct PosTexcoordVertex {
  float x;
  float y;
  float u;
  float v;

  static void init() {
    decl
      .begin()
      .add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true, true)
      .end();
  }

  static bgfx::VertexDecl decl;
};

bgfx::VertexDecl PosTexcoordVertex::decl;

static PosTexcoordVertex vertices[4] = {
  { 100.0f, 100.0f, 0.0f, 0.0f },
  { 200.0f, 100.0f, 1.0f, 0.0f },
  { 200.0f, 200.0f, 1.0f, 1.0f },
  { 100.0f, 200.0f, 0.0f, 1.0f },
};

static uint16_t indices[6] = {
  0, 1, 2,
  0, 2, 3,
};

int _main_(int argc, char** argv) {
  // Constants
  uint32_t width = 640;
  uint32_t height = 480;
  uint32_t debug = BGFX_DEBUG_TEXT;
  uint32_t reset = BGFX_RESET_VSYNC;

  // Setup window
  entry::WindowHandle defaultWindow = {0};
  entry::setWindowTitle(defaultWindow, "LesserPanda");
  entry::setWindowSize(defaultWindow, width, height);
  /* entry::toggleFullscreen(defaultWindow); */

  // Initialize bgfx
  bgfx::init();
  bgfx::reset(width, height, reset);
  bgfx::setDebug(debug);

  bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x000000ff, 1.0f, 0);

  // Initialize vertex defination
  PosTexcoordVertex::init();

  // Prepare for rendering
  // - buffers
  auto vertexBuffer = bgfx::createVertexBuffer(
    bgfx::makeRef(vertices, sizeof(vertices)),
    PosTexcoordVertex::decl
  );
  auto indexBuffer = bgfx::createIndexBuffer(
    bgfx::makeRef(indices, sizeof(indices))
  );
  // - shaders
  const auto RenderType = bgfx::getRendererType();
  auto vs = bgfx::createEmbeddedShader(shaders, RenderType, "vs_sprite");
  auto fs = bgfx::createEmbeddedShader(shaders, RenderType, "fs_sprite");
  auto program = bgfx::createProgram(vs, fs, true /* destroy shaders when program is destroyed */);
  auto uTex = bgfx::createUniform("uTex", bgfx::UniformType::Int1);
  // - camera
  float view[16];
  bx::mtxOrtho(view, 0.0f, width, height, 0.0f, -1.0f, 1.0f);
  float proj[16];
  bx::mtxIdentity(proj);

  auto img = loadTexture("media/tank.png", 0
    | BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP
    | BGFX_TEXTURE_MIN_POINT | BGFX_TEXTURE_MAG_POINT
  );

  // Game loop
  int64_t timeOffset = bx::getHPCounter();
  while (!entry::processEvents(width, height, debug, reset)) {
    int64_t now = bx::getHPCounter();
    static int64_t last = now;
    const int64_t frameTime = now - last;
    last = now;
    const double freq = double(bx::getHPFrequency());
    const double toMs = 1000.0 / freq;
    const float dt = float((now - timeOffset) / double(bx::getHPFrequency()));

    // Set view and projection matrix
    bgfx::setViewTransform(0, view, proj);
    bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

    // Dummy draw call
    bgfx::touch(0);

    // Debug info
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 1, 0x4f, "Hello BGFX");
    bgfx::dbgTextPrintf(0, 2, 0x6f, "My name is Sean");

    // Draw something
    bgfx::setVertexBuffer(vertexBuffer);
    bgfx::setIndexBuffer(indexBuffer);

    bgfx::setTexture(0, uTex, img);

    bgfx::setState(0
      | BGFX_STATE_RGB_WRITE
      | BGFX_STATE_ALPHA_WRITE
      | BGFX_STATE_BLEND_SRC_ALPHA
    );

    bgfx::submit(0, program);

    // Advance to next frame
    bgfx::frame();
  }

  // Cleanup
  bgfx::destroyTexture(img);
  bgfx::destroyProgram(program);
  bgfx::destroyUniform(uTex);
  bgfx::shutdown();

  return 0;
}

