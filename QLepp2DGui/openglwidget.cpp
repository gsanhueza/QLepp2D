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

#include <openglwidget.h>

OpenGLWidget::OpenGLWidget(QWidget* parent)
  : QOpenGLWidget(parent),
    m_program(0),
    m_xRot(0),
    m_yRot(0),
    m_zRot(0),
    m_xCamPos(0),
    m_yCamPos(0),
    m_zCamPos(-5),
    m_dataAlreadyLoaded(false),
    m_model(nullptr)
{
}

OpenGLWidget::~OpenGLWidget()
{
    cleanup();
}

void OpenGLWidget::cleanup()
{
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void OpenGLWidget::setupVertexAttribs()
{
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);                        // Vertex
    f->glEnableVertexAttribArray(1);                        // Color
    // glVertexAttribPointer(GLuint index​, GLint size​, GLenum type​, GLboolean normalized​, GLsizei stride​, const GLvoid * pointer​);
    // index = Vertex(0) or Color(1), can be more if needed
    // size = Coordinates(x, y, z) => 3
    // type = GL_DOUBLE, as that's the type of each coordinate
    // normalized = false, as there's no need to normalize here
    // stride = 0, which implies that vertices are side-to-side (VVVCCC)
    // pointer = where is the start of the data (in VVVCCC, 0 = start of vertices and 3 * GL_DOUBLE * sizeof(vertexArray) = start of color)
    f->glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
    f->glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, reinterpret_cast<void *>(3 * sizeof(Vertex) * m_model->getTriangles().size()));
    m_vbo.release();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    generateGLProgram();
}

void OpenGLWidget::generateGLProgram()
{
    m_program = new QOpenGLShaderProgram;
    // QRC resource
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("color", 1);
    m_program->link();

    m_program->bind();
    m_modelViewMatrixLoc = m_program->uniformLocation("modelViewMatrix");
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();

    // Our camera has an initial position.
    m_camera.setToIdentity();
    m_camera.translate(m_xCamPos, m_yCamPos, m_zCamPos);

    m_program->release();
}

void OpenGLWidget::loadData()
{
    // Setup our vertex buffer object.
    m_vbo.create();
    m_vbo.bind();

    // Clear old geometry data from vector.
    m_data.clear();

    // Load vertices
    std::vector<Vertex> vertices(m_model->getVertices());
    std::vector<Triangle> triangles(m_model->getTriangles());

    for (Triangle &t : triangles)
    {
        m_data.append(vertices.at(t.iv1).x);
        m_data.append(vertices.at(t.iv1).y);
        m_data.append(vertices.at(t.iv1).z);

        m_data.append(vertices.at(t.iv2).x);
        m_data.append(vertices.at(t.iv2).y);
        m_data.append(vertices.at(t.iv2).z);

        m_data.append(vertices.at(t.iv3).x);
        m_data.append(vertices.at(t.iv3).y);
        m_data.append(vertices.at(t.iv3).z);
    }

    // Generate color
    for (Triangle &t : triangles)
    {
        for (int i(1); i <= 3; i++)
        {
            m_data.append( t.bad * 0.3 * i);
            m_data.append(!t.bad * 0.3 * i);
            m_data.append(0.0);
        }
    }

    // Allocate data into VBO
    m_vbo.allocate(m_data.constData(), m_data.count() * sizeof(GLdouble));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();
    m_dataAlreadyLoaded = true;
}

void OpenGLWidget::paintGL()
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);                                // Both sides

    m_world.setToIdentity();

    // Allow rotation of the world
    m_world.rotate(m_xRot / 16.0f, 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    // Bind VAO
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Bind data of shaders to program
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_modelViewMatrixLoc, m_camera * m_world);

    // Load new data only on geometry or shader change
    if (not m_dataAlreadyLoaded)
    {
        loadData();
    }

    // Draw triangulation
    glDrawArrays(GL_TRIANGLES, 0, m_data.count() / 6);     // Last argument = Number of vertices

    m_program->release();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 10000.0f);
}

void OpenGLWidget::updateData(Model *model)
{
    if (m_model == nullptr)
    {
        m_model = model;
    }

    m_dataAlreadyLoaded = false;
    update();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    }
    else if (event->buttons() & Qt::MiddleButton)
    {
        setXMovement(m_xCamPos + dx);
        setYMovement(m_yCamPos + dy);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    m_zCamPos += (event->delta() / 120);
    m_camera.setToIdentity();
    m_camera.translate(-m_xCamPos, m_yCamPos, m_zCamPos);
    update();
}

void OpenGLWidget::resetView()
{
    m_xRot = m_yRot = m_zRot = 0;
    m_xCamPos = m_yCamPos = 0;
    m_zCamPos = -5;
    m_camera.setToIdentity();
    m_camera.translate(-m_xCamPos, m_yCamPos, m_zCamPos);
    update();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void OpenGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot)
    {
        m_xRot = angle;
        update();
    }
}

void OpenGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot)
    {
        m_yRot = angle;
        update();
    }
}

void OpenGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot)
    {
        m_zRot = angle;
        update();
    }
}

void OpenGLWidget::setXMovement(int position)
{

    if (position != m_xCamPos)
    {
        m_xCamPos = position;
        m_camera.setToIdentity();
        m_camera.translate(-m_xCamPos, m_yCamPos, m_zCamPos);
        update();
    }
}

void OpenGLWidget::setYMovement(int position)
{

    if (position != m_yCamPos)
    {
        m_yCamPos = position;
        m_camera.setToIdentity();
        m_camera.translate(-m_xCamPos, m_yCamPos, m_zCamPos);
        update();
    }
}
