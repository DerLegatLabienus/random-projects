#include "StackWrapper.h"

int main(int argc, char **argv) {
    StackWrapper<int> aviv{};
    aviv.push_wrapper(1);
    int popped = aviv.pop_wrapper();

}
