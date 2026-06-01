#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include "ClayCalculator.h"
#include "ResultValidator.h"
#include "ReportGenerator.h"
#include "Exceptions.h"

#pragma comment(lib, "comctl32.lib")

// Константы для идентификаторов элементов управления
#define IDC_BTN_ADD        1001
#define IDC_BTN_CALC       1002
#define IDC_BTN_CLEAR      1003
#define IDC_BTN_SAVE       1004
#define IDC_BTN_EXCEL      1005
#define IDC_EDIT_SAMPLE    1006
#define IDC_EDIT_CLAY      1007
#define IDC_LIST_RESULTS   1008
#define IDC_STATIC_AVG     1009
#define IDC_STATIC_MIN     1010
#define IDC_STATIC_MAX     1011
#define IDC_STATIC_STATUS  1012
#define IDC_EDIT_OPERATOR  1013
#define IDC_BTN_INFO       1014

// Глобальные объекты
ClayCalculator calculator;
ResultValidator validator;
ReportGenerator reportGenerator;
HWND hListResults;
HWND hStaticAvg, hStaticMin, hStaticMax, hStaticStatus;
HWND hEditSample, hEditClay, hEditOperator;

// Прототипы функций
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddDetermination(HWND hwnd);
void CalculateResults();
void UpdateDisplay();
void ClearAll();
void SaveProtocol(HWND hwnd);
void ExportToExcel(HWND hwnd);
void ShowInformation(HWND hwnd);
void AddToListView(const Determination& det, int index);
std::string GetCurrentTimestamp();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    // Регистрация класса окна
    const char CLASS_NAME[] = "ClayCalculatorWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Clay Content Calculator - GOST 8269.0-97",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 750, 550,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Главный цикл сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Кнопка Information (справа сверху)
        CreateWindow("BUTTON", "Information", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            650, 10, 80, 25, hwnd, (HMENU)IDC_BTN_INFO, NULL, NULL);

        // Группа ввода данных
        CreateWindow("STATIC", "Input Data:", WS_CHILD | WS_VISIBLE,
            10, 10, 200, 25, hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "Sample Mass (g):", WS_CHILD | WS_VISIBLE,
            10, 40, 120, 25, hwnd, NULL, NULL, NULL);
        hEditSample = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
            130, 40, 100, 25, hwnd, (HMENU)IDC_EDIT_SAMPLE, NULL, NULL);

        CreateWindow("STATIC", "Clay Mass (g):", WS_CHILD | WS_VISIBLE,
            10, 70, 120, 25, hwnd, NULL, NULL, NULL);
        hEditClay = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
            130, 70, 100, 25, hwnd, (HMENU)IDC_EDIT_CLAY, NULL, NULL);

        // Кнопки
        CreateWindow("BUTTON", "Add Determination", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 105, 150, 30, hwnd, (HMENU)IDC_BTN_ADD, NULL, NULL);

        CreateWindow("BUTTON", "Calculate Results", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            170, 105, 150, 30, hwnd, (HMENU)IDC_BTN_CALC, NULL, NULL);

        CreateWindow("BUTTON", "Clear All", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            330, 105, 100, 30, hwnd, (HMENU)IDC_BTN_CLEAR, NULL, NULL);

        CreateWindow("BUTTON", "Save Protocol", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            440, 105, 110, 30, hwnd, (HMENU)IDC_BTN_SAVE, NULL, NULL);

        CreateWindow("BUTTON", "Export to Excel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            560, 105, 120, 30, hwnd, (HMENU)IDC_BTN_EXCEL, NULL, NULL);

        // Оператор
        CreateWindow("STATIC", "Operator:", WS_CHILD | WS_VISIBLE,
            10, 145, 60, 25, hwnd, NULL, NULL, NULL);
        hEditOperator = CreateWindow("EDIT", "Operator", WS_CHILD | WS_VISIBLE | WS_BORDER,
            80, 145, 200, 25, hwnd, (HMENU)IDC_EDIT_OPERATOR, NULL, NULL);

        // Таблица результатов
        CreateWindow("STATIC", "Results:", WS_CHILD | WS_VISIBLE,
            10, 180, 200, 25, hwnd, NULL, NULL, NULL);

        hListResults = CreateWindow(WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
            10, 205, 720, 200, hwnd, (HMENU)IDC_LIST_RESULTS, NULL, NULL);

        // Настройка колонок таблицы
        LVCOLUMN lvc = {};
        lvc.mask = LVCF_TEXT | LVCF_WIDTH;

        char col1[] = "#";
        lvc.pszText = col1;
        lvc.cx = 40;
        ListView_InsertColumn(hListResults, 0, &lvc);

        char col2[] = "Sample Mass (g)";
        lvc.pszText = col2;
        lvc.cx = 180;
        ListView_InsertColumn(hListResults, 1, &lvc);

        char col3[] = "Clay Mass (g)";
        lvc.pszText = col3;
        lvc.cx = 180;
        ListView_InsertColumn(hListResults, 2, &lvc);

        char col4[] = "Clay Content (%)";
        lvc.pszText = col4;
        lvc.cx = 180;
        ListView_InsertColumn(hListResults, 3, &lvc);

        // Статистика
        CreateWindow("STATIC", "Statistics:", WS_CHILD | WS_VISIBLE,
            10, 420, 200, 25, hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "Average:", WS_CHILD | WS_VISIBLE,
            10, 450, 80, 25, hwnd, NULL, NULL, NULL);
        hStaticAvg = CreateWindow("STATIC", "0.00%", WS_CHILD | WS_VISIBLE | SS_SUNKEN,
            100, 450, 100, 25, hwnd, (HMENU)IDC_STATIC_AVG, NULL, NULL);

        CreateWindow("STATIC", "Minimum:", WS_CHILD | WS_VISIBLE,
            220, 450, 80, 25, hwnd, NULL, NULL, NULL);
        hStaticMin = CreateWindow("STATIC", "0.00%", WS_CHILD | WS_VISIBLE | SS_SUNKEN,
            310, 450, 100, 25, hwnd, (HMENU)IDC_STATIC_MIN, NULL, NULL);

        CreateWindow("STATIC", "Maximum:", WS_CHILD | WS_VISIBLE,
            430, 450, 80, 25, hwnd, NULL, NULL, NULL);
        hStaticMax = CreateWindow("STATIC", "0.00%", WS_CHILD | WS_VISIBLE | SS_SUNKEN,
            520, 450, 100, 25, hwnd, (HMENU)IDC_STATIC_MAX, NULL, NULL);

        CreateWindow("STATIC", "Status:", WS_CHILD | WS_VISIBLE,
            10, 485, 80, 25, hwnd, NULL, NULL, NULL);
        hStaticStatus = CreateWindow("STATIC", "Ready", WS_CHILD | WS_VISIBLE | SS_SUNKEN,
            100, 485, 400, 25, hwnd, (HMENU)IDC_STATIC_STATUS, NULL, NULL);

        // Установка шрифта
        HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Segoe UI");
        SendMessage(hListResults, WM_SETFONT, (WPARAM)hFont, TRUE);

        break;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_BTN_ADD:
            AddDetermination(hwnd);
            break;
        case IDC_BTN_CALC:
            CalculateResults();
            break;
        case IDC_BTN_CLEAR:
            ClearAll();
            break;
        case IDC_BTN_SAVE:
            SaveProtocol(hwnd);
            break;
        case IDC_BTN_EXCEL:
            ExportToExcel(hwnd);
            break;
        case IDC_BTN_INFO:
            ShowInformation(hwnd);
            break;
        }
        break;
    }

    case WM_CLOSE: {
        if (calculator.getCount() > 0) {
            int result = MessageBox(hwnd, "Save data before exit?", "Confirm", MB_YESNOCANCEL);
            if (result == IDYES) {
                SaveProtocol(hwnd);
                DestroyWindow(hwnd);
            }
            else if (result == IDNO) {
                DestroyWindow(hwnd);
            }
            return 0;
        }
        DestroyWindow(hwnd);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AddDetermination(HWND hwnd) {
    char sampleText[32], clayText[32];
    GetWindowText(hEditSample, sampleText, 32);
    GetWindowText(hEditClay, clayText, 32);

    // Проверка на пустые поля
    if (strlen(sampleText) == 0 || strlen(clayText) == 0) {
        MessageBox(hwnd, "Please enter both sample mass and clay mass!", "Input Error", MB_OK | MB_ICONERROR);
        return;
    }

    double sampleMass = atof(sampleText);
    double clayMass = atof(clayText);

    // Проверка на корректный ввод
    if (sampleMass == 0 && sampleText[0] != '0') {
        MessageBox(hwnd, "Invalid sample mass! Please enter a valid number.", "Input Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (clayMass == 0 && clayText[0] != '0') {
        MessageBox(hwnd, "Invalid clay mass! Please enter a valid number.", "Input Error", MB_OK | MB_ICONERROR);
        return;
    }

    try {
        validator.validateSampleMass(sampleMass);
        validator.validateClayMass(clayMass, sampleMass);
    }
    catch (const Exception& ex) {
        MessageBox(hwnd, ex.what(), "Input Error", MB_OK | MB_ICONERROR);
        return;
    }

    calculator.addDetermination(sampleMass, clayMass);
    calculator.calculateAll();

    SetWindowText(hEditSample, "");
    SetWindowText(hEditClay, "");

    UpdateDisplay();

    MessageBox(hwnd, "Determination added successfully!", "Success", MB_OK);
}

void CalculateResults() {
    if (calculator.getCount() == 0) {
        MessageBox(NULL, "No determinations to calculate!", "Warning", MB_OK);
        return;
    }

    calculator.calculateAll();
    UpdateDisplay();
}

void UpdateDisplay() {
    ListView_DeleteAllItems(hListResults);

    const auto& dets = calculator.getDeterminations();
    for (size_t i = 0; i < dets.size(); ++i) {
        AddToListView(dets[i], i + 1);
    }

    if (calculator.getCount() > 0) {
        double avg = calculator.getAverageContent();
        double min = calculator.getMinContent();
        double max = calculator.getMaxContent();

        char buffer[64];
        sprintf_s(buffer, 64, "%.2f%%", avg);
        SetWindowText(hStaticAvg, buffer);

        sprintf_s(buffer, 64, "%.2f%%", min);
        SetWindowText(hStaticMin, buffer);

        sprintf_s(buffer, 64, "%.2f%%", max);
        SetWindowText(hStaticMax, buffer);

        bool compliant = validator.isCompliant(avg);
        if (compliant) {
            SetWindowText(hStaticStatus, "COMPLIES with standard (0-5%)");
        }
        else {
            SetWindowText(hStaticStatus, "DOES NOT COMPLY with standard (0-5%)");
        }
    }
    else {
        SetWindowText(hStaticAvg, "0.00%");
        SetWindowText(hStaticMin, "0.00%");
        SetWindowText(hStaticMax, "0.00%");
        SetWindowText(hStaticStatus, "No data");
    }
}

void AddToListView(const Determination& det, int index) {
    LVITEM lvi = {};
    char buffer[32];

    sprintf_s(buffer, 32, "%d", index);
    lvi.mask = LVIF_TEXT;
    lvi.iItem = index - 1;
    lvi.iSubItem = 0;
    lvi.pszText = buffer;
    ListView_InsertItem(hListResults, &lvi);

    sprintf_s(buffer, 32, "%.2f", det.sampleMass);
    ListView_SetItemText(hListResults, index - 1, 1, buffer);

    sprintf_s(buffer, 32, "%.2f", det.clayMass);
    ListView_SetItemText(hListResults, index - 1, 2, buffer);

    sprintf_s(buffer, 32, "%.2f%%", det.clayContent);
    ListView_SetItemText(hListResults, index - 1, 3, buffer);
}

void ClearAll() {
    if (MessageBox(NULL, "Clear all data?", "Confirm", MB_YESNO) == IDYES) {
        calculator.clear();
        UpdateDisplay();
        MessageBox(NULL, "All data cleared!", "Success", MB_OK);
    }
}

void SaveProtocol(HWND hwnd) {
    if (calculator.getCount() == 0) {
        MessageBox(hwnd, "No data to save!", "Warning", MB_OK);
        return;
    }

    char operatorName[256];
    GetWindowText(hEditOperator, operatorName, 256);

    reportGenerator.setOperatorName(operatorName);
    reportGenerator.updateTimestamp();

    double avg = calculator.getAverageContent();
    std::string status = validator.isCompliant(avg)
        ? "COMPLIES with standard (0-5%)"
        : "DOES NOT COMPLY with standard (0-5%)";

    std::string reportContent = reportGenerator.generateReport(calculator, status);

    time_t now = time(nullptr);
    std::string filename = "clay_report_" + std::to_string(now) + ".txt";

    if (reportGenerator.saveToFile(filename, reportContent)) {
        std::string msg = "Protocol saved to: " + filename;
        MessageBox(hwnd, msg.c_str(), "Success", MB_OK);
    }
    else {
        MessageBox(hwnd, "Failed to save protocol!", "Error", MB_OK | MB_ICONERROR);
    }
}

// Функция для экспорта в Excel (создаёт CSV файл, который открывается в Excel)
void ExportToExcel(HWND hwnd) {
    if (calculator.getCount() == 0) {
        MessageBox(hwnd, "No data to export!", "Warning", MB_OK);
        return;
    }

    try {
        char operatorName[256];
        GetWindowText(hEditOperator, operatorName, 256);

        time_t now = time(nullptr);
        struct tm timeInfo;
        localtime_s(&timeInfo, &now);
        char dateBuffer[64];
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

        std::string filename = "clay_export_" + std::to_string(now) + ".csv";
        std::ofstream file(filename);

        if (!file.is_open()) {
            MessageBox(hwnd, "Failed to create file!", "Error", MB_OK | MB_ICONERROR);
            return;
        }

        // Заголовок отчёта
        file << "Clay Content Test Report\n";
        file << "GOST 8269.0-97, Paragraph 4.6\n";
        file << "\n";
        file << "Operator," << operatorName << "\n";
        file << "Date/Time," << dateBuffer << "\n";
        file << "\n";

        // Таблица с данными
        file << "No.,Sample Mass (g),Clay Mass (g),Clay Content (%)\n";

        const auto& dets = calculator.getDeterminations();
        for (size_t i = 0; i < dets.size(); ++i) {
            file << (i + 1) << ","
                << std::fixed << std::setprecision(2) << dets[i].sampleMass << ","
                << dets[i].clayMass << ","
                << dets[i].clayContent << "\n";
        }

        // Статистика
        file << "\n";
        file << "Statistics:\n";
        file << "Average," << calculator.getAverageContent() << "\n";
        file << "Minimum," << calculator.getMinContent() << "\n";
        file << "Maximum," << calculator.getMaxContent() << "\n";
        file << "Number of determinations," << calculator.getCount() << "\n";

        // Статус соответствия
        file << "\n";
        file << "Compliance Status:,";
        if (validator.isCompliant(calculator.getAverageContent())) {
            file << "COMPLIES with standard (0-5%)\n";
        }
        else {
            file << "DOES NOT COMPLY with standard (0-5%)\n";
        }

        // Информация о стандарте
        file << "\n";
        file << "GOST 8269.0-97 Standard Information:\n";
        file << "Paragraph,4.6 - Determination of clay content\n";
        file << "Standard limits,0% to 5%\n";
        file << "Calculation formula,M = m1 / m2 * 100%\n";
        file << "where,\n";
        file << "m1 - mass of clay clumps (g)\n";
        file << "m2 - mass of initial sample (g)\n";

        file.close();

        std::string msg = "Data exported to: " + filename +
            "\n\nThis is a CSV file that can be opened in Excel.\n"
            "The file contains all measurements, statistics, and GOST information.";
        MessageBox(hwnd, msg.c_str(), "Export Complete", MB_OK);

    }
    catch (const std::exception& e) {
        std::string errorMsg = "Failed to export: " + std::string(e.what());
        MessageBox(hwnd, errorMsg.c_str(), "Export Error", MB_OK | MB_ICONERROR);
    }
}

// Функция для отображения информационного окна
void ShowInformation(HWND hwnd) {
    std::string infoText =
        "Project Information\n\n"
        "Project completed according to GOST 8269.0, clause 4.6\n\n"
        "Calculation formula: M = m1 / m2 * 100%\n\n"
        "where:\n"
        "  m1 - mass of clay clumps (g)\n"
        "  m2 - mass of initial sample (g)\n\n"
        "Standard limits: 0% to 5%\n\n"
        "Developed for laboratory testing of crushed stone\n"
        "and gravel for clay content determination.";

    MessageBox(hwnd, infoText.c_str(), "Information", MB_OK | MB_ICONINFORMATION);
}