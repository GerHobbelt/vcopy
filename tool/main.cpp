#include <iostream>
#include <eternal_timestamp/lib1-pub-header.h>
#include <eternal_timestamp/eternal_timestamp.hpp>

int main(int, char **) 
{
  std::cout << "1 + 2 = " << eternaltimestamp::sum(1, 2) << '\n';
  return 0;
}
