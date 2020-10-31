#include "Scene.h"
#include <sstream>
#include <future>
#include "../../build.h"
using namespace Eigen;

double Scene::PARTICLE_RADIUS = 0.01;
const int num_cpus = std::thread::hardware_concurrency();
Scene::Scene(int particleCount)
{
    m_x = VectorXd::Zero(particleCount * 3);
    m_v = VectorXd::Zero(particleCount * 3);
}

void Scene::SetPosition(int i, const Vector3 &x)
{
    m_x.segment<3>(3 * i) = x;
}

void Scene::SetVelocity(int i, const Vector3 &v)
{
    m_v.segment<3>(3 * i) = v;
}

void Scene::AccumulateForce(VectorXd &F)
{
#ifdef MULTITHREADED_ACCUMULATE_FORCE
    int n = m_forces.size();
    vector<future<VectorXd>> futureResults;
    const auto x = getX();
    const auto v = getV();
    for (int cpu = 0; cpu < num_cpus; cpu++)
    {
        int start = cpu * n / num_cpus;
        int end = min(n, (cpu + 1) * n / num_cpus);
        // handle forces from start to end
        futureResults.push_back(
            async([&]() {
                VectorXd subF = VectorXd::Zero(F.size());
                for (int i = start; i < end; i++)
                {
                    m_forces[i]->addForceToTotal(x, v, subF);
                }
                return subF;
            }));
    }
    for (int i = 0; i < futureResults.size(); i++)
    {
        F += futureResults[i].get();
    }
#else
    for (const shared_ptr<Force> &force : m_forces)
    {
        force->addForceToTotal(getX(), getV(), F);
    }
#endif
}

string Scene::ToString() const
{
    std::stringstream s;
    s << getX().transpose();
    return s.str();
}

string Scene::SetUpToString() const
{
    std::stringstream ss;
    for (auto &edge : m_edges)
    {
        ss << edge.first << " " << edge.second << endl;
    }
    return ss.str();
}

#ifdef LOG_ENERGY
double Scene::ComputeKineticEnergy() const
{
    double totalKineticEnergy = 0;
    for (int i = 0, n = GetParticleCount(); i < n; i++)
    {
        totalKineticEnergy += 0.5 * m_v.segment<3>(3 * i).squaredNorm();
    }
    return totalKineticEnergy;
}

double Scene::ComputeEnergy(string forcename) const
{
    double E = 0;
    for (const shared_ptr<Force> &force : m_forces)
    {
        if (typeid(*force).name() == forcename)
        {
            force->addEnergyToTotal(getX(), getV(), E);
        }
    }
    return E;
}
#endif