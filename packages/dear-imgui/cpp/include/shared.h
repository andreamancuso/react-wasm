#include <string>
#include <variant>
#include <nlohmann/json.hpp>
#include <webgpu/webgpu.h>
#include <yoga/YGEnums.h>

#include "csscolorparser.hpp"
#include "imgui.h"

#pragma once

#ifndef SHARED_STUFF
#define SHARED_STUFF

using json = nlohmann::json;

// https://www.cppstories.com/2021/heterogeneous-access-cpp20/
struct StringHash {
    using is_transparent = void;
    [[nodiscard]] size_t operator()(const char *txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(std::string_view txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(const std::string &txt) const {
        return std::hash<std::string>{}(txt);
    }
};

enum HorizontalAlignment
{
    // HorizontalAlignment_None    = -1,
    HorizontalAlignment_Left    = 0,
    HorizontalAlignment_Right   = 1,
    HorizontalAlignment_Center  = 2 // Wondering this will ever be supported the way I'd like
};

using TableRow = std::unordered_map<std::string, std::string>;
using TableData = std::vector<TableRow>;

using StyleVarValue = std::variant<std::monostate, ImVec2, float>;
using StyleVarValueRef = std::variant<std::monostate, const ImVec2*, const float*>;
using StyleColors = std::unordered_map<ImGuiCol, ImVec4>;
using StyleVars = std::unordered_map<ImGuiStyleVar, StyleVarValue>;

using HEXA = std::tuple<std::string, float>;

using OnInitCallback = void (*)();
using OnTextChangedCallback = void (*)(int id, const std::string& value);
using OnComboChangedCallback = void (*)(int id, int value);
using OnNumericValueChangedCallback = void (*)(int id, float value);
using OnBooleanValueChangedCallback = void (*)(int id, bool value);
using OnMultipleNumericValuesChangedCallback = void (*)(int id, const float* values, int size);
using OnClickCallback = void (*)(int id);

ImVec4 RGBAtoIV4(int r, int g, int b, float a);
ImVec4 RGBAtoIV4(int r, int g, int b);

std::optional<ImVec4> HEXAtoIV4(const std::string& hex, float a);
std::optional<ImVec4> HEXAtoIV4(const std::string& hex);

std::optional<float> charPercentageToFloat(const char* input);

json IV4toJson(const ImVec4& imVec4);
json IV4toJsonTuple(const ImVec4& imVec4);
json IV4toJsonRGBATuple(const ImVec4& imVec4);
json IV4toJsonHEXATuple(const ImVec4& imVec4);

CSSColorParser::Color IV4toCSSColor(const ImVec4& imVec4);
HEXA IV4toHEXATuple(const ImVec4& imVec4);

std::optional<ImVec4> jsonHEXATupleToIV4(const json& tupleDef);

std::optional<ImVec4> extractColor(const json& colorDef);

ImDrawFlags cornersToDrawFlags(ImDrawFlags accumulator, std::string_view side);

struct Texture {
    WGPUTextureView textureView;
    int width;
    int height;
};

std::optional<YGAlign> ResolveAlignItems(std::string_view value);
std::optional<YGAlign> ResolveAlignContent(std::string_view value);
std::optional<YGJustify> ResolveJustifyContent(std::string_view value);
std::optional<YGFlexDirection> ResolveFlexDirection(std::string_view value);
std::optional<YGDirection> ResolveDirection(std::string_view value);
std::optional<YGPositionType> ResolvePositionType(std::string_view value);
std::optional<YGWrap> ResolveFlexWrap(std::string_view value);
std::optional<YGOverflow> ResolveOverflow(std::string_view value);
std::optional<YGDisplay> ResolveDisplay(std::string_view value);
std::optional<YGEdge> ResolveEdge(std::string_view edgeKey);
std::optional<YGGutter> ResolveGutter(std::string_view gutterKey);

// Base class for type-erased setters
struct IVariadicFn {
    virtual ~IVariadicFn() = default;
    virtual void call(void* args[]) = 0;
};

// Templated derived class for variadic setters
template <typename... Args>
class VariadicFnImpl : public IVariadicFn {
    std::function<void(Args...)> setter;

public:
    explicit VariadicFnImpl(std::function<void(Args...)> s) : setter(std::move(s)) {}

    void call(void* args[]) override {
        // Use std::apply to call the setter with unpacked arguments
        callImpl(std::index_sequence_for<Args...>{}, args);
    }

private:
    template <std::size_t... I>
    void callImpl(std::index_sequence<I...>, void* args[]) {
        setter(*static_cast<Args*>(args[I])...);
    }
};

bool LoadTexture(WGPUDevice device, const void* data, int numBytes, Texture* texture);

#endif
