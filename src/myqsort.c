#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <debug52.h>

#include "onegin.h"

#pragma GCC diagnostic ignored "-Wpointer-arith"
void ExchangeElements (void* a, void* b, size_t SzElem) {
    assert (a != 0);
    assert (b != 0);
    assert (SzElem != 0);

    uint8_t buf = 0;

    for (size_t offset = 0; offset < SzElem; offset++) {
        buf = *(uint8_t*)(a + offset);
        *(uint8_t*)(a + offset) = *(uint8_t*)(b + offset);
        *(uint8_t*)(b + offset) = buf;
    }
}
#pragma GCC diagnostic warning "-Wpointer-arith"


#pragma GCC diagnostic ignored "-Wpointer-arith"
unsigned Divide (void* data, unsigned left, unsigned right, size_t SzElem, int (*CompareElements)(const void* a, const void* b)) {
    assert (data != 0);
    assert (SzElem != 0);
    assert (CompareElements != 0);
    assert (right - left + 1 > 0);
    assert ((uintptr_t)data != 0);

#if defined(ONEGIN_MYQSORT_SELECT_CENTRE_AS_MIDDLE)
    unsigned middle = left + (right + left - 1) / 2;
#else
    unsigned middle = left + ((unsigned)rand() % (right - left + 1));
#endif

    while (left <= right) {
        while (left < right && CompareElements (data + left * SzElem, data + middle * SzElem) < 0) {
            left++;
            assert (left <= right);
        }
        while (left < right && CompareElements (data + right * SzElem, data + middle * SzElem) > 0) {
            right--;
            assert (left <= right);
        }

        if (left == right) {
            break;
        }
        assert (left < right);
    
        ExchangeElements (data + left * SzElem, data + right * SzElem, SzElem);

        if (middle == left) {
            middle = right;
        } else if (middle == right) {
            middle = left;
        }

        if(right == left + 1) {
            right--;
        }
        else if (left < right) {
            left++;
            right--;
        }
        
        assert (left <= right);
    }

    assert (left == right);
    return (CompareElements (data + left * SzElem, data + middle * SzElem) > 0 ? left - 1 : right);
}
#pragma GCC diagnostic warning "-Wpointer-arith"


void MyQuickSort (void* data, size_t size, unsigned left, unsigned right, size_t SzElem,
                    int (*CompareElements) (const void* a, const void* b)) {
    if (left < right) {
        unsigned border = Divide (data, left, right, SzElem, CompareElements);

        assert(left < size);
        assert(right < size);
        assert(border < size);

        if (right - border > 0) {
            MyQuickSort (data, size, border + 1, right, SzElem, CompareElements);
        }
        if (border - left + 1 > 0) {
            MyQuickSort (data, size, left, border, SzElem, CompareElements);
        }
    }
}