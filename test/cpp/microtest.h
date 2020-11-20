//
// microtest.h
//
// URL: https://github.com/torpedro/microtest.h
// Author: Pedro Flemming (http://torpedro.com/)
// License: MIT License (https://github.com/torpedro/microtest.h/blob/master/LICENSE)
// Copyright (c) 2017 Pedro Flemming
//
// This is a small header-only C++ unit testing framework.
// It allows to define small unit tests with set of assertions available.
//
#ifndef __MICROTEST_H__
#define __MICROTEST_H__

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

////////////////
// Assertions //
////////////////

#define ASSERT(cond)\
  ASSERT_TRUE(cond);

#define ASSERT_TRUE(cond)\
  if (!(cond)) throw mt::AssertFailedException(#cond, __FILE__, __LINE__);

#define ASSERT_FALSE(cond)\
  if (cond) throw mt::AssertFailedException(#cond, __FILE__, __LINE__);

#define ASSERT_NULL(value)\
  ASSERT_TRUE(value == NULL);

#define ASSERT_NOTNULL(value)\
  ASSERT_TRUE(value != NULL);

#define ASSERT_STREQ(a, b)\
  if (std::string(a).compare(std::string(b)) != 0) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a << " != " << b << std::endl;\
    throw mt::AssertFailedException(#a " == " #b, __FILE__, __LINE__);\
  }

#define ASSERT_STRNEQ(a, b)\
  if (std::string(a).compare(std::string(b)) !== 0) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a << " == " << b << std::endl;\
    throw mt::AssertFailedException(#a " != " #b, __FILE__, __LINE__);\
  }

#define ASSERT_EQ(a, b)\
  if (a != b) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a << " != " << b << std::endl;\
  }\
  ASSERT(a == b);

#define ASSERT_LE(a, b)\
  if (a > b) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a << " > " << b << std::endl;\
  }\
  ASSERT(a <= b);

#define ASSERT_GE(a, b)\
  if (a < b) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a << " < " << b << std::endl;\
  }\
  ASSERT(a >= b);

#define ASSERT_NEQ(a, b)\
  if (a == b) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a << " == " << b << std::endl;\
  }\
  ASSERT(a != b);

#define ASSERT_NEAR(a, b, eps)\
  if (abs(a - b) > eps) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a << " not near " << b << std::endl;\
  }\
  ASSERT(abs(a - b) < eps);

#define ASSERT_ANY_THROW(stmt) \
    {\
        bool test_caught_any = false; \
        try { \
          stmt; \
        } catch (...) { \
          test_caught_any = true; \
        } \
        if (test_caught_any) {\
            printf("%s{    info} %s", mt::yellow(), mt::def());\
            std::cout << "Code should throw an exception." << std::endl;\
        }\
        ASSERT(test_caught_any);\
    }

////////////////
// Unit Tests //
////////////////

#define TEST(name) \
  void name();\
  namespace {\
    bool __##name = mt::TestsManager::AddTest(name, #name);\
  }\
  void name()

#define TEST_ONLY(name) \
  void name();\
  namespace {\
    bool __##name = mt::TestsManager::AddTestOnly(name, #name);\
  }\
  void name()


///////////////
// Framework //
///////////////

namespace mt {

inline const char* red() {
    return "";
    //return "\033[1;31m";
}

inline const char* green() {
    return "";
    //return "\033[0;32m";
}

inline const char* yellow() {
    return "";
    //return "\033[0;33m";
}

inline const char* def() {
    return "";
    //return "\033[0m";
}

inline void printRunning(const char* message, FILE* file = stdout) {
    fprintf(file, "%s{ running}%s %s\n", green(), def(), message);
}

inline void printOk(const char* message, FILE* file = stdout) {
    fprintf(file, "%s{      ok}%s %s\n", green(), def(), message);
}

inline void printFailed(const char* message, FILE* file = stdout) {
    fprintf(file, "%s{  failed} %s%s\n", red(), message, def());
}

// Exception that is thrown when an assertion fails.
class AssertFailedException : public std::exception {
public:
    AssertFailedException(std::string description, std::string filepath, int line) :
        std::exception(),
        description_(description),
        filepath_(filepath),
        line_(line) {};

    virtual const char* what() const throw() {
        return description_.c_str();
    }

    inline const char* getFilepath() {
        return filepath_.c_str();
    }

    inline int getLine() {
        return line_;
    }

protected:
    std::string description_;
    std::string filepath_;
    int line_;
};

class TestsManager {
    // Note: static initialization fiasco
    // http://www.parashift.com/c++-faq-lite/static-init-order.html
    // http://www.parashift.com/c++-faq-lite/static-init-order-on-first-use.html
public:
    struct Test {
        const char* name;
        void(*fn)(void);
    };

    static std::vector<Test>& tests() {
        static std::vector<Test> tests_;
        return tests_;
    }

    static std::vector<Test>& tests_only() {
        static std::vector<Test> tests_only_;
        return tests_only_;
    }

    // Adds a new test to the current set of tests.
    inline static bool AddTest(void(*fn)(void), const char* name) {
        tests().push_back({ name, fn });
        return true;
    }

    // Adds a new test to the current set of exclusive tests.
    inline static bool AddTestOnly(void(*fn)(void), const char* name) {
        tests_only().push_back({ name, fn });
        return true;
    }

    inline static int GetTestCount() {
        int res = (int)tests_only().size();
        if (res > 0) return res;
        return (int)tests().size();
    }

    static void RunSingleTest(const Test &test, FILE* file, size_t &num_failed) {
        // Run the test.
        // If an AsserFailedException is thrown, the test has failed.
        try {
            printRunning(test.name, file);
            (*test.fn)();
            printOk(test.name, file);
        } catch (AssertFailedException& e) {
            printFailed(test.name, file);
            fprintf(file, "           %sAssertion failed: %s%s\n", red(), e.what(), def());
            fprintf(file, "           %s%s:%d%s\n", red(), e.getFilepath(), e.getLine(), def());
            ++num_failed;
        }
    }

    // Run all tests that are registered.
    // Returns the number of tests that failed.
    inline static size_t RunAllTests(FILE* file = stdout) {
        size_t num_failed = 0;
        if (tests_only().size() > 0) {
            for (const Test& test : tests_only()) {
                RunSingleTest(test, file, num_failed);
            }
        } else {
            for (const Test& test : tests()) {
                RunSingleTest(test, file, num_failed);
            }
        }        

        return num_failed;
        /*int return_code = (num_failed > 0) ? 1 : 0;
        return return_code;*/
    }
};

// Class that will capture the arguments passed to the program.
class Runtime {
public:
    static const std::vector<std::string>& args(int argc = -1, char** argv = NULL) {
        static std::vector<std::string> args_;
        if (argc >= 0) {
            for (int i = 0; i < argc; ++i) {
                args_.push_back(argv[i]);
            }
        }
        return args_;
    }
};
}


#define TEST_MAIN() \
  int main(int argc, char *argv[]) {\
    mt::Runtime::args(argc, argv);\
    \
    size_t num_failed = mt::TestsManager::RunAllTests(stdout);\
    if (num_failed == 0) {\
      fprintf(stdout, "%s{ summary} All tests succeeded!%s\n", mt::green(), mt::def());\
      return 0;\
    } else {\
      double percentage = 100.0 * num_failed / mt::TestsManager::GetTestCount();\
      fprintf(stderr, "%s{ summary} %zu tests failed (%.2f%%)%s\n", mt::red(), num_failed, percentage, mt::def());\
      return -1;\
    }\
  }

///////////////////////////////////////////
// glib custom extensions

#define ASSERT_EQ_TSTR(a, b)\
  if (a != b) {\
    printf("%s{    info} %s", mt::yellow(), mt::def());\
    std::cout << "Actual values: " << a.CStr() << " != " << b.CStr() << std::endl;\
  }\
  ASSERT(a == b);

///////////////////////////////////////////
#endif // __MICROTEST_H__