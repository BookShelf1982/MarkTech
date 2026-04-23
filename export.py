import struct
import os
import bpy
import bmesh
import mathutils

def triangulate(me):
  bm = bmesh.new()
  bm.from_mesh(me)
  bmesh.ops.triangulate(bm)
  bm.to_mesh(me)
  bm.free()

C = bpy.context

if C.active_object.type != 'MESH' and C.active_object.data.uv_layers:
  print('Active object must be a model with uvs!')
  print('The current type is ' + C.active_object.type)
else:
  with open("/Users/marcus/Programming/001MARCUS/bin/deleteme.dat", 'wb') as f:
    mesh = C.active_object.data
    
    mesh.calc_loop_triangles()
    #triangulate(mesh)

    match mesh.normals_domain:
      case 'POINT':
        print("POINT NORMALS")
      case 'FACE' | 'CORNER':
        print("FACE OR CORNER NORMALS")
        
    verts = []
    for tri in mesh.loop_triangles:
      idx = [0, 2, 1]
      for i in idx:
        vert_index = tri.vertices[i]
        loop_index = tri.loops[i]
        pos = mesh.vertices[vert_index].co
        nrm = None
        match mesh.normals_domain:
          case 'POINT':
            nrm = mesh.vertex_normals[vert_index].vector.copy()
          case 'FACE' | 'CORNER':
            nrm = mesh.corner_normals[loop_index].vector.copy()

        print(nrm, nrm.magnitude)
        if nrm.magnitude > 1:
          nrm.normalize()
        
        uv_layer = mesh.uv_layers.active.data
        uv = uv_layer[loop_index].uv
        
        verts.append((pos, nrm,  uv))
    count = struct.pack('Q', len(verts))
    f.write(count)
    for vertex in verts:
      #print(vertex)
      vert = struct.pack('ffffffff', *vertex[0], *vertex[1], *vertex[2])
      f.write(vert)
    
    f.close()
