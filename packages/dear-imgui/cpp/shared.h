#include <string>

#pragma once

typedef void (*OnTextChangedCallback)(int id, std::string value);
typedef void (*OnComboChangedCallback)(int id, int value);
typedef void (*OnNumericValueChangedCallback)(int id, int value);
typedef void (*OnBooleanValueChangedCallback)(int id, bool value);
typedef void (*OnMultipleNumericValuesChangedCallback)(int id, float* values, int size);
typedef void (*OnClickCallback)(int id);

