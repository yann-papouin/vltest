#ifndef SPLIT_H_H_H
#define SPLIT_H_H_H

#include <vector>
#include <string>

void split(const std::string& s,
							   const std::string& delim,
							   std::vector<std::string>* ret);
#endif