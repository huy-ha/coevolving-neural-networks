#include <Voxelize.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <tuple>
#include <algorithm>

static int IndexOfPoint(vector<tuple<double, double, double>> &points,
                        const tuple<double, double, double> &point)
{
    auto it = find(points.begin(), points.end(), point);
    if (it == points.end())
        return -1;
    return it - points.begin();
}

static void AddToEdgesIfPointExists(
    set<tuple<int, int>> &edges,
    shared_ptr<vector<tuple<double, double, double>>> points,
    int i, double L,
    double x, double y, double z)
{
    int idx = IndexOfPoint((*points), make_tuple(x, y, z));
    if (idx != -1)
    {
        double x_this = get<0>((*points)[i]);
        double y_this = get<1>((*points)[i]);
        double z_this = get<2>((*points)[i]);
        edges.insert(make_tuple(max(i, idx), min(i, idx)));
    }
}

void Voxelize(VoxelMask &mask, nlohmann::json &output, double L)
{
    using namespace std;
    double x, y, z;

    auto pointSet = set<tuple<double, double, double>>();
    auto edgesSet = set<tuple<int, int>>();

    auto faces = make_shared<vector<tuple<int, int, int>>>();
    int maskSize = mask.Size();
    mask.ForEachVoxel([&](int i, int j, int k, bool val) {
        if (val)
        {
            pointSet.insert(make_tuple(i - 0.5, j - 0.5, k - 0.5));
            pointSet.insert(make_tuple(i - 0.5, j - 0.5, k + 0.5));
            pointSet.insert(make_tuple(i - 0.5, j + 0.5, k - 0.5));
            pointSet.insert(make_tuple(i - 0.5, j + 0.5, k + 0.5));
            pointSet.insert(make_tuple(i + 0.5, j - 0.5, k - 0.5));
            pointSet.insert(make_tuple(i + 0.5, j - 0.5, k + 0.5));
            pointSet.insert(make_tuple(i + 0.5, j + 0.5, k - 0.5));
            pointSet.insert(make_tuple(i + 0.5, j + 0.5, k + 0.5));
        }
    });
    auto points = make_shared<vector<tuple<double, double, double>>>(pointSet.begin(), pointSet.end());

    // add unique particles
    for (int i = 0; i < points->size(); ++i)
    {
        x = get<0>((*points)[i]);
        y = get<1>((*points)[i]);
        z = get<2>((*points)[i]);

        nlohmann::json point;
        point["x"] = x * L;
        point["y"] = y * L;
        point["z"] = z * L;
        output["vertices"].push_back(point);

        // current layer
        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y, z + 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y, z + 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y, z + 1);

        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y + 1, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y + 1, z + 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y + 1, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y + 1, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y + 1, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y + 1, z + 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y + 1, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y + 1, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y + 1, z + 1);

        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y - 1, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y - 1, z + 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x, y - 1, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y - 1, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y - 1, z);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y - 1, z + 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y - 1, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x + 1, y - 1, z - 1);
        AddToEdgesIfPointExists(edgesSet, points, i, L, x - 1, y - 1, z + 1);
    }
    auto edges = make_shared<vector<tuple<int, int>>>(edgesSet.begin(), edgesSet.end());

    // Create faces
    mask.ForEachVoxel([&](int i, int j, int k, bool val) {
        if (val)
        {
            // up
            if (!mask.Get(i, j + 1, k))
            {
                int vtx1 = IndexOfPoint(*points, make_tuple(i + 0.5, j + 0.5, k + 0.5));
                int vtx2 = IndexOfPoint(*points, make_tuple(i + 0.5, j + 0.5, k - 0.5));
                int vtx3 = IndexOfPoint(*points, make_tuple(i - 0.5, j + 0.5, k + 0.5));
                int vtx4 = IndexOfPoint(*points, make_tuple(i - 0.5, j + 0.5, k - 0.5));

                faces->push_back(make_tuple(vtx1, vtx4, vtx2));
                faces->push_back(make_tuple(vtx1, vtx3, vtx4));
            }

            // down
            if (!mask.Get(i, j - 1, k))
            {
                int vtx1 = IndexOfPoint(*points, make_tuple(i + 0.5, j - 0.5, k + 0.5));
                int vtx2 = IndexOfPoint(*points, make_tuple(i + 0.5, j - 0.5, k - 0.5));
                int vtx3 = IndexOfPoint(*points, make_tuple(i - 0.5, j - 0.5, k + 0.5));
                int vtx4 = IndexOfPoint(*points, make_tuple(i - 0.5, j - 0.5, k - 0.5));

                faces->push_back(make_tuple(vtx1, vtx4, vtx2));
                faces->push_back(make_tuple(vtx1, vtx3, vtx4));
            }

            // left
            if (!mask.Get(i + 1, j, k))
            {
                int vtx1 = IndexOfPoint(*points, make_tuple(i + 0.5, j - 0.5, k + 0.5));
                int vtx2 = IndexOfPoint(*points, make_tuple(i + 0.5, j - 0.5, k - 0.5));
                int vtx3 = IndexOfPoint(*points, make_tuple(i + 0.5, j + 0.5, k + 0.5));
                int vtx4 = IndexOfPoint(*points, make_tuple(i + 0.5, j + 0.5, k - 0.5));

                faces->push_back(make_tuple(vtx1, vtx4, vtx2));
                faces->push_back(make_tuple(vtx1, vtx3, vtx4));
            }

            //right
            if (!mask.Get(i - 1, j, k))
            {
                int vtx1 = IndexOfPoint(*points, make_tuple(i - 0.5, j - 0.5, k + 0.5));
                int vtx2 = IndexOfPoint(*points, make_tuple(i - 0.5, j - 0.5, k - 0.5));
                int vtx3 = IndexOfPoint(*points, make_tuple(i - 0.5, j + 0.5, k + 0.5));
                int vtx4 = IndexOfPoint(*points, make_tuple(i - 0.5, j + 0.5, k - 0.5));

                faces->push_back(make_tuple(vtx1, vtx4, vtx2));
                faces->push_back(make_tuple(vtx1, vtx3, vtx4));
            }

            //forward
            if (!mask.Get(i, j, k + 1))
            {
                int vtx1 = IndexOfPoint(*points, make_tuple(i + 0.5, j - 0.5, k + 0.5));
                int vtx2 = IndexOfPoint(*points, make_tuple(i - 0.5, j - 0.5, k + 0.5));
                int vtx3 = IndexOfPoint(*points, make_tuple(i + 0.5, j + 0.5, k + 0.5));
                int vtx4 = IndexOfPoint(*points, make_tuple(i - 0.5, j + 0.5, k + 0.5));

                faces->push_back(make_tuple(vtx1, vtx4, vtx2));
                faces->push_back(make_tuple(vtx1, vtx3, vtx4));
            }
            //back
            if (!mask.Get(i, j, k - 1))
            {
                int vtx1 = IndexOfPoint(*points, make_tuple(i + 0.5, j - 0.5, k - 0.5));
                int vtx2 = IndexOfPoint(*points, make_tuple(i - 0.5, j - 0.5, k - 0.5));
                int vtx3 = IndexOfPoint(*points, make_tuple(i + 0.5, j + 0.5, k - 0.5));
                int vtx4 = IndexOfPoint(*points, make_tuple(i - 0.5, j + 0.5, k - 0.5));

                faces->push_back(make_tuple(vtx1, vtx4, vtx2));
                faces->push_back(make_tuple(vtx1, vtx3, vtx4));
            }
        }
    });

    for (auto &edge : *edges)
    {
        nlohmann::json edgeOutput;
        edgeOutput["i"] = get<0>(edge);
        edgeOutput["j"] = get<1>(edge);
        output["edges"].push_back(edgeOutput);
    }

    for (auto &face : *faces)
    {
        nlohmann::json faceOutput;
        faceOutput.push_back(get<0>(face));
        faceOutput.push_back(get<1>(face));
        faceOutput.push_back(get<2>(face));
        output["faces"].push_back(faceOutput);
    }
}