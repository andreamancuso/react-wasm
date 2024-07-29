#include <string>
#include <sstream>
#include <iomanip>

#include <webgpu/webgpu.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"
#include "shared.h"

#pragma once

ImVec4 RGBAtoIV4(int r, int g, int b, float a) {
	float newr = r / 255.0;
	float newg = g / 255.0;
	float newb = b / 255.0;

	return ImVec4(newr, newg, newb, a);
}
ImVec4 RGBAtoIV4(int r, int g, int b) {
	return RGBAtoIV4(r, g, b, 1.0f);
}

ImVec4 HEXAtoIV4(const char* hex, float a) {
	int r, g, b;
	std::sscanf(hex, "%02x%02x%02x", &r, &g, &b);
	return RGBAtoIV4(r, g, b, a);
}
ImVec4 HEXAtoIV4(const char* hex) {
	return HEXAtoIV4(hex, 1.0f);
}

float charPercentageToFloat(const char* input) {
    float value;
    std::sscanf(input, "%f%%", &value);

    return value;
}

json IV4toJson(ImVec4 imVec4) {
    json j = {
        {"x", imVec4.x},
        {"y", imVec4.y},
        {"z", imVec4.z},
        {"w", imVec4.w}
    };

    return j;
};

json IV4toJsonTuple(ImVec4 imVec4) {
    json j = {
        imVec4.x,
        imVec4.y,
        imVec4.z,
        imVec4.w
    };

    return j;
};

json IV4toJsonRGBATuple(ImVec4 imVec4) {
    json j = {
        (int)(imVec4.x * 255),
        (int)(imVec4.y * 255),
        (int)(imVec4.z * 255),
        imVec4.w
    };

    return j;
};

// todo: refactor
json IV4toJsonHEXATuple(ImVec4 imVec4) {

    auto rAsInt = (int)(imVec4.x * 255);
    auto gAsInt = (int)(imVec4.y * 255);
    auto bAsInt = (int)(imVec4.z * 255);

    std::string h = "#";

    std::ostringstream rSs;
    rSs << std::setfill('0') << std::setw(2) << std::hex << rAsInt;
    h += rSs.str();

    std::ostringstream gSs;
    gSs << std::setfill('0') << std::setw(2) << std::hex << gAsInt;
    h +=  gSs.str();

    std::ostringstream bSs;
    bSs << std::setfill('0') << std::setw(2) << std::hex << bAsInt;
    h += bSs.str();

    json j = { h, imVec4.w };

    return j;
};

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