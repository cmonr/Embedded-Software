#ifndef _IO_CLASSES_H_
#define _IO_CLASSES_H_

class CharDevice {
 public:
  virtual int putc ( int ) = 0;
  virtual int getc () = 0;
  virtual int open () = 0;
  virtual int close () = 0;
  virtual int seek ( long int offset, int origin ) = 0;
};

#endif /* _IO_CLASSES_H_ */
