#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <bgfx/bgfx.h>
#include <common.h>

struct PosTexcoordVertex {
  float x = 0.0f;
  float y = 0.0f;
  int16_t u = 0;
  int16_t v = 0;

  static void init() {
    decl
      .begin()
      .add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
      .end();
  }

  static bgfx::VertexDecl decl;
};

bgfx::VertexDecl PosTexcoordVertex::scDecl;

int _main_(int argc, char* const* argv) {
  uint32_t width = 640;
  uint32_t height = 480;
  uint32_t debug = BGFX_DEBUG_MODE;
  uint32_t reset = BGFX_RESET_VSYNC;

  entry::WindowHandle defaultWindow = nullptr;
  entry::setWindowTitle(defaultWindow, "LesserPanda");
  //entry::toggleFullscreen(defaultWindow);

  bgfx::init();
  bgfx::reset(width, height, reset);
  bgfx::setDebug(debug);

  bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x000000ff, 1.0f, 0);

  Vertex::init();

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

    // Advance to next frame
    bgfx::frame();
  }

  bgfx::shutdown();

  return 0;
}

