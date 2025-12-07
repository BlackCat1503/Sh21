#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::string> short_options{"-b", "-e", "-n", "-s",
                                       "-t", "-u", "-v"};

std::vector<std::string> long_options{
    "--show-all", "--number-nonblank", "--show-ends",
    "--number",   "--show-tabs",       "--show-nonprinting",
};

struct ExecResult {
    int exitcode;
    std::string output;

    bool operator==(const ExecResult &o) const {
        return exitcode == o.exitcode && output == o.output;
    }
};

ExecResult exec(const char *cmd) {
    ExecResult result;
    std::array<char, 128> buffer;
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result.output += buffer.data();
    }
    int value = pclose(pipe);
    result.exitcode = WEXITSTATUS(value);
    return result;
}

void CreateInputFile1() {
    std::vector<std::string> test_strings = {
        "[begin 2 empty lines]",
        "",
        "",
        "[end 2 empty lines]",
        "",
        "Hello world!",
        "\tTabbed line",
        "\t\r\fControl characters",
        "     ",   // string with spaces
        "\t\t\t",  // string with tabs
        "[begin 3 empty lines]",
        "",
        "",
        "",
        "[end 3 empty lines]",
        "Текст на русском языке",
        "五大五码",
        "Dvě kamarádky",
        "\u007F\u0080\u011F\u0120\015E\015F",  // nonprinting characters
        "Bye!",
        "[begin 2 empty lines]",
        "",
        "",
        "[end 2 empty lines]",

        // in addition (not standartised behaviour for grep)
        // mixed space characters
        // "\u0020\u0009\u000A\u000C\u000D",
    };

    std::string test_data =
        std::accumulate(begin(test_strings), end(test_strings), std::string{},
                        [](std::string res, const std::string &str) {
                            return res += str + "\n";
                        });

    std::ofstream file("input1.txt");
    file << test_data;
    file.close();
}

void CreateInputFile2() {
    std::ofstream file("input2.txt");
    for (int i = 0; i < 256; ++i) {
        file << char(i) << " ";
    }
    file.close();
}

int TestSingleOptions() {
    std::cout << "\nTest single option:" << std::endl;
    int err = 0;
    for (std::string &opt : short_options) {
        std::string cmd_args =
            std::string(" ") + opt + " input1.txt input2.txt";
        std::string cmd1 = std::string("cat") + cmd_args;
        std::string cmd2 = std::string("./s21_cat") + cmd_args;
        std::cout << std::left << std::setw(70) << std::setfill('.') << cmd2
                  << std::setfill(' ');

        auto res1 = exec(cmd1.c_str());
        auto res2 = exec(cmd2.c_str());
        if (res1 == res2) {
            std::cout << "[Ok]";
        } else {
            std::cout << "[Error:";
            if (res1.exitcode != res2.exitcode)
                std::cout << "wrong exitcode ";
            if (res1.output != res2.output)
                std::cout << "wrong output";
            std::cout << "]";
            ++err;
        }
        std::cout << std::endl;
    }
    std::cout << "Errors: " << err << std::endl;
    return err;
}

void PrintOptions(const std::vector<std::string> &options) {
    for (const auto &opt : options) {
        std::cout << "\t" << opt << "\n";
    }
    std::cout << std::endl;
}

int TestMultiOptions(const std::vector<std::string> &options) {
    std::cout << "\nTest multiple options:" << std::endl;
    PrintOptions(options);
    size_t max_len = std::accumulate(begin(options), end(options), 0,
                                     [](size_t sum, const std::string &opt) {
                                         return sum += opt.length() + 1;
                                     });
    int err = 0;
    int num_opt = options.size();
    for (int i = 0; i < (1 << num_opt); ++i) {
        std::string opt;
        int value = i;
        for (int j = 0; j < num_opt; j++) {
            if (value & 1) {
                if (!options.at(j).empty())
                    opt += options.at(j) + " ";
            }
            value >>= 1;
        }

        std::string cmd_args =
            std::string(" ") + opt + " input1.txt input2.txt";
        std::string cmd1 = std::string("cat") + cmd_args;
        std::string cmd2 = std::string("./s21_cat") + cmd_args;
        std::cout << std::left << std::setw(4) << i << std::setw(max_len + 40)
                  << std::setfill('.') << cmd2 << std::setfill(' ');

        auto res1 = exec(cmd1.c_str());
        auto res2 = exec(cmd2.c_str());
        if (res1 == res2) {
            std::cout << "[Ok]";
        } else {
            std::cout << "[Error:";
            if (res1.exitcode != res2.exitcode)
                std::cout << "wrong exitcode ";
            if (res1.output != res2.output)
                std::cout << "wrong output";
            std::cout << "]";
            ++err;
        }
        std::cout << std::endl;
    }
    std::cout << "Errors: " << err << std::endl;
    return err;
}

void replace_first(std::string *s, std::string const &toReplace,
                   std::string const &replaceWith) {
    std::size_t pos = s->find(toReplace);
    if (pos == std::string::npos)
        return;
    s->replace(pos, toReplace.length(), replaceWith);
}

std::vector<std::string> additionalTests{
    "echo 'Hello world' | ./s21_cat",
    "echo 'Hello world' | ./s21_cat -",
    "echo 'Hello world' | ./s21_cat - input1.txt",
    "echo -e 'line1\\nline2\\nline3' | ./s21_cat",
    "echo -e 'line1\\nline2\\nline3' | ./s21_cat -",
    "echo 'Hello world' | ./s21_cat input1.txt - input2.txt",
    "echo 'Hello world' | ./s21_cat -n input1.txt - input2.txt",
};

int AdditionalTests(const std::vector<std::string> &cmd_args) {
    std::cout << "\nAdditional tests:" << std::endl;
    size_t max_len = 60;

    int err = 0;
    for (size_t i = 0; i < cmd_args.size(); ++i) {
        std::string cmd1 = cmd_args.at(i);

        std::string cmd2 = cmd1;
        replace_first(&cmd2, "./s21_cat", "cat");

        std::cout << std::left << std::setw(4) << i << std::setw(max_len + 40)
                  << std::setfill('.') << cmd1 << std::setfill(' ');

        auto res1 = exec(cmd1.c_str());
        auto res2 = exec(cmd2.c_str());
        if (res1 == res2) {
            std::cout << "[Ok]";
        } else {
            std::cout << "[Error:";
            if (res1.exitcode != res2.exitcode)
                std::cout << "wrong exitcode ";
            if (res1.output != res2.output)
                std::cout << "wrong output";
            std::cout << "]";
            ++err;
        }
        std::cout << std::endl;
    }
    std::cout << "Errors: " << err << std::endl;
    return err;
}

int main() {
    std::cout << "Test s21_cat" << std::endl;
    CreateInputFile1();
    CreateInputFile2();
    size_t err = 0;
    err += TestSingleOptions();
    err += AdditionalTests(additionalTests);
    err += TestMultiOptions(short_options);
#ifdef __unix__
    err += TestMultiOptions(long_options);
#endif

    std::cout << std::endl;
    if (err == 0) {
        std::cout << "[All right]";
    } else {
        std::cout << "[Errors: " << err << "]";
    }
    std::cout << std::endl;
    return 0;
}
