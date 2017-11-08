/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017  Gabriel Sanhueza <gabriel_8032@hotmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENCLENGINE_H
#define OPENCLENGINE_H

#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
# include <OpenCL/opencl.hpp>
#else
# include <CL/cl.hpp>
#endif

#include "engine/abstractengine.h"

class OpenCLEngine : public AbstractEngine
{
public:
    OpenCLEngine();

    virtual bool detectBadTriangles(double &angle,
                                    std::vector<Triangle> &triangles,
                                    std::vector<Vertex> &vertices);
    virtual bool improveTriangulation(  std::vector<Triangle> &triangles,
                                        std::vector<Vertex> &vertices,
                                        std::vector<int> &indices,
                                        OFFMetadata &metadata);

protected:
    virtual void setup();

private:
    std::vector<cl::Platform> m_platforms;
    std::vector<cl::Device> m_devices;
    cl::Context m_context;
    cl::CommandQueue m_queue;
    cl::Program m_program;
};

#endif // OPENCLENGINE_H
