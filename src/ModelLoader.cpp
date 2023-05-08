#include "ModelLoader.h"

bool ModelLoader::loadOBJModel(Mesh & const mesh, std::string filepath) {
    std::ifstream file(filepath);
    std::vector<int> tokens;
    std::vector<uint32_t> indices;
    std::vector<Vertex> vertices;
    std::vector <Math::Vector3> vertPos;
    std::vector<Math::Vector2> texCoords;
    std::vector<Math::Vector3> normals;
    int vertexCount = 0;

    if (file)
    {
        while (!file.eof())
        {
            std::string word;
            file >> word;
            if (strcmp(word.c_str(), "v") == 0)
            {
                //v (.obj vertex texture coordinate)
                float vertX, vertY, vertZ;
                file >> vertX >> vertY >> vertZ;    //Store the next three types

                vertPos.push_back(Math::Vector3(vertX, vertY, vertZ * -1.0f));
            }

            if (strcmp(word.c_str(), "vt") == 0)
            {
                //vt (.obj vertex texture coordinate)
                float vertTexCoordX, vertTexCoordY;
                file >> vertTexCoordX >> vertTexCoordY;
                texCoords.push_back(Math::Vector2(vertTexCoordX, 1.0f - vertTexCoordY));
            }  
            if (strcmp(word.c_str(), "vn") == 0)
            {
                //vt (.obj vertex texture coordinate)
                float vertexNormalX, vertexNormalY, vertexNormalZ;
                file >> vertexNormalX >> vertexNormalY >> vertexNormalZ;
                normals.push_back(Math::Vector3(vertexNormalX, vertexNormalY, vertexNormalZ * -1.0f));
            }
            if (strcmp(word.c_str(), "f") == 0)
            {
                vertexCount++;
                std::string values;

                std::getline(file, values);
                values.erase(std::remove(values.begin(), values.end(), 'f'), values.end());
                std::replace(values.begin(), values.end(), ' ', '/'); // replace all spaces (' ') with '/' 
                values.erase(0, 1); //deletes first char in string
                std::stringstream ss(values);

                std::string temp_str; //string that gets cut

                while (getline(ss, temp_str, '/')) { //use '/' as delim for cutting string
                    tokens.push_back(std::stoi(temp_str) - 1); //add every face value (vertex/normal/coordinate) into vector
                }
            }
        }
    }
    else
    {
        MessageBox(0, L"No model file could be found", L"Error", MB_OK);
        return false;
    }


    for (UINT i = 0; i < 3 * vertexCount; i++)
    {
        Vertex vertIndex;
        vertIndex.pos = vertPos[tokens[(i * 3) + 0]]; // index * 3 + offset to get vertex index, normal & coordinate
        vertIndex.texCoord = texCoords[tokens[(i * 3) + 1]];
        vertIndex.normals = normals[tokens[(i * 3) + 2]];
        vertices.push_back(vertIndex); // add vertex to vector
    }


    for (int i = 0; i < vertices.size(); i++)
    {
        mesh.indices.push_back(i);
        mesh.vertices.push_back(vertices[i]);
    }

    return true;
}