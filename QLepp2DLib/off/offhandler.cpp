/*
 * QLepp2D is a triangulation improver and visualization program that uses
 * a Lepp-Delaunay algorithm.
 * Copyright (C) 2017-2018 Gabriel Sanhueza <gabriel_8032@hotmail.com>
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

OFFHandler::OFFHandler() = default;

bool OFFHandler::loadOffFile(std::string filepath,
                             OFFMetadata &metadata,
                             std::vector<Vertex> &vertices,
                             std::vector<Edge> &edges,
                             std::vector<Triangle> &triangles)
{
    QString qfilepath = QString::fromStdString(filepath);
    qDebug() << "Loading OFF file from" << QString(qfilepath) << endl;

    QFile inputFile(qfilepath);

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
        metadata.triangles = parsedmetadata.at(1).toInt();
        metadata.edges = parsedmetadata.at(2).toInt();

        qDebug() << "Number of vertices:" << metadata.vertices;
        qDebug() << "Number of faces:" << metadata.triangles;
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
         * Phase 4: Update incomplete data of triangles with info from phase 3.
         */

        // Create QMap.
        QMap<QString, EdgeData> map;

        // Read faces data (indices)
        for (int i(0); i < metadata.triangles; i++)
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
            t.valid = 1;
            triangles.push_back(t);

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
                }
                else
                {
                    ed.iv1 = tmpIV.at(j % 3);
                    ed.iv2 = tmpIV.at((j + 1) % 3);
                    ed.ivopa = tmpIV.at((j + 2) % 3);
                    ed.ita = i; // Index of current triangle
                    ed.ivopb = -1; // Index of opposite vertex from neighbour not yet found
                    ed.itb = -1; // Index of neighbour triangle not yet found
                }
                map.insert(key, ed);
            }
        }

        // Phase 3
        int k = 0;
        for (QMap<QString, EdgeData>::iterator i(map.begin()); i != map.end(); i++, k++)
        {
            Edge e;
            e.ita = i.value().ita;
            e.itb = i.value().itb;
            e.iv1 = std::min(i.value().iv1, i.value().iv2);
            e.iv2 = std::max(i.value().iv1, i.value().iv2);
            e.isTerminalEdge = 0;
            e.isBorderEdge = 0;
            edges.push_back(e);

            // Phase 4
            // Triangle A
            Triangle &ta = triangles.at(e.ita);
            if (i.value().ivopa == ta.iv1)
            {
                ta.ie1 = k;
            }
            else if (i.value().ivopa == ta.iv2)
            {
                ta.ie2 = k;
            }
            else if (i.value().ivopa == ta.iv3)
            {
                ta.ie3 = k;
            }
            else
            {
                qFatal("Inconsistent data (A)!");
            }

            // Triangle B
            if (e.itb < 0)
            {
                continue; // Maybe there isn't a neighbour triangle.
            }

            Triangle &tb = triangles.at(e.itb);

            if (i.value().ivopb == tb.iv1)
            {
                tb.ie1 = k;
            }
            else if (i.value().ivopb == tb.iv2)
            {
                tb.ie2 = k;
            }
            else if (i.value().ivopb == tb.iv3)
            {
                tb.ie3 = k;
            }
            else
            {
                qFatal("Inconsistent data (B)!");
            }
        }

        inputFile.close();
        return true;
    }
    return false;
}

bool OFFHandler::saveOffFile(std::string filepath,
                             OFFMetadata &metadata,
                             std::vector<Vertex> &vertices,
                             std::vector<Triangle> &triangles) const
{
    QString qfilepath = QString::fromStdString(filepath);
    qDebug() << "Saving OFF file to" << qfilepath << endl;
    qDebug() << "(V, F, E) = " << metadata.vertices << " " << metadata.triangles << " " << metadata.edges;

    QFile outputFile(qfilepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&outputFile);
        out << "OFF" << endl;
        out << "# File created by QLepp2D." << endl;
        out << metadata.vertices << " " << metadata.triangles << " " << metadata.edges;

        // Write vertices
        int coordinatesPerVertex = static_cast<int>(vertices.size()) / metadata.vertices;
        for (int i(0); i < static_cast<int>(vertices.size()); i++)
        {
            if (i % coordinatesPerVertex == 0)
            {
                out << endl;
            }
            out << vertices.at(static_cast<unsigned long>(i)).x << " ";
            out << vertices.at(static_cast<unsigned long>(i)).y << " ";
            out << vertices.at(static_cast<unsigned long>(i)).z;
        }

        out << endl;

        // Write faces (indices)
        int verticesPerFace = 3;
        for (const Triangle &t : triangles)
        {
            out << verticesPerFace << " ";
            out << t.iv1 << " ";
            out << t.iv2 << " ";
            out << t.iv3 << endl;
        }

        outputFile.close();

        return true;
    }
    return false;
}
