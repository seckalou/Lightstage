#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "TriMesh.h"
#include "rapidxml.hpp"

using namespace std;
using namespace cv;
using namespace trimesh;
using namespace rapidxml;

int main(int argc, char** argv)
{
    if ( argc != 4 )
    {
        printf("usage: cmd <Mesh> <Normal Map> <camera.xml>\n");
        return -1;
    }


    /*************************READING CAMERAS****************************/

    printf("Reading camera parameters...\n");

    map<char*, map<char*,float> > sensors;
    map<char*, map<char*,float> > cameras;

    ifstream camFile(argv[3]);
    if(camFile.is_open()){

        /*String line;
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

        }*/

        camFile.seekg(0, camFile.end);
        int camFileLength = camFile.tellg();
        camFile.seekg(0, camFile.beg);

        char* camFileBuff = new char[camFileLength];

        camFile.read(camFileBuff, camFileLength);

        camFileBuff[camFileLength] = 0;

        xml_document<> doc;
        doc.parse<0>(camFileBuff);

        xml_node<> *root_node = doc.first_node("document");
        if(!root_node){
            cerr << "Error on reading XML cameras file" << endl;
            return -1;
        }

        root_node = root_node->first_node("chunk");
        if(!root_node){
            cerr << "Error on reading XML cameras file" << endl;
            return -1;
        }

        xml_node<> *sensors_node = root_node->first_node("sensors");
        if(!sensors_node){
            cerr << "Error on reading XML cameras file...(sensors node missing)" << endl;
            return -1;
        }

        xml_node<> *cameras_node = root_node->first_node("cameras");
        if(!cameras_node){
            cerr << "Error on reading XML cameras file...(cameras node missing)" << endl;
            return -1;
        }

        xml_node<> *node = sensors_node->first_node("sensor");

        while(node){

            float fx, fy, cx, cy, k1, k2, k3;
            xml_attribute<> *att_id = node->first_attribute("id");

            xml_node<> *calib_node = node->first_node("calibration");
            if(!calib_node){
                cerr << "Error on reading XML cameras file" << endl;
                return -1;
            }
            xml_node<> *param_node = calib_node->first_node("fx");
            if(param_node){
                sscanf(param_node->value(), "%f", &fx);
            }else{
                cerr << "Error on reading XML cameras file...(fx missing)" << endl;
                return -1;
            }

            param_node = calib_node->first_node("fy");
            if(param_node){
                sscanf(param_node->value(), "%f", &fy);
            }else{
                cerr << "Error on reading XML cameras file...(fy missing)" << endl;
                return -1;
            }

            param_node = calib_node->first_node("cx");
            if(param_node){
                sscanf(param_node->value(), "%f", &cx);
            }else{
                cerr << "Error on reading XML cameras file...(cx missing)" << endl;
                return -1;
            }

            param_node = calib_node->first_node("cy");
            if(param_node){
                sscanf(param_node->value(), "%f", &cy);
            }else{
                cerr << "Error on reading XML cameras file...(cy missing)" << endl;
                return -1;
            }

            param_node = calib_node->first_node("k1");
            if(param_node){
                sscanf(param_node->value(), "%f", &k1);
            }else{
                cerr << "Error on reading XML cameras file...(k1 missing)" << endl;
                return -1;
            }

            param_node = calib_node->first_node("k2");
            if(param_node){
                sscanf(param_node->value(), "%f", &k2);
            }else{
                cerr << "Error on reading XML cameras file...(k2 missing)" << endl;
                return -1;
            }

            param_node = calib_node->first_node("k3");
            if(param_node){
                sscanf(param_node->value(), "%f", &k3);
            }else{
                cerr << "Error on reading XML cameras file...(k3 missing)" << endl;
                return -1;
            }

            map<char*, float> entry;
            entry.insert(pair<char*, float>("fx",fx)); entry.insert(pair<char*, float>("fy",fy));
            entry.insert(pair<char*, float>("cx",cx)); entry.insert(pair<char*, float>("cy",cy));
            entry.insert(pair<char*, float>("k1",k1));
            entry.insert(pair<char*, float>("k2",k2));
            entry.insert(pair<char*, float>("k3",k3));

            sensors.insert(pair<char*, map<char*, float > > (att_id->value(), entry));

            node = node->next_sibling("sensor");
        }

        node = cameras_node->first_node("camera");

        while(node){
            float id, sensor;
            float t0, t1, t2,
                    r00, r01, r02,
                    r10, r11, r12,
                    r20, r21, r22;

            map<char*, float> entry;

            xml_attribute<> *att_id = node->first_attribute("id");
            xml_attribute<> *att_sensor_id = node->first_attribute("sensor_id");

            sscanf(att_id->value(), "%f", &id);
            sscanf(att_sensor_id->value(), "%f", &sensor);

            xml_node<> *trans_node = node->first_node("transform");
            if(trans_node){
                sscanf(trans_node->value(), "%f %f %f %f %f %f %f %f %f %f %f %f ",
                       &r00, &r01, &r02, &t0,
                       &r10, &r11, &r12, &t1,
                       &r20, &r21, &r22, &t2);
            }else{
                cerr << "Error on reading XML cameras file" << endl;
                return -1;
            }

            entry.insert(pair<char* , float>("sensor", sensor));

            entry.insert(pair<char*, float>("r00", r00));
            entry.insert(pair<char*, float>("r01", r01));
            entry.insert(pair<char*, float>("r02", r02));
            entry.insert(pair<char*, float>("r10", r10));
            entry.insert(pair<char*, float>("r11", r11));
            entry.insert(pair<char*, float>("r12", r12));
            entry.insert(pair<char*, float>("r20", r20));
            entry.insert(pair<char*, float>("r21", r21));
            entry.insert(pair<char*, float>("r22", r22));
            entry.insert(pair<char*, float>("t0", t0));
            entry.insert(pair<char*, float>("t1", t1));
            entry.insert(pair<char*, float>("t2", t2));

            cameras.insert(pair<char*, map<char*, float> >(att_id->value(), entry));

            node = node->next_sibling("camera");

        }

    }else{
        return -1;
    }

    /*************************READING MESH*****************************/

    TriMesh *themesh = TriMesh::read(argv[1]);


    if(!themesh){
        cout << "A problem occured while reading the mesh" << endl;
        return -1;
    }

    /************************PROJECTING ON EACH VIEW*******************/
    Matx33f rot_mat;
    Vec3f trans_vec;
    Matx33f cam_mat(0,0,0,0,0,0,0,0,0);
    Vec4f distCoef(0,0,0,0);
    int index = 0;

    for(map<char*, map<char*, float> >::iterator iter = cameras.begin();
        iter != cameras.end(); iter++){

        cout << iter->first << "---------------------------------------------"<< endl << endl;

        map<char*, float> cam = iter->second;

        rot_mat(0,0)=cam.at("r00"); rot_mat(0,1)=cam.at("r01"); rot_mat(0,2)=cam.at("r02");
        rot_mat(1,0)=cam.at("r10"); rot_mat(1,1)=cam.at("r11"); rot_mat(1,2)=cam.at("r12");
        rot_mat(2,0)=cam.at("r20"); rot_mat(2,1)=cam.at("r21"); rot_mat(2,2)=cam.at("r22");

        trans_vec(0)=cam.at("t0"); trans_vec(1)=cam.at("t1"); trans_vec(2)=cam.at("t2");

        cout << rot_mat << endl << rot_mat.inv() << endl << trans_vec << endl;


        Vec3f vec_rot(0,0,0);

        Rodrigues(rot_mat,vec_rot);

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
        char outputfile[100];
        sprintf(outputfile,"/home/als31/Tmp/test%d.bmp", index++);
        imwrite(, img1);

    }








    return 0;

}

