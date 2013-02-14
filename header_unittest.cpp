/*
 * =====================================================================================
 *
 *       Filename:  header_unittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年02月12日 11时50分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fang Dongheng (fndisme), fndisme@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "header.h"
#include <gtest/gtest.h>

TEST(Header, init) {
  Header header ;
  EXPECT_EQ(0, header.group()) ;
  EXPECT_EQ(0, header.id());
  EXPECT_EQ(0, header.type()) ;
  EXPECT_EQ(0, header.messageSize()) ;
  EXPECT_EQ(16, header.size()) ;

  Header header2(1,2,3,4) ;
  EXPECT_EQ(1, header2.group()) ;
  EXPECT_EQ(2, header2.id()) ;
  EXPECT_EQ(3, header2.type()) ;
  EXPECT_EQ(4, header2.messageSize()) ;
}

TEST(Header, stringPackParse) {
  Header header(1,2,3,4) ;
  std::string info = header.packToString() ;

  Header header2 ;
  bool ok = header2.parseFromString(info) ;
  EXPECT_EQ(true, ok) ;
  EXPECT_TRUE(header == header2) ;
}

TEST(Header, voidPackParse) {
  char data[Header::size()] ;

  Header header(1,2,3,4) ;
  bool ok = header.appendFullData(data, Header::size()) ;

  EXPECT_EQ(true, ok);
  EXPECT_EQ(false, header.isShort()) ;

  Header header2 ;
  ok = header2.parseFullFromData(data, Header::size()) ;
  EXPECT_EQ(true, ok) ;
  EXPECT_TRUE(header == header2) ;

  header.setShort() ;
  EXPECT_EQ(0, header.group()) ;
  EXPECT_EQ(0, header.id()) ;
  EXPECT_EQ(3, header.type()) ;
  EXPECT_EQ(4, header.messageSize()) ;
  EXPECT_EQ(true, header.isShort()) ;

  ok = header.appendShortData(data, Header::shortSize()) ;
  EXPECT_EQ(true, ok) ;
  ok = header2.parseShortFromData(data, Header::shortSize()) ;
  EXPECT_EQ(true, ok) ;
  EXPECT_TRUE(header == header2) ;
}



