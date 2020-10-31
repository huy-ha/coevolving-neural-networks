#include <gtest/gtest.h>
#include <Voxelize.hpp>
#include <vector>
#include <tuple>
#include <string>
#include <functional>
#include <nlohmann/json.hpp>

using namespace std;

TEST(Dimensions, TestCube1)
{
    nlohmann::json j;
    int n = 1;
    VoxelMask mask(n);
    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < n; y++)
        {
            for (int z = 0; z < n; z++)
            {
                mask.set(x, y, z, true);
            }
        }
    }
    Voxelize(mask, j);
    EXPECT_EQ(j["vertices"].size(), 8);
    EXPECT_EQ(j["edges"].size(), 28);
}

TEST(Dimensions, TestCube2)
{
    nlohmann::json j;
    int n = 1;
    VoxelMask mask(2);
    mask.set(0, 0, 0, true);
    mask.set(1, 0, 0, true);
    Voxelize(mask, j);
    EXPECT_EQ(j["vertices"].size(), 12);
    // EXPECT_EQ(j["edges"].size(), 50);
}

TEST(Dimensions, TestCube3)
{
    nlohmann::json j;
    int n = 1;

    VoxelMask mask(3);
    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < n; y++)
        {
            for (int z = 0; z < n; z++)
            {
                mask.set(x, y, z, true);
            }
        }
    }
    Voxelize(mask, j);
    EXPECT_EQ(j["vertices"].size(), (n + 1) * (n + 1) * (n + 1));
}