///////////////////////////////////////////////////////////////////////////////
// Cylinder.cpp
// ============
// Cylinder for OpenGL with (base radius, top radius, height, sectors, stacks)
// The min number of sectors (slices) is 3 and the min number of stacks are 1.
// - base radius: the radius of the cylinder at z = -height/2
// - top radius : the radiusof the cylinder at z = height/2
// - height     : the height of the cylinder along z-axis
// - sectors    : the number of slices of the base and top caps
// - stacks     : the number of subdivisions along z-axis
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2018-03-27
// UPDATED: 2020-03-14
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Cylinder.h"



// constants //////////////////////////////////////////////////////////////////
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT  = 1;



///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
Cylinder::Cylinder(float baseRadius, float topRadius, float height, int sectors,
                   int stacks, bool smooth) : interleavedStride(32)
{
    set(baseRadius, topRadius, height, sectors, stacks, smooth);
}



///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void Cylinder::set(float baseRadius, float topRadius, float height, int sectors,
                   int stacks, bool smooth)
{
    this->baseRadius = baseRadius;
    this->topRadius = topRadius;
    this->height = height;
    this->sectorCount = sectors;
    if(sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if(stacks < MIN_STACK_COUNT)
        this->stackCount = MIN_STACK_COUNT;
    this->smooth = smooth;

    // generate unit circle vertices first
    buildUnitCircleVertices();

   buildVerticesSmooth();

}

void Cylinder::setBaseRadius(float radius)
{
    if(this->baseRadius != radius)
        set(radius, topRadius, height, sectorCount, stackCount, smooth);
}

void Cylinder::setTopRadius(float radius)
{
    if(this->topRadius != radius)
        set(baseRadius, radius, height, sectorCount, stackCount, smooth);
}

void Cylinder::setHeight(float height)
{
    if(this->height != height)
        set(baseRadius, topRadius, height, sectorCount, stackCount, smooth);
}

void Cylinder::setSectorCount(int sectors)
{
    if(this->sectorCount != sectors)
        set(baseRadius, topRadius, height, sectors, stackCount, smooth);
}

void Cylinder::setStackCount(int stacks)
{
    if(this->stackCount != stacks)
        set(baseRadius, topRadius, height, sectorCount, stacks, smooth);
}

///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void Cylinder::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}



///////////////////////////////////////////////////////////////////////////////
// build vertices of cylinder with smooth shading
// where v: sector angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void Cylinder::buildVerticesSmooth()
{
    // clear memory of prev arrays
    clearArrays();

    float x, y, z;                                  // vertex position
    //float s, t;                                     // texCoord
    float radius;                                   // radius for each stack

    // get normals for cylinder sides
    std::vector<float> sideNormals = getSideNormals();

    // put vertices of side cylinder to array by scaling unit circle
    for(int i = 0; i <= stackCount; ++i)
    {
        z = -(height * 0.5f) + (float)i / stackCount * height;      // vertex position z
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);     // lerp
        float t = 1.0f - (float)i / stackCount;   // top-to-bottom

        for(int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k+1];
            addVertex(x * radius, y * radius, z);   // position
            addNormal(sideNormals[k], sideNormals[k+1], sideNormals[k+2]); // normal
            addTexCoord((float)j / sectorCount, t); // tex coord
        }
    }

    // remember where the base.top vertices start
    unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, -1);
    addTexCoord(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * baseRadius, y * baseRadius, z);
        addNormal(0, 0, -1);
        addTexCoord(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);    // flip horizontal
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of top of cylinder
    z = height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, 1);
    addTexCoord(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * topRadius, y * topRadius, z);
        addNormal(0, 0, 1);
        addTexCoord(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
    }

    // put indices for sides
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // bebinning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 trianles per sector
            addIndices(k1, k1 + 1, k2);
            addIndices(k2, k1 + 1, k2 + 1);

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            // horizontal lines
            lineIndices.push_back(k2);
            lineIndices.push_back(k2 + 1);
            if(i == 0)
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // remember where the base indices start
    baseIndex = (unsigned int)indices.size();

    // put indices for base
    for(int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < (sectorCount - 1))
            addIndices(baseVertexIndex, k + 1, k);
        else    // last triangle
            addIndices(baseVertexIndex, baseVertexIndex + 1, k);
    }

    // remember where the base indices start
    topIndex = (unsigned int)indices.size();

    for(int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < (sectorCount - 1))
            addIndices(topVertexIndex, k, k + 1);
        else
            addIndices(topVertexIndex, k, topVertexIndex + 1);
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}


///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void Cylinder::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        //interleavedVertices.push_back(vertices[i]);
        //interleavedVertices.push_back(vertices[i+1]);
        //interleavedVertices.push_back(vertices[i+2]);
        interleavedVertices.insert(interleavedVertices.end(), &vertices[i], &vertices[i] + 3);

        //interleavedVertices.push_back(normals[i]);
        //interleavedVertices.push_back(normals[i+1]);
        //interleavedVertices.push_back(normals[i+2]);
        interleavedVertices.insert(interleavedVertices.end(), &normals[i], &normals[i] + 3);

        //interleavedVertices.push_back(texCoords[j]);
        //interleavedVertices.push_back(texCoords[j+1]);
        interleavedVertices.insert(interleavedVertices.end(), &texCoords[j], &texCoords[j] + 2);
    }
}



///////////////////////////////////////////////////////////////////////////////
// generate 3D vertices of a unit circle on XY plance
///////////////////////////////////////////////////////////////////////////////
void Cylinder::buildUnitCircleVertices()
{
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float>().swap(unitCircleVertices);
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
}



///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void Cylinder::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}



///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void Cylinder::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}



///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void Cylinder::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}



///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
void Cylinder::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}



///////////////////////////////////////////////////////////////////////////////
// generate shared normal vectors of the side of cylinder
///////////////////////////////////////////////////////////////////////////////
std::vector<float> Cylinder::getSideNormals()
{
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan2(baseRadius - topRadius, height);
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    // rotate (x0,y0,z0) per sector angle
    std::vector<float> normals;
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        normals.push_back(cos(sectorAngle)*x0 - sin(sectorAngle)*y0);   // nx
        normals.push_back(sin(sectorAngle)*x0 + cos(sectorAngle)*y0);   // ny
        normals.push_back(z0);  // nz
        /*
        //debug
        float nx = cos(sectorAngle)*x0 - sin(sectorAngle)*y0;
        float ny = sin(sectorAngle)*x0 + cos(sectorAngle)*y0;
        std::cout << "normal=(" << nx << ", " << ny << ", " << z0
                  << "), length=" << sqrtf(nx*nx + ny*ny + z0*z0) << std::endl;
        */
    }

    return normals;
}