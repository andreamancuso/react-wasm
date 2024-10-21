#ifndef TEXTURE_HELPERS_H
#define TEXTURE_HELPERS_H

#ifdef __EMSCRIPTEN__
#include <webgpu/webgpu.h>
#endif

struct Texture {
#ifdef __EMSCRIPTEN__
    WGPUTextureView textureView;
#else
	GLuint textureView;
#endif
    int width;
    int height;
};

#endif //TEXTURE_HELPERS_H
