#ifndef __SCENE_H__
#define __SCENE_H__
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include "../force/Force.h"
#include "Utils.h"
#include "../../build.h"
using namespace Eigen;
using namespace std;
class Scene
{
    /*
    Mass of all particles is 1
    */
public:
    Scene(int particlesCount);

    inline Scene() {}

    inline const VectorXd &getX() const
    {
        return m_x;
    }
    inline VectorXd &getX()
    {
        return m_x;
    }

    inline const VectorXd &getV() const
    {
        return m_v;
    }

    inline VectorXd &getV()
    {
        return m_v;
    }

    void SetPosition(int i, const Vector3 &x);
    void SetVelocity(int i, const Vector3 &x);
    // friend ostream &operator<<(ostream &os, Scene const &s);
    string ToString() const;
    string SetUpToString() const;
    // FORCES
    inline void AddForce(shared_ptr<Force> force)
    {
        m_forces.push_back(force);
    }

    inline void AddEdge(int i, int j)
    {
        m_edges.push_back(make_pair(i, j));
    }

    inline const vector<pair<int, int>> &GetEdges() const
    {
        return m_edges;
    }

    void AccumulateForce(VectorXd &F);

    inline int GetParticleCount() const { return int(m_x.size()) / 3; }

    inline Vector3 GetCenter()
    {
        Vector3 output = Vector3::Zero();
        for (int i = 0; i < GetParticleCount(); i++)
        {
            output += m_x.segment<3>(3 * i);
        }
        output /= GetParticleCount();
        return output;
    }

    inline void TranslateAllPoints(Vector3 &x)
    {
        for (int i = 0; i < GetParticleCount(); i++)
        {
            m_x.segment<3>(3 * i) += x;
        }
    }

#ifdef LOG_ENERGY
    double ComputeKineticEnergy() const;

    double ComputeEnergy(string forcename) const;
#endif

public:
    static double PARTICLE_RADIUS;

private:
    VectorXd m_x; // positions
    VectorXd m_v; // velocities

    vector<pair<int, int>> m_edges;
    vector<shared_ptr<Force>> m_forces;
};

#endif