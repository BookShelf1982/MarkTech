import struct
import os
import bpy

# TODO: FIX PACKING.

C = bpy.context
if C.active_object.type != 'MESH' and C.active_object.data.uv_layers:
  print('Active object must be a model with uvs!')
  print('The current type is ' + C.active_object.type)
else:
  with open("/Users/marcus/Programming/001MARCUS/bin/deleteme.dat", 'wb') as f:
    mesh = C.active_object.data
    vert_map = {}
    indexed_verts_data = []
    indices = []
    for poly in mesh.polygons:
      for loop_index in poly.loop_indices:
        pos = mesh.vertices[mesh.loops[loop_index].vertex_index].co
        uv_layer = mesh.uv_layers.active.data
        uv = uv_layer[loop_index].uv 
        uv_rounded = (round(uv.x, 6), round(uv.y, 6))
        vert_key = (round(pos.x, 6), round(pos.y, 6), round(pos.z, 6), uv_rounded[0], uv_rounded[1])
        if vert_key not in vert_map:
          # If unique, add to the list of indexed vertices and map
          new_index = len(indexed_verts_data)
          vert_map[vert_key] = new_index
          # Store position (x, y, z) and UV (u, v) in a flat list or similar structure
          indexed_verts_data.append((pos.x, pos.y, pos.z, uv.x, uv.y))
          indices.append(new_index)
        else:
          # If already exists, use the existing index
          indices.append(vert_map[vert_key])
    counts = struct.pack('QQ', len(indices), len(indexed_verts_data))
    f.write(counts)
    for index in indices:
      idx = struct.pack('I', index)
      f.write(idx)
    for vertex in indexed_verts_data:
      print(vertex)
      vert = struct.pack('fffff', *vertex)
      f.write(vert)
    
    f.close()
