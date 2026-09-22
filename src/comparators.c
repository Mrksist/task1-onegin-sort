#include <assert.h>
#include <debug52.h>
#include <ctype.h>

#include "onegin.h"

int NormalComparator (const void* a_ptr, const void* b_ptr) {
  assert (a_ptr != 0);
  assert (b_ptr != 0);

  const OneginLine line_a = *(const OneginLine*)a_ptr;
  const OneginLine line_b = *(const OneginLine*)b_ptr;

  const char* lhs = line_a.line_pointer;
  const char* rhs = line_b.line_pointer;

  assert (lhs != 0);
  assert (rhs != 0);

  assert (line_a.line_size > 0);
  assert (line_b.line_size > 0);

  int diff = 0;
  while (diff == 0 && (*lhs != '\0' && *rhs != '\0')) {
    while (*lhs != '\0' && !isalpha (*lhs)) lhs++;
    while (*rhs != '\0' && !isalpha (*rhs)) rhs++;
      
    diff = tolower (*lhs) - tolower (*rhs);
    lhs++; rhs++;
  }

  if (*lhs == '\0' && *rhs == '\0')
    return 0;
  if (*lhs == '\0' && *rhs != '\0')
    return -1;
  if (*lhs != '\0' && *rhs == '\0')
    return 1;

  return diff;
}

int InverseComparator (const void* a_ptr, const void* b_ptr) {
  assert (a_ptr != 0);
  assert (b_ptr != 0);

  const OneginLine line_a = *(const OneginLine*)a_ptr;
  const OneginLine line_b = *(const OneginLine*)b_ptr;

  const char* lhs = line_a.line_pointer;
  const char* rhs = line_b.line_pointer;

  assert (lhs != 0);
  assert (rhs != 0);

  int l_ptr = (int)line_a.line_size - 2;
  int r_ptr = (int)line_b.line_size - 2;

  assert (line_a.line_size > 0);
  assert (line_b.line_size > 0);

  int diff = 0;

  while (diff == 0 && (l_ptr != -1 && r_ptr != -1)) {
    while (l_ptr != -1 && !isalpha(lhs[l_ptr])) l_ptr--;
    while (r_ptr != -1 && !isalpha(rhs[r_ptr])) r_ptr--;

    assert (l_ptr >= -1);
    assert (r_ptr >= -1);

    if (l_ptr == -1 && r_ptr == -1)
      break;

    if (l_ptr == -1 && r_ptr != -1 && diff == 0) {
      diff = -tolower (rhs[r_ptr]);
      break;
    }
    if (l_ptr != -1 && r_ptr == -1 && diff == 0) {
      diff = tolower (lhs[l_ptr]);
      break;
    }
     
    if (l_ptr != -1 && r_ptr != -1) {
      diff = tolower(lhs[l_ptr]) - tolower(rhs[r_ptr]);
      l_ptr--; r_ptr--;
    }
  }

  if (l_ptr == -1 && r_ptr == -1 && diff == 0) {
    return 0;
  }
  if (l_ptr == -1 && r_ptr != -1 && diff == 0) {
    return -1;
  }
  if (l_ptr != -1 && r_ptr == -1 && diff == 0){
    return 1;
  }

  return diff;
}