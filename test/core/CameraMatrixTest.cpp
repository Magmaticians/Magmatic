#include <gtest/gtest.h>
#include "core/CameraMatrix.hpp"

using namespace magmatic::core;

glm::mat4 mat1
{
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 1.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f},
};
glm::mat4 mat1inv
{
		{0.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, 1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f, 1.0f}
};

glm::mat4 mat2
{
		{1.0f, 1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
		{-1.0f, 0.0f, -1.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f, 1.0f}
};

glm::mat4 mat2inv{
		{0.0f, 1.0f, 0.0f, -1.0f},
		{1.0f, -1.0f, 1.0f, 0.0f},
		{0.0f, -1.0f, -1.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 0.0f}
};

glm::mat4 mat1mat2
{
		{4.0f, 2.0f, 2.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 0.0f},
		{-2.0f, -1.0f, -2.0f, -1.0f},
		{0.0f, -1.0f, -1.0f, -1.0f}
};

glm::mat4 invmat1mat2
{
	{0.0f, 1.0f, 0.0f, 0.0f},
	{1.0f, -2.0f, 1.0f, 0.0f},
	{0.0f, -2.0f, -1.0f, 1.0f},
	{-1.0f, 4.0f, 0.0f, -2.0f}
};

TEST(CameraMatrixTest, constructing)
{
	CameraMatrix cam(mat1, mat2);
	ASSERT_EQ(mat1, cam.view());
	ASSERT_EQ(mat2, cam.projection());
}

TEST(CameraMatrixTest, derived_matrices)
{
	CameraMatrix cam(mat1, mat2);

	ASSERT_EQ(mat1inv, cam.invView());
	ASSERT_EQ(mat2inv, cam.invProjection());

	ASSERT_EQ(mat1mat2, cam.viewProjection());
	ASSERT_EQ(invmat1mat2, cam.invViewProjection());
}

TEST(CameraMatrixTest, equality)
{
	CameraMatrix cam1(mat1, mat2);
	CameraMatrix cam2(mat1, mat2);
	CameraMatrix cam3(mat2, mat2);
	CameraMatrix cam4(mat2, mat2);

	ASSERT_TRUE(cam1 == cam2);
	ASSERT_TRUE(cam3 == cam4);

	ASSERT_FALSE(cam1 == cam3);
	ASSERT_FALSE(cam2 == cam4);

	ASSERT_TRUE(cam1 != cam3);
	ASSERT_TRUE(cam2 != cam4);

	ASSERT_FALSE(cam1 != cam2);
	ASSERT_FALSE(cam3 != cam4);
}