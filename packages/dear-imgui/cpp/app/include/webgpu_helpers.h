#include <webgpu/webgpu.h>

#ifndef WEBGPU_HELPERS_H
#define WEBGPU_HELPERS_H

struct Texture {
    WGPUTextureView textureView;
    int width;
    int height;
};

bool LoadTexture(WGPUDevice device, const void* data, int numBytes, Texture* texture);

#endif //WEBGPU_HELPERS_H
