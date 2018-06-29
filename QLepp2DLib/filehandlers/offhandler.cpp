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
#include <filehandlers/offhandler.h>

bool OFFHandler::load(std::string filepath,
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

        int numVertices = parsedmetadata.at(0).toInt();
        int numTriangles = parsedmetadata.at(1).toInt();
        int numEdges = parsedmetadata.at(2).toInt();

        qDebug() << "Number of vertices:" << numVertices;
        qDebug() << "Number of faces:" << numTriangles;
        qDebug() << "Number of edges:" << numEdges;

        // Read vertices data
        for (int i(0); i < numVertices; i++)
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
        QMap<QString, Edge> map;

        // Read faces data (indices)
        for (int i(0); i < numTriangles; i++)
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

            // Phase 2
            QVector<int> tmpIV; // Temporal vertices
            tmpIV.append(t.iv1);
            tmpIV.append(t.iv2);
            tmpIV.append(t.iv3);

            for (int j(0); j < 3; j++)
            {
                QString key = QString("%1-%2")
                        .arg(std::min(tmpIV.at(j % 3), tmpIV.at((j + 1) % 3)))
                        .arg(std::max(tmpIV.at(j % 3), tmpIV.at((j + 1) % 3)));
                Edge ed;

                if (map.contains(key))
                {
                    ed = map.value(key);
                    ed.itb = i; // Index of current triangle, neighbour of earlier triangle in "map"
                }
                else
                {
                    ed.iv1 = std::min(tmpIV.at(j % 3), tmpIV.at((j + 1) % 3));
                    ed.iv2 = std::max(tmpIV.at(j % 3), tmpIV.at((j + 1) % 3));
                    ed.ita = i; // Index of current triangle
                    ed.itb = -1; // Index of neighbour triangle not (yet) found
                    ed.isTerminalEdge = 0;
                }

                map.insert(key, ed);
            }
        }

        // Phase 3
        int k = 0; // Current pointer of edges
        for (QMap<QString, Edge>::iterator i(map.begin()); i != map.end(); i++, k++)
        {
            Edge e(i.value());
            edges.push_back(e);

            // Phase 4
            // Triangle A
            Triangle &ta = triangles.at(e.ita);
            /* If the Vertex "a" from the triangle is not in e.iv1 or e.iv2,
             * then this Vertex "a" is the opposite of the current Edge.
             */
            if (ta.iv1 != e.iv1 and ta.iv1 != e.iv2)
            {
                ta.ie1 = k;
            }
            else if (ta.iv2 != e.iv1 and ta.iv2 != e.iv2)
            {
                ta.ie2 = k;
            }
            else if (ta.iv3 != e.iv1 and ta.iv3 != e.iv2)
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

            if (tb.iv1 != e.iv1 and tb.iv1 != e.iv2)
            {
                tb.ie1 = k;
            }
            else if (tb.iv2 != e.iv1 and tb.iv2 != e.iv2)
            {
                tb.ie2 = k;
            }
            else if (tb.iv3 != e.iv1 and tb.iv3 != e.iv2)
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

bool OFFHandler::save(std::string filepath,
                      std::vector<Vertex> &vertices,
                      std::vector<Edge> &edges,
                      std::vector<Triangle> &triangles)
{
    int numVertices = vertices.size();
    int numTriangles = triangles.size();
    int numEdges = edges.size();

    QString qfilepath = QString::fromStdString(filepath);
    qDebug() << "Saving OFF file to" << qfilepath << endl;
    qDebug() << "(V, F, E) = " << numVertices << numTriangles << numEdges;

    QFile outputFile(qfilepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&outputFile);
        out << "OFF" << endl;
        out << "# File created by QLepp2D." << endl;

        out << numVertices << " " << numTriangles << " " << numEdges;

        // Write vertices
        unsigned long coordinatesPerVertex = vertices.size() / numVertices;
        for (unsigned long i(0); i < vertices.size(); i++)
        {
            if (i % coordinatesPerVertex == 0)
            {
                out << endl;
            }
            out << vertices.at(i).x << " ";
            out << vertices.at(i).y << " ";
            out << vertices.at(i).z;
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
