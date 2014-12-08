#ifndef GAMEFUNCS_H_INCLUDED
#define GAMEFUNCS_H_INCLUDED

#include <cstdio>

#include "Rect.h"

bool collision( const Rect<GLfloat>* a, const Rect<GLfloat>* b ){
  bool success = true;

  GLfloat a_top = a->y;
  GLfloat a_bottom = a->y+a->h;
  GLfloat a_left = a->x;
  GLfloat a_right = a->x+a->w;

  GLfloat b_top = b->y;
  GLfloat b_bottom = b->y+b->h;
  GLfloat b_left = b->x;
  GLfloat b_right = b->x+b->w;

  if( a_bottom <= b_top ){
    success = false;
  }else if( a_top >= b_bottom ){
    success = false;
  }else if( a_right <= b_left ){
    success = false;
  }else if( a_left >= b_right ){
    success = false;
  }

  return success;
}

void errorCallback( int error, const char* description ){
  fputs( description, stderr );
}

void halt( double time ){
  double start_time = glfwGetTime();
  double elapsed_time = glfwGetTime()-start_time;

  while( elapsed_time < time ){
    elapsed_time = glfwGetTime()-start_time;
  }
}

#endif // GAMEFUNCS_H_INCLUDED
