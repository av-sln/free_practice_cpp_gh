#include <gtest/gtest.h>
#include "functions.cpp" // Импортируем наши функции и лямбды



// Тесты для функции split
TEST(SplitTest, EmptyString)
{
   std::string input{};
   std::vector<std::string> expected{};
   std::vector<std::string> actual{split(input, '.')};
   ASSERT_EQ(expected, actual);
}

TEST(SplitTest, SplitByDelimiter)
{
   std::string input{"192.168.1.1"};
   std::vector<std::string> expected{"192", "168", "1", "1"};
   std::vector<std::string> actual{split(input, '.')};
   ASSERT_EQ(expected, actual);
}

TEST(SplitTest, SplitByDelimiterFirstDot)
{
   std::string input{".192.168.1.1"};
   std::vector<std::string> expected{"", "192", "168", "1", "1"};
   std::vector<std::string> actual{split(input, '.')};
   ASSERT_EQ(expected, actual);
}

// Тесты для функции invalidIP
TEST(InvalidIPTest, CorrectIP)
{
   std::vector<std::string> ip{"192", "168", "1", "1"};
   bool expected{false}; // валидный IP
   bool actual{invalidIP(ip)};
   ASSERT_EQ(expected, actual);
}

TEST(InvalidIPTest, IncorrectOctetCount)
{
   std::vector<std::string> ip{"192", "168", "1"};
   bool expected{true}; // невалидный IP, количество октетов не равно 4
   bool actual{invalidIP(ip)};
   ASSERT_EQ(expected, actual);
}

TEST(InvalidIPTest, OutOfRangeFourthOctet)
{
   std::vector<std::string> ip{"192", "168", "1", "256"};
   bool expected{true}; // невалидный IP, выход из диапазона значений 4 октет
   bool actual{invalidIP(ip)};
   ASSERT_EQ(expected, actual);
}

TEST(InvalidIPTest, OutOfRangeFirstOctet)
{
   std::vector<std::string> ip{"-1", "168", "1", "240"};
   bool expected{true}; // невалдиный IP, выход из диапазона значений 1 октет
   bool actual{invalidIP(ip)};
   ASSERT_EQ(expected, actual);
}

TEST(InvalidIPTest, CharSecondOctet)
{
   std::vector<std::string> ip{"192", "f", "1", "245"};
   bool expected{true}; // невалидный IP, буква во 2 октете
   bool actual{invalidIP(ip)};
   ASSERT_EQ(expected, actual);
}

TEST(InvalidIPTest, SymbolThirdOctet)
{
   std::vector<std::string> ip{"192", "168", "*", "245"};
   bool expected{true}; // невалидный IP, символ в 3 октете
   bool actual{invalidIP(ip)};
   ASSERT_EQ(expected, actual);
}

// Тесты для функции compareIP
TEST(CompareIPTest, ReverseLexicographicSort_1)
{
   std::vector<std::string> ip1{"192", "168", "1", "2"};
   std::vector<std::string> ip2{"192", "168", "1", "1"};
   bool expected{true}; // ip1 > ip2
   bool actual{compareIP(ip1, ip2)};
   ASSERT_EQ(expected, actual);
}

TEST(CompareIPTest, ReverseLexicographicSort_2)
{
   std::vector<std::string> ip1{"192", "168", "1", "1"};
   std::vector<std::string> ip2{"203", "168", "1", "1"};
   bool expected{false};   // ip1 < ip2
   bool actual{compareIP(ip1, ip2)};
   ASSERT_EQ(expected, actual);
}

TEST(CompareIPTest, ReverseLexicographicSort_3)
{
   std::vector<std::string> ip1{"192", "168", "1", "1"};
   std::vector<std::string> ip2{"192", "168", "1", "1"};
   bool expected{false};   // ip1 == ip2
   bool actual{compareIP(ip1, ip2)};
   ASSERT_EQ(expected, actual);
}

// Тесты для фильтра first second bytes
TEST(FilterOneTwoTest, MatchBothBytes)
{
   std::vector<std::string> ip{"46", "70", "1", "1"};
   bool expected{true}; // подходит под фильтр
   bool actual{filterOneTwo(ip)};
   ASSERT_EQ(expected, actual);
}

TEST(FilterOneTwoTest, FailBothBytes)
{
   std::vector<std::string> ip{"192", "168", "1", "1"};
   bool expected{false}; // не подходит под фильтр
   bool actual{filterOneTwo(ip)};
   ASSERT_EQ(expected, actual);
}

// Тесты для фильтра any byte equals 46
TEST(AnyByteFilterTest, ByteMatches)
{
   std::vector<std::string> ip{"192", "46", "1", "1"};
   bool expected{true}; // подходит под фильтр
   bool actual{anyByteFilter(ip)};
   ASSERT_EQ(expected, actual);
}

TEST(AnyByteFilterTest, NoMatchingByte)
{
   std::vector<std::string> ip{"192", "168", "1", "1"};
   bool expected{false}; // не подходит под фильтр
   bool actual{anyByteFilter(ip)};
   ASSERT_EQ(expected, actual);
}

// Главная точка входа для запуска тестов
int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}