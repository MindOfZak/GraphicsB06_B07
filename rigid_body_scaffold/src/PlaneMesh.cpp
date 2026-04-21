#include "PlaneMesh.h"


void PlaneMesh::createXZ() {

        //leapWid = (float) width / (float) resWid;
        //float offset_x = (float) width / 2.0f - (float) leapWid / 2.0f; // center

        //leapLen = (float) length / (float) resLen;
        //float offset_z = (float) length / 2.0f - (float) leapLen / 2.0f; // center

        //restLengthX = leap_x;
        //restLengthZ = leap_z;
        //restLengthXZ = sqrt(pow(leap_x, 2) + pow(leap_z, 2));

        Vertex v;
        for (float z = 0; z < resLen; z += 1.0f) {
            for (float x = 0; x < resWid; x += 1.0f) {
                v.pos = glm::vec3(x*leapWid + orgWid, height, z*leapLen + orgLen);
                //init_positions.push_back(glm::vec3(x*leap_x - offset_x, height, z*leap_z - offset_z));

                v.normal = glm::vec3(0,1,0);
                v.uv = glm::vec2(x /(resWid - 1), z / (resLen-1));
                vertices.push_back(v);

                // Properties
                //forces.push_back(glm::vec3(0));
                //velocities.push_back(glm::vec3(0));
                //prev_positions.push_back(vertices[vertices.size() - 1]);

            }
        }

        for (int i = 0; i < resWid - 1; i++) {
            for (int j = 0; j < resLen -1; j++) {
                // Triangle 1
                indices.push_back(i + j * (resWid));
                indices.push_back((i + 1) + j * (resWid));
                indices.push_back(i + (j + 1) * (resWid));

                // Triangle 2
                indices.push_back((i + 1) + j * (resWid));
                indices.push_back((i + 1) + (j + 1) * (resWid));
                indices.push_back((i) + (j + 1) * (resWid));

            }
        }

    }