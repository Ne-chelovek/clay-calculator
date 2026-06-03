#include "XLSXmanager.h"
#include "ResultValidator.h"

#include <utility>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace xlnt;

XLSXmanager::XLSXmanager(): ws(wb.active_sheet()) {
    ws.column_properties(1).width = 31;
    ws.column_properties(2).width = 28;
    ws.column_properties(3).width = 12;
    ws.column_properties(4).width = 12;
    ws.column_properties(5).width = 28;
}

void XLSXmanager::createTable(const std::vector<Determination>& dets, const std::string& operatorName, const std::string& filename) {
    this->writeHeader(operatorName);
    for (const auto & det : dets) {
        this->writeData(det);
    }
    this->writeFooter(dets);
    wb.save(filename);
}

void XLSXmanager::writeHeader(const std::string& opName) {
    this->ws.cell("A1").value("Clay Content Test Report");
    this->ws.cell("A2").value("GOST 8269.0-97, Paragraph 4.6");

    this->ws.cell("A4").value("Operator");
    this->ws.cell("B4").value(opName);

    time_t now = std::time(nullptr);
    struct tm timeInfo{};
    localtime_s(&timeInfo, &now);
    char dateBuffer[64];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    this->ws.cell("A5").value("Date/Time");
    this->ws.cell("B5").value(dateBuffer);

    this->ws.cell("A7").value("No.");
    this->ws.cell("B7").value("Sample Mass (g)");
    this->ws.cell("C7").value("Clay Mass (g)");
    this->ws.cell("D7").value("Clay Content");
}

void XLSXmanager::writeFooter(const std::vector<Determination>& dets) {
    std::ostringstream formula;
    this->ws.cell(1, 9 + offset).value("Statistics:");

    this->ws.cell(1, 10 + offset).value("Average");
    formula.str("");
    formula << "=SUM(D8:D" << 8 + offset - 1 << ")/" << offset;
    this->ws.cell(2, 10 + offset).formula(formula.str());
    this->ws.cell(2, 10 + offset).number_format(number_format("0.00%"));
    formula.str("");
    formula << "(";
    for (size_t i = 0; i < dets.size() - 1; i++) {
        formula << dets[i].clayContent << "+";
    }
    formula << dets[dets.size() - 1].clayContent << ")/" << offset;
    this->ws.cell(3, 10 + offset).value(formula.str());

    this->ws.cell(1, 11 + offset).value("Minimum");
    formula.str("");
    formula << "=MIN(D8:D" << 8 + offset - 1 << ")";
    this->ws.cell(2, 11 + offset).formula(formula.str());

    this->ws.cell(1, 12 + offset).value("Maximum");
    formula.str("");
    formula << "=MAX(D8:D" << 8 + offset - 1 << ")";
    this->ws.cell(2, 12 + offset).formula(formula.str());

    this->ws.cell(1, 13 + offset).value("Number of determinations");
    this->ws.cell(2, 13 + offset).value(offset);

    this->ws.cell(1, 15 + offset).value("Compliance Status");
    formula.str("");
    formula << "=AND(B" << 10 + offset << ">0, B" << 10 + offset << "<0.05)";
    this->ws.cell(2, 15 + offset).formula(formula.str());


    this->ws.cell(1, 17 + offset).value("GOST 8269.0-97 Standard Information:");

    this->ws.cell(1, 18 + offset).value("Paragraph");
    this->ws.cell(2, 18 + offset).value("4.6 - Determination of clay content");

    this->ws.cell(1, 19 + offset).value("Standart limits");
    this->ws.cell(2, 19 + offset).value("0-5%");

    this->ws.cell(1, 20 + offset).value("Calculation formula");
    this->ws.cell(2, 20 + offset).value("M = m1 / m2 * 100%");
    this->ws.cell(1, 21 + offset).value("where");
    this->ws.cell(1, 22 + offset).value("m1 - mass of clay clumps (g)");
    this->ws.cell(1, 23 + offset).value("m2 - mass of initial sample (g)");
}

void XLSXmanager::writeData(const Determination& det) {
    this->ws.cell(1, 8 + offset).value(offset + 1);
    this->ws.cell(2, 8 + offset).value(det.sampleMass);
    this->ws.cell(3, 8 + offset).value(det.clayMass);
    std::ostringstream formula;
    formula << "=C" << 8 + offset << "/B" << 8 + offset;
    this->ws.cell(4, 8 + offset).formula(formula.str());
    this->ws.cell(4, 8 + offset).number_format(number_format("0.00%"));
    formula.str("");
    formula << std::fixed << std::setprecision(2) << det.clayMass << " / " << det.sampleMass << " * 100% = " << det.clayContent;
    this->ws.cell(5, 8 + offset).value(formula.str());
    this->offset++;
}

std::vector<Determination> XLSXmanager::readTable(const std::string& filename) {
    std::vector<Determination> result;
    workbook rwb = workbook(path(filename));
    worksheet rws = rwb.active_sheet();

    size_t n = 1;
    while (true) {
        if (rws.cell(1, n).value<std::string>() == "Number of determinations") {
            break;
        } else {
            n++;
        }
    }
    size_t detsNum = rws.cell(2, n).value<size_t>();
    for (size_t i = 8; i <= 8 + detsNum - 1; i++) {
        double sm = rws.cell(2, i).value<double>();
        double cm = rws.cell(3, i).value<double>();
        result.emplace_back(sm, cm);
    }

    return result;
}

