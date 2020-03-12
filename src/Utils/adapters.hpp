//
// Created by mathis on 28/02/2020.
//

#ifndef DAFT_ENGINE_ADAPTERS_HPP
#define DAFT_ENGINE_ADAPTERS_HPP

#include <Eigen>
#include <glm.hpp>

template <int m, int n>
inline glm::mat<m, n, float> toGlm( const Eigen::Matrix<float, m, n>& mat ) {
    glm::mat<m, n, float> ret;
    for ( int i = 0; i < m; ++i )
    {
        for ( int j = 0; j < n; ++j )
        {
            ret[j][i] = mat( i, j );
        }
    }
    return ret;
}

template <int m>
inline glm::vec<m, float> toGlm( const Eigen::Vector<float, m>& vec ) {
    glm::vec<m, float> ret;
    for ( int i = 0; i < m; ++i )
    {
        ret[i] = vec( i );
    }
    return ret;
}

template <int m, int n>
inline Eigen::Matrix<float, m, n> toEigen( const glm::mat<m, n, float>& mat ) {
    Eigen::Matrix<float, m, n> ret;
    for ( int i = 0; i < m; ++i )
    {
        for ( int j = 0; j < n; ++j )
        {
            ret( i, j ) = mat[j][i];
        }
    }
    return ret;
}

template <int m>
inline Eigen::Vector<float, m> toEigen( const glm::vec<m, float>& vec ) {
    Eigen::Vector<float, m> ret;
    for ( int i = 0; i < m; ++i )
    {
        ret( i ) = vec[i];
    }
    return ret;
}

#endif // DAFT_ENGINE_ADAPTERS_HPP
