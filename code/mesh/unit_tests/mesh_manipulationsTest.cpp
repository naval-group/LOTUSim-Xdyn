/*
 * mesh_manipulationsTest.cpp
 *
 *  Created on: May 20, 2014
 *      Author: cady
 */

#include "mesh_manipulationsTest.hpp"
#include "xdyn/external_data_structures/GeometricTypes3d.hpp"
#include "mesh_manipulations.hpp"
#include "test_data_generator/TriMeshTestData.hpp"
#include "xdyn/exceptions/MeshException.hpp"
#include "mesh/MeshBuilder.hpp"

#include <ssc/macros.hpp>

mesh_manipulationsTest::mesh_manipulationsTest() : a(ssc::random_data_generator::DataGenerator(321))
{
}

mesh_manipulationsTest::~mesh_manipulationsTest()
{
}

void mesh_manipulationsTest::SetUp()
{
}

void mesh_manipulationsTest::TearDown()
{
}

TEST_F(mesh_manipulationsTest, unit_normal)
{
//! [mesh_manipulationsTest unit_normal_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d n = unit_normal(M);
    ASSERT_DOUBLE_EQ(1, n.norm());
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(0));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(1));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(2));
//! [mesh_manipulationsTest unit_normal_example]
}

TEST_F(mesh_manipulationsTest, barycenter)
{
//! [mesh_manipulationsTest barycentre_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d G = barycenter(M);
    ASSERT_DOUBLE_EQ(1/3., (double)G(0));
    ASSERT_DOUBLE_EQ(1/3., (double)G(1));
    ASSERT_DOUBLE_EQ(1/3., (double)G(2));
//! [mesh_manipulationsTest barycentre_example]
}

TEST_F(mesh_manipulationsTest, area)
{
//! [mesh_manipulationsTest area_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    ASSERT_DOUBLE_EQ(1, area(M));
//! [mesh_manipulationsTest area_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity)
{
//! [mesh_manipulationsTest centre_of_gravity_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 4; M(0,2) = 3; M(0,3) = 1;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(2,(double)G(0));
    ASSERT_DOUBLE_EQ(4./9.,(double)G(1));
    ASSERT_DOUBLE_EQ(0,(double)G(2));
//! [mesh_manipulationsTest centre_of_gravity_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity_2)
{
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(0.5,(double)G(0));
    ASSERT_DOUBLE_EQ(0.5,(double)G(1));
    ASSERT_DOUBLE_EQ(0,(double)G(2));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_single_point)
{
    const VectorOfPoints single_point(1,a.random<EPoint>());
    // Doesn't much matter because there is no surface
    ASSERT_TRUE(oriented_inwards(single_point, a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_segment)
{
    VectorOfPoints segment;
    segment.push_back(a.random<EPoint>());
    segment.push_back(a.random<EPoint>());
    // Doesn't much matter because there is no surface
    ASSERT_TRUE(oriented_inwards(segment, a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_degenerated_triangle)
{
    // Doesn't much matter because there is no surface (so we don't know what "inside" means)
    ASSERT_FALSE(oriented_inwards(degenerated_triangle(), a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_triangle)
{
    ASSERT_TRUE(oriented_inwards(one_triangle(), EPoint(3,2,4)));
    ASSERT_FALSE(oriented_inwards(one_triangle(), EPoint(1,4.5,6)));
}

void flip(VectorOfVectorOfPoints& mesh, const size_t facet_index);
void flip(VectorOfVectorOfPoints& mesh, const size_t facet_index)
{
    const size_t first_coordinate = 1;
    const size_t second_coordinate = 2;
    const auto pivot = mesh.at(facet_index).at(first_coordinate);
    mesh.at(facet_index).at(first_coordinate) = mesh.at(facet_index).at(second_coordinate);
    mesh.at(facet_index).at(second_coordinate) = pivot;
}

TEST_F(mesh_manipulationsTest, issue_an_error_message_if_too_many_inward_facing_faces)
{
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    ASSERT_TRUE(error.str().empty());
    // Make a mesh with inward-oriented faces (4/12)
    auto mesh = unit_cube();
    flip(mesh, 4);
    flip(mesh, 5);
    flip(mesh, 6);
    flip(mesh, 7);
    // Call the orientation test function
    const bool inwards = check_oriented_inwards(mesh, EPoint(0, 0, 0));
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
    // Run test assertions after to avoid segfaults if test assertions fail
    ASSERT_EQ("Warning: 4 facets seem oriented inwards (body) while 8 facets seem oriented outwards (fluid). Please check that all the facet normals in your mesh are oriented outwards (fluid).\n", error.str());
    ASSERT_TRUE(inwards);
}

TEST_F(mesh_manipulationsTest, no_error_message_if_few_inward_facing_faces)
{
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    ASSERT_TRUE(error.str().empty());
    // Make a mesh with inward-oriented faces (1/12)
    auto mesh = unit_cube();
    flip(mesh, 4);
    // Call the orientation test function
    const bool inwards = check_oriented_inwards(mesh, EPoint(0, 0, 0));
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
    // Run test assertions after to avoid segfaults if test assertions fail
    ASSERT_TRUE(error.str().empty());
    ASSERT_FALSE(inwards);
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_tetrahedron)
{
    ASSERT_FALSE(check_oriented_inwards(tetrahedron(1,4,5,6), EPoint(4,5,6+sqrt(6.)/4.)));
    ASSERT_TRUE(check_oriented_inwards(tetrahedron_clockwise(1,2,3,4), EPoint(2,3,4+sqrt(6.)/4.)));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_cube)
{
    ASSERT_FALSE(check_oriented_inwards(unit_cube(), EPoint(0,0,0)));
    ASSERT_TRUE(check_oriented_inwards(unit_cube_clockwise(), EPoint(0,0,0)));
}

Matrix3x mesh_manipulationsTest::get_rectangle(const double w, const double h) const
{
    Matrix3x rectangle_vertices(3,4);
    rectangle_vertices <<
            -w/2 , +w/2 , +w/2 , -w/2 ,
            -h/2 , -h/2 , +h/2 , +h/2 ,
             0   ,  0   ,  0   ,  0   ;
    return rectangle_vertices;
}

TEST_F(mesh_manipulationsTest, can_compute_average_immersion)
{
//! [hydrostaticTest average_immersion_example]
    const Mesh mesh = MeshBuilder(trapezium()).build();
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double h = a.random<double>();
        ASSERT_DOUBLE_EQ(h/2., average_immersion(mesh.facets.front().vertex_index, {0,0,h,h}));
    }
//! [hydrostaticTest average_immersion_example]
}

TEST_F(mesh_manipulationsTest, inertia_of_rectangle)
{
    const double w = a.random<double>().between(-2,2);
    const double h = a.random<double>().between(-2,2);
    const auto all_nodes = get_rectangle(w,h);

    const std::vector<double> v = {1,2,3,4};

    const MeshPtr mesh(new Mesh(MeshBuilder(all_nodes).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);
    const auto Id=Eigen::MatrixXd::Identity(3,3);
    const auto J0=get_inertia_of_polygon_wrt(intersector.begin_immersed() , Id , mesh->all_nodes );

    double Jx = h*std::pow(w,3) / 12.0;
    double Jy = w*std::pow(h,3) / 12.0;
    double area = w*h;
    Eigen::Matrix3d expected;
    expected <<
             Jx ,  0 ,  0  ,
             0  , Jy ,  0  ,
             0  ,  0 ,  0  ;
    ASSERT_LT((expected/area - J0).array().abs().maxCoeff(),1.0e-10);
}

TEST_F(mesh_manipulationsTest, inertia_of_rotated_rectangle)
{
    const double w = a.random<double>().between(-2,2);
    const double h = a.random<double>().between(-2,2);
    const auto all_nodes = get_rectangle(w,h);

    const std::vector<double> v = {1,2,3,4};

    const MeshPtr mesh(new Mesh(MeshBuilder(all_nodes).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const double theta = a.random<double>().between(0,2*std::acos(1.0));
    const double c = std::cos(theta);
    const double s = std::sin(theta);
    Eigen::Matrix3d R10;
    R10 <<
             c , s , 0 ,
            -s , c , 0 ,
             0 , 0 , 1 ;
    const auto R01 = R10.transpose();

    const auto Id=Eigen::MatrixXd::Identity(3,3);
    const auto J0=get_inertia_of_polygon_wrt(intersector.begin_immersed() , Id  , mesh->all_nodes );
    const auto J1=get_inertia_of_polygon_wrt(intersector.begin_immersed() , R10 , mesh->all_nodes );

    const auto expected = R10*J0*R01;
    ASSERT_LT((double)((expected - J1).array().abs().maxCoeff()),1.0e-10);
}
