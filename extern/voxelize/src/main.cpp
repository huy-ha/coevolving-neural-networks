#include <iostream>
#include <Voxelize.hpp>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;
int main(int argc, char **argv)
{
    nlohmann::json j;
    int n = 10;
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
    auto start = high_resolution_clock::now();

    Voxelize(mask, j);
    auto end = high_resolution_clock::now();
    cout << "voxelization of cube of length " << n << " took " << duration_cast<duration<double>>(end - start).count() << " seconds" << endl;
    return 0;
}