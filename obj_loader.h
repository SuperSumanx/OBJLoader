#ifndef OBJ_LOADER
#define OBJ_LOADER

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <array>

inline void processLine(std::stringstream &sstream, unsigned int indices[3][3])
{
    std::string temp;

    std::size_t delimiter=-1;
    std::size_t seperatorIndex;
    for(int i = 0; i<3; ++i)
    {
        sstream >> temp; //take a triad and store in temp;
        //std::cout << "Triad is " << temp << std::endl;
        for(int j=0; j<3; ++j)
        {
            seperatorIndex=temp.find('/', delimiter+1);
            if(seperatorIndex==std::string::npos)
            {
                std::string number = temp.substr(delimiter+1, temp.length()-delimiter-1);
                //std::cout << "Processed number is " << number << std::endl;
                indices[j][i]=std::stoi(number)-1;
                delimiter=-1;
                break; //coz this is the last element
            }
            std::string number=temp.substr(delimiter+1, seperatorIndex-delimiter-1);
            //std::cout << "Processed number is " << number << std::endl;
            if(number.empty())
            {
                indices[j][i]=-1;
                delimiter=seperatorIndex;
                continue;
            }
            indices[j][i]=std::stoi(number)-1;
            delimiter=seperatorIndex;
            continue;
        }
        //std::cout << "****************************" << std::endl;
        //std::cout << std::endl;
    }
}

static void processInstructions(std::string instrucLine, std::vector<glm::vec3> &vertexArray, std::vector<glm::vec3> &normalArray, std::vector<glm::vec2> &textureArray, std::vector< unsigned int > &indicesArray)
{
    std::stringstream objLine;
    objLine.precision(10); //we want a precision of 10 decimal places
    objLine << instrucLine;

    std::string str;

    //start processing
    objLine >> str;
    if(str[0] == '#') //if it is a comment
        return; //there is nothing unsigned interesting in this line so skip

    //now to proceed...
    //test for plain vertices if(str == "v")
    if(str == "v")
    {
        float vertices[3]; //temp to store the vertices
        for(unsigned int i=0; i<3; i++)
        {
            objLine >> vertices[i];
        }
        vertexArray.push_back(glm::vec3(vertices[0],vertices[1],vertices[2]));
    }

    //test for texture coordinates
    if(str == "vt")
    {
        float texture[2];
        for(unsigned int i=0; i<2; i++)
        {
            objLine >> texture[i];
        }
        textureArray.push_back(glm::vec2(texture[0],texture[1]));
    }

    //test for normals
    if(str == "vn")
    {
        float normals[3];
        for(unsigned int i=0; i<3; i++)
        {
            objLine >> normals[i];
        }
        normalArray.push_back(glm::vec3(normals[0],normals[1],normals[2]));
    }

    //face indices
    //We'll use a string to hold the commands and parse it independently
    if(str=="f")
    {
        std::string indexTriad;
        unsigned int indices[3][3]; //array to hold all values
        //parse the individual indices
        //f vertex/texture/normal

        processLine(objLine, indices);
        //std::cout << std::endl << "Now printing and storing array " << std::endl;
        for(int i=0; i<3; i++)
        {
            //std::cout << "Pushing " << indices[0][i] << std::endl;
            indicesArray.push_back(indices[0][i]);
            //std::cout << std::endl;
        }
    }
}

bool load_obj(const char * objPath, std::vector<glm::vec3> &vertexArray, std::vector<glm::vec3> &normalArray, std::vector<glm::vec2> &textureArray, std::vector < unsigned int > &indicesArray)
{
    std::ifstream objStream;
    objStream.open(objPath);

    if(objStream.good())   //if there are not problems in opening the file
    {
        std::string objLine;
        while(!objStream.eof())
        {
            std::getline(objStream, objLine);
            processInstructions(objLine, vertexArray, normalArray, textureArray, indicesArray);
        }
        return true; //FIXME
    }
    else
    {
        std::cerr << "obj_loader :\tFailed " << std::endl;
        std::cerr << "obj_loader :\tCould not load obj file" << std::endl;
        return false;
    }
}

#endif

