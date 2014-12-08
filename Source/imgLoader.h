#ifndef IMGLOADER_H_INCLUDED
#define IMGLOADER_H_INCLUDED

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

GLuint loadPNG( std::string filename ){
  GLuint texture = SOIL_load_OGL_texture(
      filename.c_str(),
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT );

  if( !texture ){
    std::cout << "SOIL Error: " << SOIL_last_result() << std::endl;
  }else{
    std::cout << filename << " loaded successfully! ID: " << texture << std::endl;
  }

	return texture;
}

GLuint loadBMP( std::string filename ){
  GLuint texture = SOIL_load_OGL_texture(
        filename.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y );

  if( !texture ){
    std::cout << "SOIL Error: " << SOIL_last_result() << std::endl;
  }

	return texture;
}

#endif // IMGLOADER_H_INCLUDED
