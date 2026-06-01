#include <gtest/gtest.h>
#include "ClayCalculator.h"
#include "ResultValidator.h"
#include "ReportGenerator.h"
#include "Exceptions.h"
#include <fstream>
#include <cstdio>
#include <string>

// =======================
// ClayCalculator Tests
// =======================
class ClayCalculatorTest : public ::testing::Test {
protected:
    ClayCalculator calc;
};

TEST_F(ClayCalculatorTest, InitiallyEmpty) {
EXPECT_EQ(calc.getCount(), 0);
EXPECT_DOUBLE_EQ(calc.getAverageContent(), 0.0);
}

TEST_F(ClayCalculatorTest, AddDetermination) {
calc.addDetermination(100.0, 5.0);
EXPECT_EQ(calc.getCount(), 1);
}

TEST_F(ClayCalculatorTest, CalculateAll_Empty) {
EXPECT_FALSE(calc.calculateAll());
}

TEST_F(ClayCalculatorTest, CalculateAll_InvalidSampleMass) {
calc.addDetermination(100.0, 5.0);
calc.addDetermination(0.0, 5.0); // Неверная масса пробы
EXPECT_FALSE(calc.calculateAll());
}

TEST_F(ClayCalculatorTest, CalculateAll_Success) {
calc.addDetermination(100.0, 5.0);  // 5%
calc.addDetermination(100.0, 3.0);  // 3%
calc.addDetermination(100.0, 4.0);  // 4%

EXPECT_TRUE(calc.calculateAll());

EXPECT_DOUBLE_EQ(calc.getClayContent(0), 5.0);
EXPECT_DOUBLE_EQ(calc.getClayContent(1), 3.0);
EXPECT_DOUBLE_EQ(calc.getClayContent(2), 4.0);

EXPECT_DOUBLE_EQ(calc.getAverageContent(), 4.0);
EXPECT_DOUBLE_EQ(calc.getMinContent(), 3.0);
EXPECT_DOUBLE_EQ(calc.getMaxContent(), 5.0);
}

TEST_F(ClayCalculatorTest, GetDeterminations) {
calc.addDetermination(100.0, 5.0);
const auto& dets = calc.getDeterminations();
EXPECT_EQ(dets.size(), 1);
EXPECT_DOUBLE_EQ(dets[0].sampleMass, 100.0);
}

TEST_F(ClayCalculatorTest, ClearMethod) {
calc.addDetermination(100.0, 5.0);
calc.calculateAll();
calc.clear();

EXPECT_EQ(calc.getCount(), 0);
EXPECT_DOUBLE_EQ(calc.getAverageContent(), 0.0);
}

// =======================
// ResultValidator Tests
// =======================

class ResultValidatorTest : public ::testing::Test {
protected:
    ResultValidator validator;
};

TEST_F(ResultValidatorTest, ValidateSampleMass_Valid) {
EXPECT_NO_THROW(validator.validateSampleMass(100.0));
}

TEST_F(ResultValidatorTest, ValidateSampleMass_Zero) {
EXPECT_THROW(validator.validateSampleMass(0.0), NegativeMassException);
}

TEST_F(ResultValidatorTest, ValidateSampleMass_Negative) {
EXPECT_THROW(validator.validateSampleMass(-10.0), NegativeMassException);
}

TEST_F(ResultValidatorTest, ValidateClayMass_Valid) {
EXPECT_NO_THROW(validator.validateClayMass(5.0, 100.0));
EXPECT_NO_THROW(validator.validateClayMass(0.0, 100.0));
EXPECT_NO_THROW(validator.validateClayMass(100.0, 100.0));
}

TEST_F(ResultValidatorTest, ValidateClayMass_Negative) {
EXPECT_THROW(validator.validateClayMass(-1.0, 100.0), NegativeMassException);
}

TEST_F(ResultValidatorTest, ValidateClayMass_ExceedsSample) {
EXPECT_THROW(validator.validateClayMass(101.0, 100.0), MassExceedException);
}

TEST_F(ResultValidatorTest, IsCompliant_InLimits) {
EXPECT_TRUE(validator.isCompliant(0.0));
EXPECT_TRUE(validator.isCompliant(2.5));
EXPECT_TRUE(validator.isCompliant(5.0));
}

TEST_F(ResultValidatorTest, IsCompliant_OutOfLimits) {
EXPECT_FALSE(validator.isCompliant(-0.1));
EXPECT_FALSE(validator.isCompliant(5.1));
}

// =======================
// ReportGenerator Tests
// =======================

class ReportGeneratorTest : public ::testing::Test {
protected:
    ReportGenerator generator;
    ClayCalculator calc;

    void SetUp() override {
        generator.setOperatorName("Test Operator");
        calc.addDetermination(100.0, 3.0);
        calc.calculateAll();
    }
};

TEST_F(ReportGeneratorTest, GenerateReport_ContainsHeaders) {
std::string report = generator.generateReport(calc, "Compliant");
EXPECT_TRUE(report.find("CLAY CONTENT TEST PROTOCOL") != std::string::npos);
EXPECT_TRUE(report.find("Test Operator") != std::string::npos);
EXPECT_TRUE(report.find("Compliant") != std::string::npos);
}

TEST_F(ReportGeneratorTest, SaveToFile_Success) {
std::string report = generator.generateReport(calc, "Compliant");
std::string filename = "test_report.txt";

EXPECT_TRUE(generator.saveToFile(filename, report));

std::ifstream file(filename);
EXPECT_TRUE(file.is_open());
file.close();

std::remove(filename.c_str()); // Очистка после теста
}

TEST_F(ReportGeneratorTest, SaveToFile_InvalidPath) {
std::string report = generator.generateReport(calc, "Compliant");
// Попытка сохранить в несуществующую директорию
std::string invalidFilename = "/invalid_directory_for_tests/test_report.txt";
EXPECT_FALSE(generator.saveToFile(invalidFilename, report));
}