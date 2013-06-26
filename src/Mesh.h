#ifndef MESH_H
#define MESH_H

namespace Mesh {

  Mesh* loadObj(const std::string filename) {
    std::ifstream in(filename.c_str());

    if(!in.good())
    {
      std::cout  << "ERROR: loading obj:(" << filename << ") \n";
      exit(0);
    }

    char buffer[256], str[255];
    float f1,f2,f3;

    std::vector<glm::vec3> verts;
    std::vector<Face> faces;

    while(!in.getline(buffer,255).eof())
    {
      buffer[255] = '\0';

      sscanf_s(buffer,"%s",str,255);

      // reading a vertex
      if (buffer[0]=='v' && (buffer[1]==' '  || buffer[1]==32) )
      {
        if ( sscanf(buffer,"v %f %f %f",&f1,&f2,&f3)==3 )
        {
          verts.push_back(glm::vec3(f1,f2,f3));
        }
        else
        {
          std::cout << "ERROR: vertex format" << "\n";
          exit(-1);
        }
      }

      // reading FaceMtls 
      else if (buffer[0]=='f' && (buffer[1]==' ' || buffer[1]==32) )
      {
        Face f;
        int nt = sscanf(buffer,"f %u %u %u",&f.m_v.x,&f.m_v.y,&f.m_v.z);
        if( nt!=3 )
        {
          std::cout << "ERROR: FaceMtl format" << "\n";
          exit(-1);
        }
      
        f.m_v -= 1;   // start at index 0
        faces.push_back(f);
      }
    }
  
    // construct Mesh

    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));

    size_t vertsMemSize = verts.size()*sizeof(glm::vec3);
    mesh->m_verts = (glm::vec3*)malloc(vertsMemSize);
    memcpy(mesh->m_verts, verts.data(), vertsMemSize);
    mesh->m_numVerts = verts.size();

    size_t facesMemSize = faces.size()*sizeof(Face);
    mesh->m_faces = (Face*)malloc(facesMemSize);
    memcpy(mesh->m_faces, faces.data(), facesMemSize);
    mesh->m_numFaces = faces.size();

    // calc bounding box
    mesh->m_bmin = glm::vec3(FLT_MAX);
    mesh->m_bmax = glm::vec3(FLT_MIN);
    for (int i=0; i<mesh->m_numVerts; ++i) {
      mesh->m_bmin.x = glm::min(mesh->m_bmin.x, mesh->m_verts[i].x);
      mesh->m_bmin.y = glm::min(mesh->m_bmin.y, mesh->m_verts[i].y);
      mesh->m_bmin.z = glm::min(mesh->m_bmin.z, mesh->m_verts[i].z);

      mesh->m_bmax.x = glm::max(mesh->m_bmax.x, mesh->m_verts[i].x);
      mesh->m_bmax.y = glm::max(mesh->m_bmax.y, mesh->m_verts[i].y);
      mesh->m_bmax.z = glm::max(mesh->m_bmax.z, mesh->m_verts[i].z);
    }

    //for (int i=0; i<mesh->m_numVerts; ++i)
    //  std::printf("%i f %f %f %f\n", i, mesh->m_verts[i].x, mesh->m_verts[i].y, mesh->m_verts[i].z);
    //for (int i=0; i<mesh->m_numFaces; ++i)
    //  std::printf("%i f %u %u %u\n", i, mesh->m_faces[i].m_v[0]+1, mesh->m_faces[i].m_v[1]+1, mesh->m_faces[i].m_v[2]+1);

    std::printf("Loaded \"%s\" %u verts %u faces\n", filename.c_str(), verts.size(), faces.size());

    return mesh;
  }

}

#endif  // MESH_H