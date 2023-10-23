#include <iostream>
#include <eternal_timestamp/lib1-pub-header.h>

int main(int, char **) 
{
  std::cout << "Running tests...\n";
  return 42 == eternaltimestamp::sum(40, 2) ? 0 : 1;
}
