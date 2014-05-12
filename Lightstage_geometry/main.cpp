#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "TriMesh.h"

using namespace std;
using namespace cv;
using namespace trimesh;

int main(int argc, char** argv)
{
    if ( argc != 4 )
        {
            printf("usage: cmd <Mesh> <Normal Map> <camera.txt>\n");
            return -1;
        }


    printf("Reading camera parameters...\n");
    ifstream camFile(argv[3]);
    if(camFile.is_open()){
        String line;

        Matx33f rot_mat;
        Vec3f trans_vec;
        Matx33f cam_mat(0,0,0,0,0,0,0,0,0);
        Vec4f distCoef(0,0,0,0);

        while(getline(camFile,line))
        {
            size_t sep_index = line.find(":");

            String tag = line.substr(0,sep_index);
            String value = line.substr(sep_index+1,line.length());

            float tmp_scale = 1;

            if(tag == "fx"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "fx : " << v << endl;
                cam_mat(0,0) = v;

            }else if(tag == "fy"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "fy : " << v << endl;
                cam_mat(1,1) = v;

            }else if(tag == "cx"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "cx : " << v << endl;
                cam_mat(0,2) = v;

            }else if(tag == "cy"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "cy : " << v << endl;
                cam_mat(1,2) = v;

            }else if(tag == "skew"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "skew : " << v << endl;
                cam_mat(0,1) = v;

            }else if(tag == "k1"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "k1 : " << v << endl;
                distCoef(0) = v;

            }else if(tag == "k2"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "k2 : " << v << endl;
                distCoef(1) = v;

            }else if(tag == "p1"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "p1 : " << v << endl;
                distCoef(2) = v;

            }else if(tag == "p2"){

                float v; sscanf(value.c_str(),"%f",&v);
                //cout << "p2 : " << v << endl;
                distCoef(3) = v;

            }else if(tag == "r00"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(0,0) = v * tmp_scale;

            }else if(tag == "r01"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(0,1) = v * tmp_scale;

            }else if(tag == "r02"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(0,2) = v * tmp_scale;

            }else if(tag == "r10"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(1,0) = v * tmp_scale;

            }else if(tag == "r11"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(1,1) = v * tmp_scale;

            }else if(tag == "r12"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(1,2) = v * tmp_scale;

            }else if(tag == "r20"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(2,0) = v * tmp_scale;

            }else if(tag == "r21"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(2,1) = v * tmp_scale;

            }else if(tag == "r22"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(2,2) = v * tmp_scale;

            }else if(tag == "t0"){

                float v; sscanf(value.c_str(),"%f",&v);
                trans_vec(0) = v * tmp_scale;

            }else if(tag == "t1"){

                float v; sscanf(value.c_str(),"%f",&v);
                trans_vec(1) = v * tmp_scale;

            }else if(tag == "t2"){

                float v; sscanf(value.c_str(),"%f",&v);
                trans_vec(2) = v * tmp_scale;

            }

        }

        Vec3f vec_rot(0,0,0);

        Rodrigues(rot_mat,vec_rot);

        TriMesh *themesh = TriMesh::read(argv[1]);


        if(!themesh){
            cout << "A problem occured while reading the mesh" << endl;
        }else{
            Mat normalMap = imread(argv[2]);

            vector<trimesh::point> vertices = themesh->vertices;
            vector<cv::Point3f> points_3D;

            for(int i = 0 ; i < vertices.size() ; i++){
                trimesh::point v = vertices[i];

//                cout << v << endl;

                Point3f p(v[0],v[1],v[2]);
                points_3D.push_back(p);

            }

            vector<cv::Point2f> points_2D;
            projectPoints(points_3D, vec_rot, trans_vec, cam_mat, distCoef,points_2D);

            cv::Point2f up_left = points_2D[0];
            cv::Point2f down_right = points_2D[0];
            float max_z = points_3D[0].z;
            float min_z = points_3D[0].z;

            for(int i = 0 ; i < points_2D.size() ; i++){

                cv::Point2f p = points_2D.at(i);

                if(up_left.x > p.x ) up_left.x = p.x;
                if(up_left.y > p.y ) up_left.y = p.y;
                if(down_right.x < p.x) down_right.x = p.x;
                if(down_right.y < p.y) down_right.y = p.y;

                if(max_z < points_3D[i].z) max_z = points_3D[i].z;
                if(min_z > points_3D[i].z) min_z = points_3D[i].z;

//                p.x += cam_mat(0,2);
//                p.y += cam_mat(1,2);

//                cout << p << endl;
            }

            float img_scale =1;
            int w = (down_right.x - up_left.x)*img_scale;
            int h = (down_right.y - up_left.y)*img_scale;

            Mat img;
            img.create(h + 3,w + 3,CV_8U);


            cout << up_left << endl;
            cout << down_right << endl;
            cout << w << " , " << h << endl;
            cout << min_z << " , " << max_z << endl;

            for(int i = 0 ; i < points_3D.size() ; i++){
                int x = floor(points_2D[i].x - up_left.x) * img_scale + 1;
                int y = floor(points_2D[i].y - up_left.y) * img_scale + 1;

//                cout << x << " , " << y << endl;

                img.row(y).col(x) = points_3D[i].z * 255 / (max_z - min_z);
            }

            Mat img1;
            Size s(w/10, h/10);
            cv::resize(img,img1,s);
            imwrite("/home/alou/Tmp/test.bmp", img1);

        }

    }


        waitKey(0);
    return 0;
}

