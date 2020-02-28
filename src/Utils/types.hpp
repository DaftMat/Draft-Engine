/**
 * Created by mathis on 28/02/2020.
 */

#ifndef DAFT_ENGINE_TYPES_HPP
#define DAFT_ENGINE_TYPES_HPP

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Sparse>

namespace Utils {
    using VectorN  = Eigen::Matrix<float, Eigen::Dynamic, 1>;
    using VectorNf = Eigen::VectorXf;
    using VectorNd = Eigen::VectorXd;

    using Vector4  = Eigen::Matrix<float, 4, 1>;
    using Vector4f = Eigen::Vector4f;
    using Vector4d = Eigen::Vector4d;

    using Vector3  = Eigen::Matrix<float, 3, 1>;
    using Vector3f = Eigen::Vector3f;
    using Vector3d = Eigen::Vector3d;

    using Vector2  = Eigen::Matrix<float, 2, 1>;
    using Vector2f = Eigen::Vector2f;
    using Vector2d = Eigen::Vector2d;

    using VectorNi = Eigen::VectorXi;
    using Vector2i = Eigen::Vector2i;
    using Vector3i = Eigen::Vector3i;
    using Vector4i = Eigen::Vector4i;

    using VectorNui = Eigen::Matrix<uint, Eigen::Dynamic, 1>;
    using Vector1ui = Eigen::Matrix<uint, 1, 1>;
    using Vector2ui = Eigen::Matrix<uint, 2, 1>;
    using Vector3ui = Eigen::Matrix<uint, 3, 1>;
    using Vector4ui = Eigen::Matrix<uint, 4, 1>;

    using Ray  = Eigen::ParametrizedLine<float, 3>;
    using Rayf = Eigen::ParametrizedLine<float, 3>;
    using Rayd = Eigen::ParametrizedLine<double, 3>;

    using MatrixN = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;
    using Matrix4 = Eigen::Matrix<float, 4, 4>;
    using Matrix3 = Eigen::Matrix<float, 3, 3>;
    using Matrix2 = Eigen::Matrix<float, 2, 2>;

    using MatrixNf = Eigen::MatrixXf;
    using Matrix4f = Eigen::Matrix4f;
    using Matrix3f = Eigen::Matrix3f;
    using Matrix2f = Eigen::Matrix2f;

    using MatrixNd = Eigen::MatrixXd;
    using Matrix4d = Eigen::Matrix4d;
    using Matrix3d = Eigen::Matrix3d;
    using Matrix2d = Eigen::Matrix2d;

    using MatrixNui = Eigen::Matrix<uint, Eigen::Dynamic, Eigen::Dynamic>;

    using Diagonal = Eigen::DiagonalMatrix< float, Eigen::Dynamic >;
    using Sparse = Eigen::SparseMatrix<float>;

    using Quaternion  = Eigen::Quaternion<float>;
    using Quaternionf = Eigen::Quaternionf;
    using Quaterniond = Eigen::Quaterniond;

    using Transform  = Eigen::Transform<float, 3, Eigen::Affine>;
    using Transformf = Eigen::Affine3f;
    using Transformd = Eigen::Affine3d;

    using Aabb  = Eigen::AlignedBox<float, 3>;
    using Aabbf = Eigen::AlignedBox3f;
    using Aabbd = Eigen::AlignedBox3d;

    using AngleAxis  = Eigen::AngleAxis<float>;
    using AngleAxisf = Eigen::AngleAxisf;
    using AngleAxisd = Eigen::AngleAxisd;

    using Translation  = Eigen::Translation<float, 3>;
    using Translationf = Eigen::Translation3f;
    using Translationd = Eigen::Translation3d;
}

#endif //DAFT_ENGINE_TYPES_HPP
