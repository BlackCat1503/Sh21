#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<std::string> short_options{"-i", "-s", "-v", "-n",
                                       "-h", "-o", "-l", "-c"};

std::vector<std::string> long_options{
    "--ignore-case", "--no-messages",   "--invert-match",       "--line-number",
    "--no-filename", "--only-matching", "--files-with-matches", "--count",
};

void CreateInputFile1() {
    std::vector<std::string> test_strings = {
        "",
        "12344",
        "Hellooo wrld",
        "hello 123",
        "2/4 == 1/2",
        "Привет, агент 007!",
        "The ABBA is very famous! abba...",
        "Мадам, эту мадам я вам не отдам. Sooory, madam!",
        "1 one 2 two 3 three 4",
    };

    std::string test_data =
        std::accumulate(begin(test_strings), end(test_strings), std::string{},
                        [](std::string res, const std::string &str) {
                            return res += str + "\n";
                        });

    std::ofstream file("input1.txt");
    file << test_data;

    file << std::endl;
    std::transform(test_data.begin(), test_data.end(), test_data.begin(),
                   ::toupper);
    file << test_data;
    file.close();
}

void CreateInputFile2() {
    std::ofstream file("input2.txt");
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < rand() % 128; ++j) {
            file << char(' ' + rand() % (127 - ' '));
        }
        file << std::endl;
    }
    file.close();
}

struct ExecResult {
    int exitcode;
    std::string output;

    bool operator==(const ExecResult &o) const {
        return exitcode == o.exitcode && output == o.output;
    }
    bool operator!=(const ExecResult &o) const { return !(*this == o); }
};

ExecResult exec(const char *cmd) {
    ExecResult result;
    std::array<char, 128> buffer;
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != NULL) {
        result.output += buffer.data();
    }
    int value = pclose(pipe);
    result.exitcode = WEXITSTATUS(value);
    return result;
}

/*
ExecResult exec2(const char *cmd) {
    auto res1 = exec(cmd);
    auto res2 = exec(cmd);
    if (res1 != res2) {
        std::cout << "Bad exec2" << std::endl;
    }
    return res1;
}
*/

void LogOutputToFile(std::string dirname, std::string label,
                     const ExecResult &res) {
    std::string filename =
        dirname + "/" + label + "_ret_" + std::to_string(res.exitcode) + ".txt";
    std::cout << std::endl;
    std::cout << "Create log file: " << filename << std::endl;
    std::ofstream file(filename);
    file << res.output;
    file.close();
}

void LogError(const ExecResult &res1, const ExecResult &res2) {
    static size_t err_counter = 0;
    ++err_counter;
    std::string dirname =
        std::string("./tests/errors/err") + std::to_string(err_counter);
    fs::create_directories(dirname);
    LogOutputToFile(dirname, "1", res1);
    LogOutputToFile(dirname, "2", res2);
}

int CheckResults(const ExecResult &res1, const ExecResult &res2) {
    int err = 0;
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
        LogError(res1, res2);
    }
    std::cout << std::endl;
    return err;
}

std::vector<std::string> patterns{
    "[a-z]",
    "[A-Z]",
    "[[:alnum:]]",
    "[[:digit:]]",
    " [[:digit:]]",
    "[[:digit:]]\\{2\\}",
    "[how]",
    "[^how]",
    "^.",
    ".$",
    "o\\*",
    "o\\{2\\}",
    "a\\{3,\\}",
    "a\\{3,5\\}",
    "\\(ol\\)*",
    "\\[.\\]",
    "\\[.\\*\\]",
    "\\([a-z]\\)\\1",                    // ll, oo, ee, ss
    "\\([a-z]\\)\\([a-z]\\)\\2\\1",      // abba, foof
    "\\([a-z]\\)\\([a-z]\\)[a-z]\\2\\1"  // madam, abcbc
};

int TestPatternWithSingleOption(const std::string &pattern,
                                const std::string &option) {
    std::string cmd_args = std::string(" \"") + pattern + "\"" +
                           " input1.txt " + option + " input2.txt";

    std::string cmd1 = std::string("grep") + cmd_args;
    std::string cmd2 = std::string("./s21_grep") + cmd_args;
    std::cout << std::left << std::setw(100) << std::setfill('.') << cmd2
              << std::setfill(' ');
    std::cout.flush();
    auto res1 = exec(cmd1.c_str());
    auto res2 = exec(cmd2.c_str());
    return CheckResults(res1, res2);
}

void PrintOptions(const std::vector<std::string> &options) {
    for (const auto &opt : options) {
        std::cout << "\t" << opt << "\n";
    }
    std::cout << std::endl;
}

int TestMultiOptions(const std::string &extended_patterns,
                     const std::string &pattern,
                     const std::vector<std::string> &options) {
    std::cout << "\nTest multiple options with pattern '" << pattern
              << "':" << std::endl;
    PrintOptions(options);
    size_t max_len = std::accumulate(begin(options), end(options), 0,
                                     [](size_t sum, const std::string &opt) {
                                         return sum += opt.length() + 1;
                                     });

    max_len += extended_patterns.length() + pattern.length();

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

        std::string cmd_args = extended_patterns;
        if (extended_patterns.empty()) {
            cmd_args = std::string(" \"") + pattern + "\"";
        }
        cmd_args += " input1.txt " + opt + " input2.txt";

        std::string cmd1 = std::string("grep") + cmd_args;
        std::string cmd2 = std::string("./s21_grep") + cmd_args;
        std::cout << std::left << std::setw(4) << i << std::setw(max_len + 60)
                  << std::setfill('.') << cmd2 << std::setfill(' ');

        auto res1 = exec(cmd1.c_str());
        auto res2 = exec(cmd2.c_str());
        err += CheckResults(res1, res2);
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

std::vector<std::string> additionalTests1{
    "echo 'Foo bOO zoooz' | ./s21_grep \"o\"",
    "echo 'Foo bOO zoooz' | ./s21_grep \"o\" -",
    "echo 'Foo bOO zoooz' | ./s21_grep \"oo\" -",
    "echo -e 'Foo\\n bOO\\n zoooz\\n' | ./s21_grep \"oo\"",
    "echo -e 'Foo\\n bOO\\n zoooz\\n' | ./s21_grep \"oo\" -i",
    "echo -e 'Бла\\n Бла бла\\n БЛА бла бла\\n' | ./s21_grep \"бла\" -i",
};

int AdditionalTests(const std::vector<std::string> &cmd_args,
                    const std::string &name) {
    std::cout << "\nAdditional tests '" << name << "':" << std::endl;
    size_t max_len = 60;

    int err = 0;
    for (size_t i = 0; i < cmd_args.size(); ++i) {
        std::string cmd1 = cmd_args.at(i);

        std::string cmd2 = cmd1;
        replace_first(&cmd2, "./s21_grep", "grep");

        std::cout << std::left << std::setw(4) << i << std::setw(max_len + 60)
                  << std::setfill('.') << cmd1 << std::setfill(' ');

        auto res1 = exec(cmd1.c_str());
        auto res2 = exec(cmd2.c_str());
        err += CheckResults(res1, res2);
    }
    std::cout << "Errors: " << err << std::endl;
    return err;
}

size_t TestSingleOption(const std::vector<std::string> &options) {
    std::cout << "\nTest single pattern with single option:" << std::endl;
    size_t err = 0;
    for (auto &pattern : patterns) {
        err += std::accumulate(options.begin(), options.end(), 0,
                               [&](size_t err, const std::string &opt) {
                                   return err + TestPatternWithSingleOption(
                                                    pattern, opt);
                               });
    }
    return err;
}

size_t TestManyOptions(const std::vector<std::string> &patterns,
                       const std::vector<std::string> &options) {
    size_t err =
        std::accumulate(patterns.begin(), patterns.end(), 0,
                        [&](size_t err, const std::string &pattern) {
                            return err + TestMultiOptions("", pattern, options);
                        });
    return err;
}

void CreatePatternsFile(const std::string &filename,
                        std::vector<std::string> pattern_in_file) {
    std::string content = std::accumulate(
        begin(pattern_in_file), end(pattern_in_file), std::string{},
        [](std::string res, const std::string &str) {
            return res += str + "\n";
        });

    std::ofstream file(filename);
    file << content;
    file.close();
}

size_t TestManyPatternsAndManyOptions(const std::vector<std::string> &options) {
    CreatePatternsFile("patterns1.txt", {"[o|e]", "[[:digit:]]"});
    std::string extended_patterns =
        " -e \"X\" -e \"y\" -e \"[I-K]\" -f patterns1.txt";
    size_t err = 0;
    err += TestMultiOptions(extended_patterns, "", options);
    return err;
}

int main() {
    setlocale(LC_ALL, "");
    std::cout << "Test s21_cat" << std::endl;
    size_t err = 0;
    CreateInputFile1();
    CreateInputFile2();

    fs::remove_all("./tests/errors");
    err += AdditionalTests(additionalTests1, "read from stdin");
    if (!err)
        err += TestSingleOption(short_options);
    if (!err)
        err += TestSingleOption(long_options);

    std::vector<std::string> patterns2 = {
        "",
        "^[[:alnum:]]*$",
        "[[:digit:]]",
        "o\\*",
        "\\([a-z]\\)\\1",                    // ll, oo, ee, ss
        "\\([a-z]\\)\\([a-z]\\)\\2\\1",      // abba, foof
        "\\([a-z]\\)\\([a-z]\\)[a-z]\\2\\1"  // madam, abcbc
    };
    if (!err)
        err += TestManyOptions(patterns2, short_options);
    if (!err)
        err += TestManyOptions(patterns2, long_options);
    if (!err)
        err += TestManyPatternsAndManyOptions(short_options);

    std::cout << std::endl;
    if (err == 0) {
        std::cout << "[All right]";
    } else {
        std::cout << "[Errors: " << err << "]";
    }
    std::cout << std::endl;
    return 0;
}
