#include "file_reader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

FileReader::FileReader(const std::string filepath) : filepath(filepath) {
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