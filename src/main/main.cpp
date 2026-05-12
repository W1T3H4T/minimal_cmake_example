// main.cpp
// This is a no-nonsense main entry point for the application. It demonstrates how to read a resource 
// file and print its contents to the console.

#include <iostream>
#include <fstream>
#include <string>
#include "fs.h"
#include "frobnication.h"

int main(int argc, char** argv) {
  auto resource_path = fs::absolute(argv[0]).parent_path() / "resources";

  std::fstream resource1((resource_path / "resource1").string());
  std::string line;

  while(resource1) {
    std::getline(resource1, line);
    std::cout << line << std::endl;
  }
  return EXIT_SUCCESS;
}

