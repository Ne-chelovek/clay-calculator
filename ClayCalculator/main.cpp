#include <iostream>
#include <iomanip>
#include <string>
#include "ClayCalculator.h"
#include "ResultValidator.h"
#include "ReportGenerator.h"

using namespace std;

void DisplayMenu() {
    cout << "\n========================================\n";
    cout << "    CLAY CONTENT CALCULATOR (GOST)\n";
    cout << "========================================\n";
    cout << "1. Add determination\n";
    cout << "2. Calculate results\n";
    cout << "3. View current data\n";
    cout << "4. Save protocol\n";
    cout << "5. Clear all data\n";
    cout << "0. Exit\n";
    cout << "========================================\n";
    cout << "Choose option: ";
}

void addDetermination(ClayCalculator& calc, ResultValidator& validator) {
    double sampleMass, clayMass;

    cout << "\n--- Add Determination ---\n";
    cout << "Enter sample mass (g): ";
    cin >> sampleMass;

    try { validator.validateSampleMass(sampleMass); }
    catch (string& error) {
        cout << error << endl;
        return;
    }

    cout << "Enter clay mass (g): ";
    cin >> clayMass;

    try { validator.validateClayMass(clayMass, sampleMass); }
    catch(string& error) {
        cout << error << endl;
        return;
    }

    calc.addDetermination(sampleMass, clayMass);
    cout << "Determination added successfully!\n";
}

void CalculateResults(ClayCalculator& calc, ResultValidator& validator) {
    if (!calc.getCount())
    {
        cout << "ERROR: No determinations to calculate!\n";
        return;
    }

    if (!calc.calculateAll())
    {
        cout << "ERROR: Calculation failed!\n";
        return;
    }

    double average = calc.getAverageContent();
    ostringstream limits;
    limits << fixed << setprecision(0) << ResultValidator::getMinLimit() << "-" << ResultValidator::getMaxLimit();
    string range = limits.str();
    string status = validator.isCompliant(average)
                    ? "COMPLIES with standard (" + range + "%)"
                    : "DOES NOT COMPLY with standard (" + range + "%)";

    cout << "\n========== RESULTS ==========\n";
    cout << "Number of determinations: " << calc.getCount() << "\n";
    cout << "Average clay content: " << fixed << setprecision(2) << average << "%\n";
    cout << "Minimum value: " << fixed << setprecision(2) << calc.getMinContent() << "%\n";
    cout << "Maximum value: " << fixed << setprecision(2) << calc.getMaxContent() << "%\n";
    cout << "Status: " << status << "\n";
    cout << "=============================\n";
}

void ViewData(ClayCalculator& calc) {
    if (!calc.getCount()) {
        cout << "No data to display.\n";
        return;
    }

    cout << "\n========== CURRENT DATA ==========\n";
    cout << setw(5) << "No." << setw(20) << "Sample Mass (g)"
        << setw(20) << "Clay Mass (g)" << setw(20) << "Clay Content (%)\n";
    cout << "==================================\n";

    const auto& dets = calc.getDeterminations();
    for (size_t i = 0; i < dets.size(); ++i) {
        cout << setw(5) << (i + 1)
            << setw(20) << fixed << setprecision(2) << dets[i].sampleMass
            << setw(20) << fixed << setprecision(2) << dets[i].clayMass
            << setw(20) << fixed << setprecision(2) << dets[i].clayContent << "\n";
    }
    cout << "==================================\n";
}

void SaveProtocol(ClayCalculator& calc, ResultValidator& validator, ReportGenerator& report) {
    if (!calc.getCount()) {
        cout << "ERROR: No data to save!\n";
        return;
    }

    string operatorName;
    cout << "Enter operator name: ";
    cin.ignore(); // remove \n
    getline(cin, operatorName);

    report.setOperatorName(operatorName);
    report.updateTimestamp();

    double average = calc.getAverageContent();
    ostringstream limits;
    limits << fixed << setprecision(0) << ResultValidator::getMinLimit() << "-" << ResultValidator::getMaxLimit();
    string range = limits.str();
    string status = validator.isCompliant(average)
                    ? "COMPLIES with standard (" + range + "%)"
                    : "DOES NOT COMPLY with standard (" + range + "%)";

    string reportContent = report.generateReport(calc, status);
    string filename = "clay_report_" + to_string(time(nullptr)) + ".txt";

    if (report.saveToFile(filename, reportContent))
    {
        cout << "Protocol saved to: " << filename << "\n";
        cout << "\n" << reportContent;
    }
    else
        cout << "ERROR: Failed to save protocol!\n";
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    ClayCalculator calc;
    ResultValidator validator;
    ReportGenerator report;

    int choice;

    while (true)
    {
        DisplayMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            addDetermination(calc, validator);
            break;
        case 2:
            CalculateResults(calc, validator);
            break;
        case 3:
            ViewData(calc);
            break;
        case 4:
            SaveProtocol(calc, validator, report);
            break;
        case 5:
            calc.clear();
            cout << "All data cleared!\n";
            break;
        case 0:
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid option. Try again.\n";
        }
    }
}