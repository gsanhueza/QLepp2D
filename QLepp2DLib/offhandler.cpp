#include <QDebug>
#include <QFile>
#include "offhandler.h"

OFFHandler::OFFHandler()
{
}

bool OFFHandler::loadOffFile(QString &filepath, std::vector<Vertex> &vertices, std::vector<int> &indices, std::vector<Triangle> &triangles)
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
            qWarning("Not an OFF file");
            inputFile.close();
            return false;
        }

        // Old data cleanup
        vertices.clear();
        indices.clear();

        // Skip comments
        do
        {
            line = in.readLine();
        } while (line.contains("#") or line.size() == 0);

        // Read file metadata (vertices, faces, edges)
        QStringList metadata = line.split(" ");

        m_numVertices = metadata.at(0).toInt();
        m_numFaces = metadata.at(1).toInt();
        m_numEdges = metadata.at(2).toInt();

        qDebug() << "Number of vertices:" << m_numVertices;
        qDebug() << "Number of faces:" << m_numFaces;
        qDebug() << "Number of edges:" << m_numEdges;

        // Read vertices data
        for (int i(0); i < m_numVertices; i++)
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
        for (int i(0); i < m_numFaces; i++)
        {
            line = in.readLine();
            QStringList mappedIndices = line.split(" ", QString::SkipEmptyParts);
            // We check and skip the first one, because it marks the amount of indices, not the index itself.
            indices.push_back(mappedIndices.at(1).toInt());
            indices.push_back(mappedIndices.at(2).toInt());
            indices.push_back(mappedIndices.at(3).toInt());

            Triangle t;
            t.v1 = vertices.at(mappedIndices.at(1).toInt());
            t.v2 = vertices.at(mappedIndices.at(2).toInt());
            t.v3 = vertices.at(mappedIndices.at(3).toInt());
            t.bad = 0;
            triangles.push_back(t);
        }

        inputFile.close();
        return true;
    }
    return false;
}

bool OFFHandler::saveOffFile(QString &filepath, std::vector<Vertex> &vertices, std::vector<int> &indices) const
{
    qDebug() << "Saving OFF file to" << filepath << endl;

    qWarning() << "(V, F, E) = " << m_numVertices << m_numFaces << m_numEdges;
    qWarning() << "coordinatesPerVertex = " << vertices.size() / m_numVertices;

    QFile outputFile(filepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&outputFile);
        out << "OFF" << endl;
        out << "" << endl;
        out << m_numVertices << " " << m_numFaces << " " << m_numEdges;

        // Write vertices
        int coordinatesPerVertex = vertices.size() / m_numVertices;
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
        int verticesPerFace = indices.size() / m_numFaces;
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
