#ifndex _IO_CLASSES_H_
#define _IO_CLASSES_H_

class CharDevice {
 public:
  int putc ( int );
  int getc ( void );
  int open ( void );
  int close ( void );
  int seek ( long int offset, int origin );
}

#endif /* _IO_CLASSES_H_ */
