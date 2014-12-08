#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

template <class T>
class Rect{
public:
  T x;
  T y;
  T w;
  T h;

  Rect(){

  }

  Rect( T x, T y, T w, T h ){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
  }

  virtual ~Rect(){

  }

  void set( T x, T y, T w, T h ){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
  }

};

#endif // RECT_H_INCLUDED
