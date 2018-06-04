#!/usr/bin/octave -qf
# This script creates a random Delaunay mesh.

# Parse command line arguments
arg_list = argv();

if (size(arg_list) > 0)
    V = str2num(arg_list{1});
else
    printf("Usage: generator.m N\n");
    printf("  N = Number of points\n");
    exit(1);
endif

printf("Creating %d random points...\n", V);

x = rand (1, V);
y = rand (1, V);
z = zeros(1, V);

printf("Creating Delaunay mesh...\n");
tri = delaunay(x, y);

F = size(tri)(1);

printf("%d triangles have been created!\n", F);

# Euler's formula: V + F - E = 2
E = V + F - 2;

printf("Writing file...\n");

# Now we write our OFF file.
FILENAME = strcat("random", num2str(F), "T.off");
fid = fopen(FILENAME, "w");
fprintf(fid, "OFF\n");
fprintf(fid, "# File randomly generated using Octave's delaunay method,\n");
fprintf(fid, "%d %d %d\n", V, F, E);

# First we write the vertices' coordinates.
for (i = 1:V)
    fprintf(fid, "%f %f %f\n", x(i), y(i), z(i));
endfor

# Then, we write the pointers to vertices so we can get our triangles.
# Remember to offset the numbers, because Octave's arrays start at 1.
for (i = 1:F)
    fprintf(fid, "%d %d %d %d\n", 3, tri(i, 1) - 1,tri(i, 2) - 1, tri(i, 3) - 1);
endfor

fclose(fid);

printf("Finished!\n");
