#pragma once

#include <vector>
#include <iostream>
#include "Mesh.h"
#include "AOMMath.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <Windows.h>

struct ModelLoader
{
    bool loadOBJModel(Mesh& const mesh, std::string filepath);
};
