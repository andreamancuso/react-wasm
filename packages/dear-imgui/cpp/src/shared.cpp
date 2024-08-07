#include <string>
#include <sstream>
#include <iomanip>
#include <yoga/YGEnums.h>
#include <webgpu/webgpu.h>
#include "csscolorparser.hpp"
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

std::optional<ImVec4> HEXAtoIV4(const std::string& hex, float a) {
    auto maybeColor = CSSColorParser::parse(hex);

    if (maybeColor.has_value()) {
        CSSColorParser::Color& color = maybeColor.value();

        return RGBAtoIV4(color.r, color.g, color.b, a);
    }

	return std::nullopt;
}
std::optional<ImVec4> HEXAtoIV4(const std::string& hex) {
	return HEXAtoIV4(hex, 1.0f);
}

std::optional<float> charPercentageToFloat(const char* input) {
    std::optional<float> value;
    float parsedValue;

    if (auto outcome = std::sscanf(input, "%f%%", &parsedValue); outcome != EOF && outcome > 0) {
        value.emplace(parsedValue);
    }

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

std::optional<ImVec4> jsonHEXATupleToIV4(const json& tupleDef) {
    if (!tupleDef.is_array() || tupleDef.size() != 2
        || !tupleDef[0].is_string() || !tupleDef[1].is_number_unsigned()) {
        return std::nullopt;
        }

    const auto color = tupleDef[0].template get<std::string>();
    const auto alpha = tupleDef[1].template get<float>();

    return HEXAtoIV4(color, alpha);
};

std::optional<ImVec4> extractColor(const json& item) {
    if (item.is_string()) {
        return HEXAtoIV4(item.template get<std::string>());
    }

    return jsonHEXATupleToIV4(item);
};

ImDrawFlags cornersToDrawFlags(ImDrawFlags accumulator, const std::string_view side) {
    if (side == "all") {
        accumulator |= ImDrawFlags_RoundCornersAll;
    } else if (side == "topLeft") {
        accumulator |= ImDrawFlags_RoundCornersTopLeft;
    } else if (side == "topRight") {
        accumulator |= ImDrawFlags_RoundCornersTopRight;
    } else if (side == "bottomLeft") {
        accumulator |= ImDrawFlags_RoundCornersBottomLeft;
    } else if (side == "bottomRight") {
        accumulator |= ImDrawFlags_RoundCornersBottomRight;
    }

    return accumulator;
};

std::optional<YGAlign> ResolveAlignItems(std::string_view value) {
    std::optional<YGAlign> alignItems;

    if (value == "auto") {
        alignItems = YGAlignAuto;
    } else if (value == "flex-start") {
        alignItems = YGAlignFlexStart;
    } else if (value == "center") {
        alignItems = YGAlignCenter;
    } else if (value == "flex-end") {
        alignItems = YGAlignFlexEnd;
    } else if (value == "stretch") {
        alignItems = YGAlignStretch;
    } else if (value == "baseline") {
        alignItems = YGAlignBaseline;
    }

    return alignItems;
};

std::optional<YGAlign> ResolveAlignContent(std::string_view value) {
    std::optional<YGAlign> alignContent;

    if (value == "auto") {
        alignContent = YGAlignAuto;
    } else if (value == "flex-start") {
        alignContent = YGAlignFlexStart;
    } else if (value == "center") {
        alignContent = YGAlignCenter;
    } else if (value == "flex-end") {
        alignContent = YGAlignFlexEnd;
    } else if (value == "stretch") {
        alignContent = YGAlignStretch;
    } else if (value == "space-between") {
        alignContent = YGAlignSpaceBetween;
    } else if (value == "space-around") {
        alignContent = YGAlignSpaceAround;
    } else if (value == "space-evenly") {
        alignContent = YGAlignSpaceEvenly;
    }

    return alignContent;
};

std::optional<YGJustify> ResolveJustifyContent(std::string_view value) {
    std::optional<YGJustify> justifyContent;

    if (value == "flex-start") {
        justifyContent = YGJustifyFlexStart;
    } else if (value == "center") {
        justifyContent = YGJustifyCenter;
    } else if (value == "flex-end") {
        justifyContent = YGJustifyFlexEnd;
    } else if (value == "space-between") {
        justifyContent = YGJustifySpaceBetween;
    } else if (value == "space-around") {
        justifyContent = YGJustifySpaceAround;
    } else if (value == "space-evenly") {
        justifyContent = YGJustifySpaceEvenly;
    }

    return justifyContent;
};

std::optional<YGFlexDirection> ResolveFlexDirection(std::string_view value) {
    std::optional<YGFlexDirection> flexDirection;

    if (value == "column") {
        flexDirection = YGFlexDirectionColumn;
    } else if (value == "column-reverse") {
        flexDirection = YGFlexDirectionColumnReverse;
    } else if (value == "row") {
        flexDirection = YGFlexDirectionRow;
    } else if (value == "row-reverse") {
        flexDirection = YGFlexDirectionRowReverse;
    }

    return flexDirection;
};

std::optional<YGDirection> ResolveDirection(std::string_view value) {
    std::optional<YGDirection> direction;

    if (value == "inherit") {
        direction = YGDirectionInherit;
    } else if (value == "ltr") {
        direction = YGDirectionLTR;
    } else if (value == "rtl") {
        direction = YGDirectionRTL;
    }

    return direction;
};

std::optional<YGPositionType> ResolvePositionType(std::string_view value) {
    std::optional<YGPositionType> positionType;

    if (value == "static") {
        positionType = YGPositionTypeStatic;
    } else if (value == "relative") {
        positionType = YGPositionTypeRelative;
    } else if (value == "absolute") {
        positionType = YGPositionTypeAbsolute;
    }

    return positionType;
};

std::optional<YGWrap> ResolveFlexWrap(std::string_view value) {
    std::optional<YGWrap> flexWrap;

    if (value == "no-wrap") {
        flexWrap = YGWrapNoWrap;
    } else if (value == "wrap") {
        flexWrap = YGWrapWrap;
    } else if (value == "wrap-reverse") {
        flexWrap = YGWrapWrapReverse;
    }

    return flexWrap;
};

std::optional<YGOverflow> ResolveOverflow(std::string_view value) {
    std::optional<YGOverflow> overflow;

    if (value == "visible") {
        overflow = YGOverflowVisible;
    } else if (value == "hidden") {
        overflow = YGOverflowHidden;
    } else if (value == "scroll") {
        overflow = YGOverflowScroll;
    }

    return overflow;
};

std::optional<YGDisplay> ResolveDisplay(std::string_view value) {
    std::optional<YGDisplay> display;

    if (value == "flex") {
        display = YGDisplayFlex;
    } else if (value == "none") {
        display = YGDisplayNone;
    }

    return display;
};

std::optional<YGEdge> ResolveEdge(std::string_view edgeKey) {
    std::optional<YGEdge> edge;

    if (edgeKey == "left") {
        edge = YGEdgeLeft;
    } else if (edgeKey == "top") {
        edge = YGEdgeTop;
    } else if (edgeKey == "right") {
        edge = YGEdgeRight;
    } else if (edgeKey == "bottom") {
        edge = YGEdgeBottom;
    } else if (edgeKey == "start") {
        edge = YGEdgeStart;
    } else if (edgeKey == "end") {
        edge = YGEdgeEnd;
    } else if (edgeKey == "horizontal") {
        edge = YGEdgeHorizontal;
    } else if (edgeKey == "vertical") {
        edge = YGEdgeVertical;
    } else if (edgeKey == "all") {
        edge = YGEdgeAll;
    }

    return edge;
}

std::optional<YGGutter> ResolveGutter(std::string_view gutterKey) {
    std::optional<YGGutter> gutter;

    if (gutterKey == "column") {
        gutter = YGGutterColumn;
    } else if (gutterKey == "row") {
        gutter = YGGutterRow;
    } else if (gutterKey == "all") {
        gutter = YGGutterAll;
    }

    return gutter;
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