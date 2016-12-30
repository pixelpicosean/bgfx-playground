bgfx_include_compat()

fips_begin_app(Game windowed)
  fips_files(main.cpp)
  # fips_src(gfx)

  # fips_dir(gfx/shaders)
  # fips_files(varying.def.sc)
  # bgfx_shaders(FILES vs_sprite.sc fs_sprite.sc)

  fips_deps(Remotery)
  fips_deps(bgfx bgfx-examples-common bgfx-imgui bgfx-ib-compress)
fips_end_app()
