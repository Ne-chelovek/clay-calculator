#pragma once

#include "ClayCalculator.h"

class ExcelManager {
private:
    static void writeHeader();
public:
    ExcelManager() = delete;
    ~ExcelManager() = delete;
    static void writeData();
    static void extractData(ClayCalculator& calc);
};