#include "file_reader.hpp"

#include <fstream>

FileReader::FileReader(const std::string filepath) : filepath(filepath) {
  this->inputStream = std::ifstream(filepath);
};

char FileReader::readCharacter() {
  char out;
  this->inputStream.seekg(this->readIndex);
  this->inputStream.read(&out, 1);
  return out;
}

void FileReader::increment() { this->readIndex++; }