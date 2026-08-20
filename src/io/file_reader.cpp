#include "file_reader.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "io/program_text.hpp"

FileReader::FileReader(const std::string& filepath) : filepath(filepath) {
  this->inputStream = std::ifstream(filepath);
};

bool FileReader::isOpen() const { return this->inputStream.is_open(); };

std::string FileReader::readAll() {
  std::stringstream out;
  out << this->inputStream.rdbuf();
  this->inputStream.seekg(0);
  return out.str();
};

char FileReader::readCharacter() {
  char out;
  this->inputStream.seekg(this->readIndex);
  this->inputStream.read(&out, 1);
  return out;
}

void FileReader::increment() { this->readIndex++; }

ProgramText FileReader::toProgramText() {
  ProgramText programText = ProgramText();

  std::string allText = this->readAll();

  std::string buffer = "";
  for (size_t i = 0; i < allText.size(); i++) {
    char c = allText[i];
    if (c == '\n') {
      programText.addLine(buffer);
      buffer = "";
      continue;
    }

    buffer += c;
  }

  if (buffer.size() != 0) {
    programText.addLine(buffer);
  }

  return programText;
};