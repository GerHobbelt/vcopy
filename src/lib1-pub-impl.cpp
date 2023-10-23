#include "lib1-priv-header.h"

namespace eternaltimestamp 
{

auto sum(const int first, const int second) -> int 
{
  return sum_aux(first, second);
}

}