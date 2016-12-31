#include <stdlib.h>
#include <stdio.h>

#include <bgfx_utils.h>
#include <common.h>

#include <bgfx/bgfx.h>
#include <math.h>

#include "vs_sprite.bin.h"
#include "fs_sprite.bin.h"

struct PosTexcoordVertex {
  float x;
  float y;
  int16_t u;
  int16_t v;

  static void init() {
    decl
      .begin()
      .add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
      .end();
  }

  static bgfx::VertexDecl decl;
};

bgfx::VertexDecl PosTexcoordVertex::decl;

static PosTexcoordVertex vertices[4] = {
  { -0.5f, +0.5f, 0, 0 },
  { +0.5f, +0.5f, 0, 0 },
  { +0.5f, -0.5f, 0, 0 },
  { -0.5f, -0.5f, 0, 0 },
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
  auto vertexBuffer = bgfx::createVertexBuffer(
    bgfx::makeRef(vertices, sizeof(vertices)),
    PosTexcoordVertex::decl
  );
  auto indexBuffer = bgfx::createIndexBuffer(
    bgfx::makeRef(indices, sizeof(indices))
  );
  auto vs = bgfx::createShader(bgfx::makeRef(vs_sprite_glsl, sizeof(vs_sprite_glsl)));
  auto fs = bgfx::createShader(bgfx::makeRef(fs_sprite_glsl, sizeof(fs_sprite_glsl)));
  auto program = bgfx::createProgram(vs, fs);

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

    // Set view 0 default viewport
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

    bgfx::setState(0
      | BGFX_STATE_RGB_WRITE
      | BGFX_STATE_ALPHA_WRITE
      | BGFX_STATE_MSAA
    );

    bgfx::submit(0, program);

    // Advance to next frame
    bgfx::frame();
  }

  // Cleanup
  bgfx::shutdown();

  return 0;
}

