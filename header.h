/*
 * =====================================================================================
 *
 *       Filename:  header.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/02/12 09时28分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef FND_HEADER_H
#define FND_HEADER_H
#include <cstdint>
#include <string>

union HeaderCell {
  int32_t value ;
  char data[sizeof int32_t / sizeof char] ;
} ;
union HeaderUnion {
  int32_t Total[4] ; 
  char TotalData[4 * sizeof(int32_t) / sizeof(char)] ;
  struct Group {
    HeaderCell group ;
    HeaderCell id ;
    union Inner {
      int32_t InnerTotal[2] ;
      char InnerTotalData[2 * sizeof(int32_t) / sizeof(char)] ;
      struct InnerStruct {
        HeaderCell type ;
        HeaderCell size ;
      } ;
    Inner TypeSize ;
    } ;
  } ;

  Group Info ;
} ;

class Header {
  private:
    HeaderUnion m_data ;
  public:
    int32_t group() const { return m_data.Info.group.value ;}
    int32_t id() const { return m_data.Info.id.value ;}
    int32_t type() const { return m_data.Info.TypeSize.type.value ;}
    int32_t size() const { return m_data.Info.TypeSize.size.value ;}
    explicit Header(int32_t g = 0, int32_t i = 0, int32_t type = 0, int32_t s = 0) {
      m_data.Total[0] = g ;
      m_data.Total[1] = i ;
      m_data.Total[2] = type ;
      m_data.Total[3] = s ;
    }

    void parseFromString(const std::string& s) {
      
    }
    std::string packToString() const ;

} ;
#endif
