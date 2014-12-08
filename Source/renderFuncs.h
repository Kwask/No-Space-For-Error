#ifndef RENDERFUNCS_H_INCLUDED
#define RENDERFUNCS_H_INCLUDED

#include <GLFW/glfw3.h>

#include "Rect.h"

void drawSquare( const Rect<GLfloat>* dest, const Rect<GLfloat>* clip, GLuint texture ){
  Rect<GLfloat> temp( 0, 0, 1, 1 );

  if( !clip ){
    clip = &temp;
  }

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glBegin( GL_TRIANGLE_FAN );
      glTexCoord2f( clip->x, clip->y+clip->h );
      glVertex2f( dest->x, dest->y );
      glTexCoord2f( clip->x, clip->y );
      glVertex2f( dest->x, dest->y+dest->h );
      glTexCoord2f( clip->x+clip->w, clip->y );
      glVertex2f( dest->x+dest->w, dest->y+dest->h );
      glTexCoord2f( clip->x+clip->w, clip->y+clip->h );
      glVertex2f( dest->x+dest->w, dest->y );
    glEnd();
  glBindTexture( GL_TEXTURE_2D, 0 );
  glDisable( GL_TEXTURE_2D );
}

void drawSquare( const Rect<GLfloat>* dest ){
  glBegin( GL_TRIANGLE_FAN );
    glColor3f( 1.f, 0.f, 0.f );
    glVertex2f( dest->x, dest->y );
    glColor3f( 0.f, 1.f, 0.f );
    glVertex2f( dest->x, dest->y+dest->h );
    glColor3f( 0.f, 0.f, 1.f );
    glVertex2f( dest->x+dest->w, dest->y+dest->h );
    glColor3f( 1.f, 1.f, 0.f );
    glVertex2f( dest->x+dest->w, dest->y );
  glEnd();
}

#endif // RENDERFUNCS_H_INCLUDED
