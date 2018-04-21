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

#ifndef QLEPP2DLIB_GLOBAL_H
#define QLEPP2DLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QLEPP2DLIB_LIBRARY)
#  define QLEPP2DLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QLEPP2DLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QLEPP2DLIB_GLOBAL_H
