#define _USE_MATH_DEFINES

#include "math/matrix.h"
#include "math/vector.h"
#include "math/plane.h"
#include "utils/utils.h"

#include <gtest/gtest.h>

class CMatrixVecRotTest : public ::testing::Test
{
};

TEST_F(CMatrixVecRotTest, IdentityMatrixRotation)
{
    CMatrix mat;
    mat.identity();

    CVector const input{1.0f, 2.0f, 3.0f};
    CVector const result = mat.vecRot(input);

    EXPECT_TRUE(utils::VectorsEqual(result, input));
}

TEST_F(CMatrixVecRotTest, ZeroVector)
{
    CMatrix mat;
    mat.identity();

    CVector const input{0.0f, 0.0f, 0.0f};
    CVector const result = mat.vecRot(input);

    EXPECT_TRUE(utils::VectorsEqual(result, {0.0f, 0.0f, 0.0f}));
}

TEST_F(CMatrixVecRotTest, RotationAroundZ)
{
    CMatrix mat;
    mat.rotYPR(0.0f, 0.0f, M_PI_2);

    CVector const input{1.0f, 0.0f, 0.0f};
    CVector const result = mat.vecRot(input);

    EXPECT_TRUE(utils::VectorsEqual(result, {0.0f, 1.0f, 0.0f}));
}

TEST_F(CMatrixVecRotTest, RotationAroundY)
{
    CMatrix mat;
    mat.rotYPR(M_PI_2, 0.0f, 0.0f);

    CVector const input{1.0f, 0.0f, 0.0f};
    CVector const result = mat.vecRot(input);

    EXPECT_TRUE(utils::VectorsEqual(result, {0.0f, 0.0f, -1.0f}));
}

TEST_F(CMatrixVecRotTest, NegativeVector)
{
    CMatrix mat;
    mat.identity();

    CVector const input{-5.0f, -3.0f, -2.0f};
    CVector const result = mat.vecRot(input);

    EXPECT_TRUE(utils::VectorsEqual(result, input));
}

TEST_F(CMatrixVecRotTest, UniformScaledMatrix)
{
    CMatrix mat;
    mat.identity();
    mat._11 = 2.0f;
    mat._22 = 2.0f;
    mat._33 = 2.0f;

    CVector const input{1.0f, 1.0f, 1.0f};
    CVector const result = mat.vecRot(input);

    EXPECT_TRUE(utils::VectorsEqual(result, {2.0f, 2.0f, 2.0f}));
}

class CMatrixReflectTest : public ::testing::Test
{
};

TEST_F(CMatrixReflectTest, ReflectAcrossXYPlane)
{
    CMatrix mat;
    CPlane plane;
    plane.m_normal = {0.0f, 0.0f, 1.0f};
    plane.m_dist = 0.0f;

    mat.reflect(plane);

    EXPECT_TRUE(utils::FloatEquals(mat._11, 1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._22, 1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._33, -1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._44, 1.0f));
}

TEST_F(CMatrixReflectTest, ReflectAcrossYZPlane)
{
    CMatrix mat;
    CPlane plane;
    plane.m_normal = {1.0f, 0.0f, 0.0f};
    plane.m_dist = 0.0f;

    mat.reflect(plane);

    EXPECT_TRUE(utils::FloatEquals(mat._11, -1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._22, 1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._33, 1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._44, 1.0f));
}

TEST_F(CMatrixReflectTest, ReflectAcrossXZPlane)
{
    CMatrix mat;
    CPlane plane;
    plane.m_normal = {0.0f, 1.0f, 0.0f};
    plane.m_dist = 0.0f;

    mat.reflect(plane);

    EXPECT_TRUE(utils::FloatEquals(mat._11, 1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._22, -1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._33, 1.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._44, 1.0f));
}

TEST_F(CMatrixReflectTest, ReflectWithNonZeroDistance)
{
    CMatrix mat;
    CPlane plane;
    plane.m_normal = {0.0f, 0.0f, 1.0f};
    plane.m_dist = 5.0f;

    mat.reflect(plane);

    EXPECT_TRUE(utils::FloatEquals(mat._41, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._42, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._43, 10.0f));
}

TEST_F(CMatrixReflectTest, ReflectOffDiagonalZero)
{
    CMatrix mat;
    CPlane plane;
    plane.m_normal = {1.0f, 0.0f, 0.0f};
    plane.m_dist = 0.0f;

    mat.reflect(plane);

    EXPECT_TRUE(utils::FloatEquals(mat._12, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._13, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._21, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._23, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._31, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._32, 0.0f));
}

TEST_F(CMatrixReflectTest, ReflectBottomRowCorrect)
{
    CMatrix mat;
    CPlane plane;
    plane.m_normal = {1.0f, 1.0f, 1.0f};
    plane.m_dist = 2.0f;

    mat.reflect(plane);

    EXPECT_TRUE(utils::FloatEquals(mat._14, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._24, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._34, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(mat._44, 1.0f));
}

class CMatrixGetYPRTest : public ::testing::Test
{
};

TEST_F(CMatrixGetYPRTest, IdentityMatrix)
{
    CMatrix mat;
    mat.identity();

    float y, p, r;
    mat.getYPR(y, p, r);

    EXPECT_TRUE(utils::FloatEquals(p, 0.0f));
    EXPECT_TRUE(utils::FloatEquals(r, 0.0f));
}

TEST_F(CMatrixGetYPRTest, PitchNinety)
{
    CMatrix mat;
    mat.identity();
    mat._23 = 1.0f;
    mat._11 = 0.0f;
    mat._12 = 1.0f;

    float y, p, r;
    mat.getYPR(y, p, r);

    EXPECT_TRUE(utils::FloatEquals(p, M_PI_2, 1e-4f));
    EXPECT_TRUE(utils::FloatEquals(r, 0.0f));
}

TEST_F(CMatrixGetYPRTest, PitchNegativeNinety)
{
    CMatrix mat;
    mat.identity();
    mat._23 = -1.0f;
    mat._11 = 0.0f;
    mat._12 = 1.0f;

    float y, p, r;
    mat.getYPR(y, p, r);

    EXPECT_TRUE(utils::FloatEquals(p, -M_PI_2, 1e-4f));
    EXPECT_TRUE(utils::FloatEquals(r, 0.0f));
}

TEST_F(CMatrixGetYPRTest, NormalCase)
{
    CMatrix mat;
    mat.identity();
    mat._23 = 0.5f;
    mat._13 = 0.5f;
    mat._33 = 0.866f;
    mat._21 = 0.5f;
    mat._22 = 0.866f;

    float y, p, r;
    mat.getYPR(y, p, r);

    float const expectedPitch = asin(0.5f);
    EXPECT_TRUE(utils::FloatEquals(p, expectedPitch));
}

TEST_F(CMatrixGetYPRTest, SmallAngles)
{
    CMatrix mat;
    mat.identity();
    mat._23 = 0.1f;
    mat._13 = 0.05f;
    mat._33 = 0.99875f;
    mat._21 = 0.05f;
    mat._22 = 0.99875f;

    float y, p, r;
    mat.getYPR(y, p, r);

    float const expectedPitch = asin(0.1f);
    EXPECT_TRUE(utils::FloatEquals(p, expectedPitch));
    EXPECT_TRUE(p < 0.2f && p > 0.0f);
}
