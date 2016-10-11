#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>

int main(int argc, char** argv) {
    std::vector<std::string> args;

    if (argc == 1) {
        std::cerr << "Usage: header [namespace] [namespace] ... [class name]" << std::endl;
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    std::string class_name = args.back();

    // Keep only the namespaces in args
    args.pop_back();

    // Produce the file name
    std::string file_name;

    for (const auto c : class_name) {
        if (!file_name.empty()) {
            if (isupper(c)) {
                file_name.push_back('-');
                file_name.push_back(tolower(c));
            } else {
                file_name.push_back(c);
            }
        } else {
            file_name.push_back(tolower(c));
        }
    }

    file_name.append(".h");

    // Produce the header guard
    std::string header_guard;

    for (const auto& ns : args) {
        for (const auto c : ns) {
            header_guard.push_back(toupper(c));
        }
        header_guard.push_back('_');
    }

    header_guard.append(class_name);
    header_guard.append("_H_");

    std::stringstream output;

    output << "#ifndef " << header_guard << std::endl;
    output << "#define " << header_guard << std::endl << std::endl;

    for (const auto& ns : args) {
        output << "namespace " << ns << " {" << std::endl;
    }
    
    output << std::endl;
    output << "class " << class_name << " {" << std::endl;
    output << "};" << std::endl << std::endl;

    for (auto ns_iterator = args.rbegin(); ns_iterator != args.rend(); ++ns_iterator) {
        output << "} // namespace " << *ns_iterator << std::endl;
    }

    output << std::endl;
    output << "#endif // " << header_guard;

    std::cout << "Class name: " << class_name << std::endl;
    std::cout << "File name: " << file_name << std::endl;
    std::cout << "Header guard: " << header_guard << std::endl << std::endl;

    std::cout << output.str() << std::endl;
    std::ofstream file(file_name);
    file << output.str();
}
