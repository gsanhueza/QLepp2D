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

#include <QDebug>
#include <QFile>
#include <QMap>
#include "off/offhandler.h"

OFFHandler::OFFHandler()
{
}

bool OFFHandler::loadOffFile(   QString &filepath,
                                OFFMetadata &metadata,
                                std::vector<Vertex> &vertices,
                                std::vector<int> &indices,
                                std::vector<Edge> &edges,
                                std::vector<Triangle> &triangles)
{
    qDebug() << "Loading OFF file from" << filepath << endl;

    QFile inputFile(filepath);

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);

        // Check if it's a real OFF file
        QString line = in.readLine();
        if (line.compare("OFF") != 0)
        {
            qCritical("Not an OFF file");
            return false;
        }

        // Old data cleanup
        vertices.clear();
        indices.clear();
        triangles.clear();
        edges.clear();

        // Skip comments
        do
        {
            line = in.readLine();
        } while (line.contains("#") or line.size() == 0);

        // Read file metadata (vertices, faces, edges)
        QStringList parsedmetadata = line.split(" ");

        metadata.vertices = parsedmetadata.at(0).toInt();
        metadata.indices = parsedmetadata.at(1).toInt();
        metadata.edges = parsedmetadata.at(2).toInt();

        qDebug() << "Number of vertices:" << metadata.vertices;
        qDebug() << "Number of faces:" << metadata.indices;
        qDebug() << "Number of edges:" << metadata.edges;

        // Read vertices data
        for (int i(0); i < metadata.vertices; i++)
        {
            line = in.readLine();
            QStringList coordinates = line.split(" ", QString::SkipEmptyParts);
            // We push the coordinates (x, y, z)
            Vertex v;
            v.x = coordinates.at(0).toFloat();
            v.y = coordinates.at(1).toFloat();
            v.z = coordinates.at(2).toFloat();
            vertices.push_back(v);
        }

        /* From here we create our structures in 4 phases:
         * Phase 1: Add triangles with incomplete data (only indices to vertices).
         * Phase 2: Create a temporal QMap that can detect neighbors of each
         * parsed triangle.
         * Phase 3: Use the temporal QMap to update the "edges" vector.
         * Phase 4: Update incomplete data of triangles with info from earlier
         * phases.
         */

        // Create QMap.
        QMap<QString, EdgeData> map;

        // Read faces data (indices)
        for (int i(0); i < metadata.indices; i++)
        {
            line = in.readLine();
            QStringList mappedIndices = line.split(" ", QString::SkipEmptyParts);
            // We check and skip the first one, because it marks the amount of indices, not the index itself.
            Triangle t;

            // Phase 1
            t.iv1 = mappedIndices.at(1).toInt();
            t.iv2 = mappedIndices.at(2).toInt();
            t.iv3 = mappedIndices.at(3).toInt();
            t.ie1 = -1;
            t.ie2 = -1;
            t.ie3 = -1;
            t.bad = 0;
            triangles.push_back(t);

            indices.push_back(t.iv1);
            indices.push_back(t.iv2);
            indices.push_back(t.iv3);

            // Phase 2
            QVector<int> tmpIV; // Temporal vertices
            tmpIV.append(t.iv1);
            tmpIV.append(t.iv2);
            tmpIV.append(t.iv3);

            for (int j(0); j < 3; j++)
            {
                QString key;
                key.append(std::min(tmpIV.at(j % 3), tmpIV.at((j + 1) % 3)));
                key.append("-");
                key.append(std::max(tmpIV.at(j % 3), tmpIV.at((j + 1) % 3)));
                EdgeData ed;

                if (map.contains(key))
                {
                    ed = map.value(key);
                    ed.ivopb = tmpIV.at((j + 2) % 3); // Index of opposite vertex from neighbour whose edge was already in "map"
                    ed.itb = i; // Index of current triangle, neighbour of earlier triangle in "map"
                    map.insert(key, ed);
                }
                else
                {
                    ed.iv1 = j % 3;
                    ed.iv2 = (j + 1) % 3;
                    ed.ivopa = (j + 2) % 3;
                    ed.ita = i; // Index of current triangle
                    ed.ivopb = -1; // Index of opposite vertex from neighbour not yet found
                    ed.itb = -1; // Index of neighbour triangle not yet found
                    map.insert(key, ed);
                }
            }
        }

        // Phase 3
        for (QMap<QString, EdgeData>::iterator i(map.begin()); i != map.end(); i++)
        {
            Edge e;
            e.ita = i.value().ita;
            e.itb = i.value().itb;
            edges.push_back(e);

            qDebug() << "test";
            // Phase 4
//            Triangle &ta = triangles.at(e.ita);
//            Triangle &tb = triangles.at(e.itb);
            // TODO Update ta.ie1, ta.ie2, ta.ie3
            // TODO Update tb.ie1, tb.ie2, tb.ie3
        }

        inputFile.close();
        return true;
    }
    return false;
}

bool OFFHandler::saveOffFile(   QString &filepath,
                                OFFMetadata &metadata,
                                std::vector<Vertex> &vertices,
                                std::vector<int> &indices) const
{
    qDebug() << "Saving OFF file to" << filepath << endl;
    qDebug() << "(V, F, E) = " << metadata.vertices << " " << metadata.indices << " " << metadata.edges;
    qDebug() << "coordinatesPerVertex = " << vertices.size() / metadata.vertices;

    QFile outputFile(filepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&outputFile);
        out << "OFF" << endl;
        out << "# File created by QLepp2D." << endl;
        out << metadata.vertices << " " << metadata.indices << " " << metadata.edges;

        // Write vertices
        int coordinatesPerVertex = vertices.size() / metadata.vertices;
        for (unsigned int i(0); i < vertices.size(); i++)
        {
            if (i % coordinatesPerVertex == 0)
            {
                out << endl;
            }
            out << vertices.at(i).x << " ";
            out << vertices.at(i).y << " ";
            out << vertices.at(i).z;
        }

        // Write faces (indices)
        int verticesPerFace = indices.size() / metadata.indices;
        for (unsigned int i(0); i < indices.size(); i++)
        {
            if (i % verticesPerFace == 0)
            {
                out << endl;
                out << verticesPerFace;
            }
            out << " " << indices.at(i);
        }

        outputFile.close();

        return true;
    }
    return false;
}
