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
#include "off/offhandler.h"

OFFHandler::OFFHandler()
{
}

bool OFFHandler::loadOffFile(QString &filepath, OFFMetadata &metadata, std::vector<Vertex> &vertices, std::vector<int> &indices, std::vector<Triangle> &triangles)
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

        // Read faces data (indices)
        for (int i(0); i < metadata.indices; i++)
        {
            line = in.readLine();
            QStringList mappedIndices = line.split(" ", QString::SkipEmptyParts);
            // We check and skip the first one, because it marks the amount of indices, not the index itself.
            Triangle t;
            t.i1 = mappedIndices.at(1).toInt();
            t.i2 = mappedIndices.at(2).toInt();
            t.i3 = mappedIndices.at(3).toInt();
            t.bad = 0;
            triangles.push_back(t);

            indices.push_back(t.i1);
            indices.push_back(t.i2);
            indices.push_back(t.i3);
        }

        inputFile.close();
        return true;
    }
    return false;
}

bool OFFHandler::saveOffFile(QString &filepath, OFFMetadata &metadata, std::vector<Vertex> &vertices, std::vector<int> &indices) const
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
