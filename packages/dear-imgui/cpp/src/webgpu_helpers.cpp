#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <webgpu/webgpu.h>
#include "webgpu_helpers.h"

// borrowed from https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
bool LoadTexture(WGPUDevice device, const void* data, const int numBytes, Texture* texture)
{
    if (data == nullptr)
        return false;

    int width;
    int height;

    // TODO: figure out why we need the STB library to load image data for us, seems like I'm missing a step when using leptonica
    const auto stbiData = stbi_load_from_memory(static_cast<const stbi_uc*>(data), numBytes, &width, &height, nullptr, 4);

    WGPUTextureView view;
    {
        WGPUTextureDescriptor tex_desc = {};
        tex_desc.label = "texture";
        tex_desc.dimension = WGPUTextureDimension_2D;
        tex_desc.size.width = width;
        tex_desc.size.height = height;
        tex_desc.size.depthOrArrayLayers = 1;
        tex_desc.sampleCount = 1;
        tex_desc.format = WGPUTextureFormat_RGBA8Unorm;
        tex_desc.mipLevelCount = 1;
        tex_desc.viewFormatCount = 0;
        tex_desc.viewFormats = nullptr;
        tex_desc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;

        auto tex = wgpuDeviceCreateTexture(device, &tex_desc);

        WGPUTextureViewDescriptor tex_view_desc = {};
        tex_view_desc.format = WGPUTextureFormat_RGBA8Unorm;
        tex_view_desc.dimension = WGPUTextureViewDimension_2D;
        tex_view_desc.baseMipLevel = 0;
        tex_view_desc.mipLevelCount = 1;
        tex_view_desc.baseArrayLayer = 0;
        tex_view_desc.arrayLayerCount = 1;
        tex_view_desc.aspect = WGPUTextureAspect_All;
        view = wgpuTextureCreateView(tex, &tex_view_desc);

        WGPUImageCopyTexture dst_view = {};
        dst_view.texture = tex;
        dst_view.mipLevel = 0;
        dst_view.origin = { 0, 0, 0 };
        dst_view.aspect = WGPUTextureAspect_All;
        WGPUTextureDataLayout layout = {};
        layout.offset = 0;
        layout.bytesPerRow = width * 4;
        layout.rowsPerImage = height;
        const WGPUExtent3D size = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

        const auto queue = wgpuDeviceGetQueue(device);

        wgpuQueueWriteTexture(queue, &dst_view, stbiData, static_cast<uint32_t>(width * 4 * height), &layout, &size);

        wgpuQueueRelease(queue);
    }

    texture->textureView = view;
    texture->width = width;
    texture->height = height;

    stbi_image_free(stbiData);

    return true;
}