import struct
import os
import bpy

C = bpy.context
if C.active_object.type != 'MESH' and C.active_object.data.uv_layers:
  print('Active object must be a model with uvs!')
  print('The current type is ' + C.active_object.type)
else:
  with open("/Users/marcus/Programming/001MARCUS/bin/deleteme.dat", 'wb') as f:
    mesh = C.active_object.data
    mesh.calc_loop_triangles()
    mesh.flip_normals()
    verts = []
    for tri in mesh.loop_triangles:
      for i in range(3):
        vert_index = tri.vertices[i]
        loop_index = tri.loops[i]
        pos = mesh.vertices[vert_index].co
        nrm = mesh.loops[loop_index].normal
        uv_layer = mesh.uv_layers.active.data
        uv = uv_layer[loop_index].uv
        
        verts.append((pos, nrm,  uv))
    count = struct.pack('Q', len(verts))
    f.write(count)
    for vertex in verts:
      print(vertex)
      vert = struct.pack('ffffffff', *vertex[0], *vertex[1], *vertex[2])
      f.write(vert)
    
    f.close()
