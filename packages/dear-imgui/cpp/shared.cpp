#include <string>
#include <optional>
#include "imgui.h"
#include "shared.h"

#pragma once

ImVec4 RGBAtoIV4(int r, int g, int b, float a) {
	float newr = r / 255;
	float newg = g / 255;
	float newb = b / 255;
	return ImVec4(newr, newg, newb, a);
}
ImVec4 RGBAtoIV4(int r, int g, int b) {
	return RGBAtoIV4(r, g, b, 1.0f);
}

ImVec4 HEXAtoIV4(const char* hex, float a) {
	int r, g, b;
	std::sscanf(hex, "%02x%02x%02x", &r, &g, &b);
	return ImVec4(r, g, b, a);
}
ImVec4 HEXAtoIV4(const char* hex) {
	return HEXAtoIV4(hex, 1.0f);
}

json IV4toJson(ImVec4 imVec4) {
    json j = {
        {"w", imVec4.w},
        {"x", imVec4.x},
        {"y", imVec4.y},
        {"z", imVec4.z}
    };

    return j;
};
