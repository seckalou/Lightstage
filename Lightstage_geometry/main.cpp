#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "TriMesh.h"

using namespace std;
using namespace cv;

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

            if(tag == "fx"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "fx : " << v << endl;
                cam_mat(0,0) = v;

            }else if(tag == "fy"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "fy : " << v << endl;
                cam_mat(1,1) = v;

            }else if(tag == "cx"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "cx : " << v << endl;
                cam_mat(0,2) = v;

            }else if(tag == "cy"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "cy : " << v << endl;
                cam_mat(1,2) = v;

            }else if(tag == "skew"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "skew : " << v << endl;
                cam_mat(0,1) = v;

            }else if(tag == "k1"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "k1 : " << v << endl;
                distCoef(0) = v;

            }else if(tag == "k2"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "k2 : " << v << endl;
                distCoef(1) = v;

            }else if(tag == "p1"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "p1 : " << v << endl;
                distCoef(2) = v;

            }else if(tag == "p2"){

                float v; sscanf(value.c_str(),"%f",&v);
                cout << "p2 : " << v << endl;
                distCoef(3) = v;

            }else if(tag == "r00"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(0,0) = v;

            }else if(tag == "r01"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(0,1) = v;

            }else if(tag == "r02"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(0,2) = v;

            }else if(tag == "r10"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(1,0) = v;

            }else if(tag == "r11"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(1,1) = v;

            }else if(tag == "r12"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(1,2) = v;

            }else if(tag == "r20"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(2,0) = v;

            }else if(tag == "r21"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(2,1) = v;

            }else if(tag == "r22"){

                float v; sscanf(value.c_str(),"%f",&v);
                rot_mat(2,2) = v;

            }else if(tag == "t0"){

                float v; sscanf(value.c_str(),"%f",&v);
                trans_vec(0) = v;

            }else if(tag == "t1"){

                float v; sscanf(value.c_str(),"%f",&v);
                trans_vec(1) = v;

            }else if(tag == "t2"){

                float v; sscanf(value.c_str(),"%f",&v);
                trans_vec(2) = v;

            }

        }

        Vec3f vec_rot(0,0,0);

        Rodrigues(rot_mat,vec_rot);



    }


        waitKey(0);
    return 0;
}

