#include <gtest/gtest.h>

int main(int argc, char** args)
{
    ::testing::InitGoogleTest(&argc, args);

    int res = RUN_ALL_TESTS();

    return res;
}
