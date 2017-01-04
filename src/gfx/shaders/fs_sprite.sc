$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(uTex, 0);

void main() {
  gl_FragColor = texture2D(uTex, v_texcoord0);
}
