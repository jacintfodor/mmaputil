// A multimap_util osztály template szabványos multimapek kényelmes
// kezeléséhez: a konstruktorban megkapott adatszerkezettel kell dolgoznia, a
// replace_key-jel egy kulcsot lehet átcserélni egy másikra, swap-pel két
// kulcshoz tartozó értékeket lehet felcserélni, update_value-val a
// multimapben lévő értékeket lehet átírni, erase-zel a multimapből lehet
// törölni érték szerint, működjön együtt custom rendezéssel működő
// multimappel.

#include <map>
#include <vector>
#include <iostream>
//#include "mmaputil.h"
#include <algorithm>
#include <list>
#include <string>
#include "header/mmaputil.h"

class value
{

  int i;

public:

  value( int s ): i( s ) { }

  bool operator()( const std::pair<int, int>& p ) const
  {
    return p.second == i;
  }

};


struct string_size_less
{

  bool operator()( const std::string& lhs,
                   const std::string& rhs ) const
  {
    return lhs.size() < rhs.size();
  }

};


const int max = 1000;

bool check()
{
  std::multimap<int, int> s;
  multimap_util<int, int> su( s );
  for( int i = 0; i < max; ++i )
  {
    s.insert( std::make_pair( i, i ) );
    su.replace_key( i, 2 * i );
  }

  std::multimap<std::string, std::string> langs;
  langs.insert( std::make_pair( "C++", "<3" ) );
  langs.insert( std::make_pair( "C++", ":-D" ) );
  langs.insert( std::make_pair( "Python", ":-/" ) );
  langs.insert( std::make_pair( "Ada", ":-o" ) );
  langs.insert( std::make_pair( "C", ":-)" ) );

  multimap_util<std::string, std::string> langsu( langs );
  langsu.swap( "Ada", "Python" ).swap( "Ada", "Python" );
  langsu.replace_key( "Python", "Modula" ).replace_key( "Ada", "Haskell" );
  langsu.swap( "C", "C++" );

  if ( 1 != langs.count( "Haskell" ) || 0 != langs.count( "Ada" ) ||
       ":-)" != langs.find( "C++" )->second || 1U * max != s.size() ||
       ":-o" != langs.find( "Haskell" )->second || 5 != langs.size() ||
       2 * max - 2 != s.rbegin()->first || 0 >= s.count( max + 2 ) )
  {
    return false;
  }

  langs.insert( std::make_pair( "Scala", ":-/" ) );
  langs.insert( std::make_pair( "Scala", ":-S" ) );
  langsu.replace_value( ":-/", ":-(" );
  langsu.erase( ":-S" );

  su.replace_value( 1, 0 );

  if ( ":-(" != langs.find( "Modula" )->second || 1U * max != s.size() ||
       ":-(" != langs.find( "Scala" )->second || 1 != langs.count( "Scala" ) ||
       0 != std::count_if( s.begin(), s.end(), value( 1 ) ) ||
       2 != std::count_if( s.begin(), s.end(), value( 0 ) ) )
  {
    return false;
  }

  std::multimap<std::string, std::string, string_size_less> lss;
  lss.insert( std::make_pair( "C++", "8-)" ) );
  lss.insert( std::make_pair( "C++", ":-)" ) );
  lss.insert( std::make_pair( "SQL", "o.O" ) );
  lss.insert( std::make_pair( "Ada", ":-/" ) );

  multimap_util<std::string, std::string, string_size_less> lssu( lss );
  lssu.replace_key( "C++", "Ada" ).replace_key( "Simula", "Java" );

  return ( 4 == lss.size() && 0 == lss.count( "Java") );
}

int main()
{
  std::cout
    << "Your solution is " << (check() ? "" : "not ")
    << "ready for submission.\n";
}
