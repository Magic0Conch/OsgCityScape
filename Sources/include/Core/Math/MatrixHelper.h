#pragma once
#include "glm/fwd.hpp"
#include <osg/Matrix>
#include <osg/ref_ptr>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace CSEditor {
    static const float Math_PI = 3.14159265358979323846264338327950288f;
    class MatrixHelper{
        public:
        // static double getAspectRatio() 
        // {
        //     if(viewportHeight != 0)
        //         return 1.0 * viewportWidth / viewportHeight;
        //     else return 1;
        // };

        static double focal2fov(double focal)
        {
            if (focal < 0) focal = -focal;
            return  double(atan(1.0 / focal) * 2.0);
        }

        static glm::dmat4 getPerspectiveMatrix(double focal_length,double cut_off_near,double cut_off_far,float aspectRatio)
        {
            double fov = focal2fov(focal_length);
            glm::dmat4 perspective_matrix = glm::scale(
                glm::perspective(fov, 1.0, cut_off_near, cut_off_far),
                glm::dvec3(1.0 / aspectRatio, 1, 1));
            // return new osg::Matrixd(glm::value_ptr(perspective_matrix));
            return perspective_matrix;
        }        
        

        static glm::dmat4 getModelViewProjectionMatrix(double focal_length,double cut_off_near,double cut_off_far,float aspectRatio,glm::dmat4 modelViewMatrix)
        {
            glm::dmat4 model_view_projection_matrix =
                getPerspectiveMatrix(focal_length,cut_off_near,cut_off_far,aspectRatio) * modelViewMatrix;
            return model_view_projection_matrix;
        }   

        static glm::dvec3 getYawPitchRoll(glm::dmat4 M) {
            glm::dvec3 yawPitchRoll;
            glm::extractEulerAngleYXZ(glm::dmat4(glm::inverse(M)), yawPitchRoll.y, yawPitchRoll.x, yawPitchRoll.z);
            yawPitchRoll = glm::degrees(yawPitchRoll);
            yawPitchRoll.x -= 90.0;
            yawPitchRoll = glm::mod(yawPitchRoll + 180.0, 360.0) - 180.0;
            return yawPitchRoll;
        }
    //     glm::dvec3 getYawPitchRoll(glm::dmat4 M, int mode)
    //     {
    //         // only x right y up mode is implemented
    //         glm::dmat4 M_inv = glm::inverse(M); // change from model view to camera view
    //         cv::Vec3d angles;
    //         cv::Mat rmat(3, 3, CV_64F), mtxR, mtxQ;
    //         for (int i = 0; i < 3; i++)
    //         {
    //             for (int j = 0; j < 3; j++)
    //             {
    //                 rmat.at<double>(j, i) = M_inv[i][j]; // opengl(x y) to opencv(row,col), transposed
    //             }
    //         }
    //         angles = cv::RQDecomp3x3(rmat, mtxR, mtxQ); // rotations about three angles, x, y, z
    //         glm::dvec3 result;
    //         // yaw pitch roll, initially looking at the world west(-1,0,0), camera x right y up z back
    //         result[0] = angles[2] - 90; // yaw about z axis(camera y)
    //         if (result[0] < -180)result[0] += 360;
    //         if (result[0] >= 180)result[0] -= 360;
    //         result[1] = angles[0] - 90; // pitch about the new x axis(camera x)
    //         if (result[1] < -180)result[1] += 360;
    //         if (result[1] >= 180)result[1] -= 360;
    //         result[2] = angles[1]; // roll about the new y axis(camera -z)
    //         if (result[2] < -180)result[2] += 360;
    //         if (result[2] >= 180)result[2] -= 360;
    //         return result;
    //     }             
    // };

    static void getXYZYawPitchRoll(glm::dmat4& M, glm::dvec3& position, glm::dvec3& ypr) {
        glm::dmat4 M_inv = glm::inverse(M);
        position = glm::dvec3(M_inv * glm::dvec4(0.0, 0.0, 0.0, 1.0));
        ypr = MatrixHelper::getYawPitchRoll(M);
    }


    static osg::Matrixd glmToOsgMatrix(const glm::dmat4& glmMatrix) {
        osg::Matrixd osgMatrix;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                osgMatrix(i, j) = glmMatrix[i][j];
            }
        }
        return osgMatrix;
    }
    
    // void getXYZYawPitchRoll(glm::dmat4 M, glm::dvec3 & position, glm::dvec3 & ypr)
    // {
    //     glm::dmat4 M_inv = glm::inverse(M);
    //     position = glm::dvec3(M_inv * glm::dvec4(0.0, 0.0, 0.0, 1.0));
    //     ypr = getYawPitchRoll(M);
    // }
    };
}