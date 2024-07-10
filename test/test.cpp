//
// Created by abris on 7/6/24.
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN  // in only one cpp file
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(first_test)
{
int i = 1;
BOOST_TEST(i);
BOOST_TEST(i == 2);
}