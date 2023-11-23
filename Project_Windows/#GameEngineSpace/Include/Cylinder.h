///////////////////////////////////////////////////////////////////////////////
// Cylinder.h
// ==========
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
// UPDATED: 2019-12-02
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_CYLINDER_H
#define GEOMETRY_CYLINDER_H

#include <vector>

class Cylinder
{
public:
    // ctor/dtor
    Cylinder(float baseRadius=1.0f, float topRadius=1.0f, float height=1.0f,
             int sectorCount=36, int stackCount=1, bool smooth=true);
    ~Cylinder() {}

    // getters/setters
    float getBaseRadius() const             { return baseRadius; }
    float getTopRadius() const              { return topRadius; }
    float getHeight() const                 { return height; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }
    void set(float baseRadius, float topRadius, float height,
             int sectorCount, int stackCount, bool smooth=true);
    void setBaseRadius(float radius);
    void setTopRadius(float radius);
    void setHeight(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);

    // for vertex data

    unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }
   
    unsigned int getIndexSize() const       { return (unsigned int)indices.size() * sizeof(unsigned int); }
    
    const unsigned int* getIndices() const  { return indices.data(); }


    // for interleaved vertices: V/N/T
  
    unsigned int getInterleavedVertexSize() const   { return (unsigned int)interleavedVertices.size() * sizeof(unsigned int); }    // # of bytes
    int getInterleavedStride() const                { return interleavedStride; }   // should be 32 bytes
    const float* getInterleavedVertices() const     { return &interleavedVertices[0]; }





protected:

private:
    // member functions
    void clearArrays();
    void buildVerticesSmooth();

    void buildInterleavedVertices();
    void buildUnitCircleVertices();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> getSideNormals();


    // memeber vars
    float baseRadius;
    float topRadius;
    float height;
    int sectorCount;                        // # of slices
    int stackCount;                         // # of stacks
    unsigned int baseIndex;                 // starting index of base
    unsigned int topIndex;                  // starting index of top
    bool smooth;
    std::vector<float> unitCircleVertices;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

};

#endif
