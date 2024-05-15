#ifndef __mem_buf_h
#define __mem_buf_h
#include <stdexcept> //std::runtime_error
#include <stdlib.h>  //malloc free
#include <string.h>  //size_t
#include <string>

namespace hdiff_private {

struct TAutoMem {
  inline explicit TAutoMem(size_t size = 0)
      : _data(0), _data_end(0), _capacity_end(0) {
    realloc(size);
  }
  inline ~TAutoMem() { clear(); }
  inline unsigned char *data() { return _data; }
  inline const unsigned char *data() const { return _data; }
  inline unsigned char *data_end() { return _data_end; }
  inline const unsigned char *data_end() const { return _data_end; }
  inline size_t size() const { return (size_t)(_data_end - _data); }
  inline size_t capacity() const { return (size_t)(_capacity_end - _data); }
  inline bool empty() const { return (_data_end == _data); }
  inline void clear() {
    if (_data) {
      free(_data);
      _data = 0;
      _data_end = 0;
      _capacity_end = 0;
    }
  }
  inline void realloc(size_t newSize) {
    if (newSize <= capacity()) {
      _data_end = _data + newSize;
    } else {
      unsigned char *_new_data = (unsigned char *)::realloc(_data, newSize);
      if (_new_data == 0)
        throw std::runtime_error("TAutoMem::TAutoMem() realloc() error!");
      _data = _new_data;
      _data_end = _new_data + newSize;
      _capacity_end = _data_end;
    }
  }
  inline void reduceSize(size_t reserveSize) {
    if (reserveSize <= capacity())
      _data_end = _data + reserveSize;
    else
      throw std::runtime_error("TAutoMem::reduceSize() error!");
  }

private:
  unsigned char *_data;
  unsigned char *_data_end;
  unsigned char *_capacity_end;
};

} // namespace hdiff_private
#endif //__mem_buf_h