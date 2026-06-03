#pragma once
#include "xlnt/xlnt.hpp"
#include "Determination.h"
#include <vector>

class XLSXmanager {
private:
    xlnt::workbook wb;
    xlnt::worksheet ws;
    size_t offset = 0;
    void writeHeader(const std::string& operatorName);
    void writeFooter();
    void writeData(const Determination& det);
public:
    XLSXmanager();
    void createTable(const std::vector<Determination>& dets, const std::string& operatorName, const std::string& filename);
    std::vector<Determination> readTable(const std::string& filename);
};