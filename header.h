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
 *         Author:  Fang Dongheng (fndisme), fndisme@163.com 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef FND_HEADER_H
#define FND_HEADER_H
#include <cstdint>
#include <cstring>
#include <string>

union HeaderCell {
  int32_t value ;
  char data[sizeof(int32_t) / sizeof(char)] ;
} ;
union HeaderUnion {
  int32_t Total[4] ; 
  char TotalData[4 * sizeof(int32_t) / sizeof(char)] ;
  struct Group {
    HeaderCell group ;
    HeaderCell id ;
    union InnerUnion {
      int32_t InnerTotal[2] ;
      char InnerTotalData[2 * sizeof(int32_t) / sizeof(char)] ;
      struct InnerStruct {
        HeaderCell type ;
        HeaderCell size ;
      } ;
    InnerStruct TypeSize ;
    } ;
    InnerUnion Inner;
  } ;

  Group Info ;
} ;

/**
 * @brief Header is the header information of the message.
 *
 * message is group by header and body.
 * header is four part.
 * (1) group. This is used for server information
 * (2) id. This is used for per client information
 * (3) type. This is used for message type
 * (4) messageSize. This is used for body size
 *
 * client send to server is short header.
 * server send to server is long header.
 */
class Header {
  private:
    HeaderUnion m_data ;
    bool m_isShort = false;
  public:
    bool operator == (const Header& rhs) const {
      return memcmp(m_data.TotalData, rhs.m_data.TotalData, size()) == 0 &&
        m_isShort == rhs.m_isShort ;
    }
    int32_t group() const { return m_data.Info.group.value ;}
    int32_t id() const { return m_data.Info.id.value ;}
    int32_t type() const { return m_data.Info.Inner.TypeSize.type.value ;}
    int32_t messageSize() const { return m_data.Info.Inner.TypeSize.size.value ;}
    explicit Header(int32_t g = 0, int32_t i = 0, int32_t type = 0, int32_t s = 0) {
      m_data.Total[0] = g ;
      m_data.Total[1] = i ;
      m_data.Total[2] = type ;
      m_data.Total[3] = s ;
    }

    void setGroupId(int32_t g, int32_t i) {
      m_data.Info.id.value = i ;
      m_data.Info.group.value = g ;
      m_isShort = false ;
    }

    void setMessageInformation(int32_t type, int32_t s) {
      m_data.Info.type.value = type ;
      m_data.Info.size.value = s ;
    }

    /**
     * @brief set header is short header
     *
     * set group id zero
     * make flag m_isShort = true
     */
    void setShort() {
      m_data.Info.id.value = 0 ;
      m_data.Info.group.value = 0 ;
      m_isShort = true ;
    }

    bool isShort() const {
      return m_isShort ;
    }
    

    static constexpr int longSize() { return sizeof(HeaderUnion) ;}

    static constexpr int size() { return longSize() ;}
    static constexpr int shortSize() { return sizeof(HeaderUnion::Group::InnerUnion) ;}

    /**
     * @brief init long header with string.
     *
     * if successed, header is long. type messageSize group id are valid.
     * isShort() == false
     *
     * @param s source of data. must s.size() >= longSize()
     *
     * @return  true for become long. otherwize unchanged....
     */
    bool parseFromString(const std::string& s) {
      if(s.size() < size()) return false ;
      memcpy(m_data.TotalData, s.data(), size()) ;
      m_isShort = false ;
      return true ;
    }

    /**
     * @brief init short header with string. 
     *
     * if successed, header is short. and type messageSize are valid.
     * group and id are setted to zero 
     *
     * @param s soure of data. must is large enough for shortSize (s.size() >=
     * shortSize())
     *
     * @return true for become short. otherwize unchanged....
     */
    bool parseFromShortString(const std::string& s) {
      if(s.size() < shortSize()) return false ;
      setShort() ;
      memcpy(m_data.Info.Inner.InnerTotalData, s.data(), shortSize()) ;
      return true ;
    }
    /**
     * @brief same with packToLongString
     *
     * @return string block can parse back to Header
     *
     * @seealso packToLongString
     */
    std::string packToString() const {
      return packToLongString() ;
    }

    /**
     * @brief pack message to string
     *
     * all the part is parsed( group id type messageSize isShort)
     *
     * @return string block can parse back to Header
     */
    std::string packToLongString() const {
      return std::string(std::begin(m_data.TotalData), std::end(m_data.TotalData));
    }

    std::string packToShortString() const {
      return std::string(std::begin(m_data.Info.Inner.InnerTotalData),
          std::end(m_data.Info.Inner.InnerTotalData)) ;
    }

    void appendFullDataToString(std::string* data) const {
      // FIXME maybe not fast....
      *data += packToLongString() ;
    }

    void appendShortDataToString(std::string* data) const {
      // FIXME maybe not fast....
      *data += packToShortString() ;
    }

    size_t parseFullFromData(const void* data, size_t dataSize) {
      if(dataSize < size()) return 0;

      memcpy(fullData(), data, size()) ;
      m_isShort = false ;
      return size() ; 
    }

    size_t parseShortFromData(const void* data, size_t dataSize) {
      if(dataSize < shortSize()) return 0;
      setShort() ;
      memcpy(shortData(), data, shortSize()) ;
      return shortSize();
    }

    size_t appendFullData(void* data, size_t dataSize) const {
      if(dataSize < size()) return 0;
      memcpy(data, fullData(), size()) ;
      return size();
    }

    size_t appendShortData(void* data, size_t dataSize) const {
      if(dataSize < shortSize()) return 0;
      memcpy(data, shortData(), shortSize()) ;
      return shortSize() ;
    }

    private:
    const void* fullData() const {
      return m_data.TotalData ;
    }

    void* fullData() {
    return m_data.TotalData ;
    }

    const void* shortData() const {
    return m_data.Info.Inner.InnerTotalData ;
    }

    void* shortData() {
    return m_data.Info.Inner.InnerTotalData ;
    }

} ;
#endif
