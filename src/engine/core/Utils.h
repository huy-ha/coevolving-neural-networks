#ifndef __UTILS_H__
#define __UTILS_H__
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <string>
#include <vector>
#include <future>
#include <chrono>

typedef Eigen::Matrix<double, 3, 1> Vector3;
typedef Eigen::Matrix<double, 3, 3> Matrix3;

bool CheckOrCreateDirectory(std::string path);
std::string GetFullPath(std::string path);
void GetFilesInDirectory(const std::string &path, std::vector<std::string> &v);

template <typename R>
inline bool is_ready(std::future<R> const &f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}
#endif