#ifdef __EMSCRIPTEN__
#include <webgpu/webgpu.h>
#endif

#ifndef TEXTURE_HELPERS_H
#define TEXTURE_HELPERS_H

struct Texture {
#ifdef __EMSCRIPTEN__
    WGPUTextureView textureView;
#else
	GLuint textureView;
#endif
    int width;
    int height;
};

bool LoadTexture(WGPUDevice device, const void* data, int numBytes, Texture* texture);

#endif //TEXTURE_HELPERS_H
