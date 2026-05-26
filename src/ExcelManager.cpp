#include "ExcelManager.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <xlnt/xlnt.hpp>
using namespace xlnt;


void ExcelManager::writeHeader() {
    // example code from docs
//    workbook wb;
//    worksheet ws = wb.active_sheet();
//    ws.cell("A1").value(5);
//    ws.cell("B2").value("string data");
//    ws.cell("C3").formula("=RAND()");
//    ws.merge_cells("C3:C4");
//    ws.freeze_panes("B2");
//    wb.save("example.xlsx");
}


void ExcelManager::writeData() { }

void ExcelManager::extractData(ClayCalculator &calc) { }