#ifndef __VOXELIZE_HPP__
#define __VOXELIZE_HPP__
#include <vector>
#include <iostream>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>

using namespace std;

class VoxelMask
{
public:
    inline VoxelMask(int N)
    {
        m_size = N;
        for (int i = 0; i < N; i++)
        {
            m_mask.push_back(vector<vector<bool>>());
            for (int j = 0; j < N; j++)
            {
                m_mask[i].push_back(vector<bool>());
                for (int k = 0; k < N; k++)
                {
                    m_mask[i][j].push_back(false);
                }
            }
        }
    }

    inline bool set(int i, int j, int k, bool val)
    {
        if (i >= m_size || j >= m_size || k >= m_size)
        {
            cout << "ERROR: wrong dimensions" << endl;
            return false;
        }
        m_mask[i][j][k] = val;
        return true;
    }

    inline void ForEachVoxel(function<void(int, int, int, bool)> f)
    {
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                for (int k = 0; k < m_size; k++)
                {
                    f(i, j, k, m_mask[i][j][k]);
                }
            }
        }
    }

    inline bool Get(int i, int j, int k) const
    {
        if (i >= m_size || j >= m_size || k >= m_size || i < 0 || j < 0 || k < 0)
        {
            return false;
        }
        return m_mask[i][j][k];
    }

    inline int Size() const { return m_size; }

private:
    vector<vector<vector<bool>>> m_mask;
    int m_size;
};

// Input: 3D mask of where a voxel should be
// Output: vertices, edges, and faces
void Voxelize(VoxelMask &mask, nlohmann::json &output, double L = 1);
#endif