/** @file
 *
 * @par History
 */

#ifndef H_BSCRIPT_IMPSTR_H
#define H_BSCRIPT_IMPSTR_H

#ifndef BSCRIPT_BOBJECT_H
#include "bobject.h"
#endif

#include <stack>
#include <string>

namespace Pol
{
namespace Bscript
{
class SubString;

class String : public BObjectImp
{
  typedef BObjectImp base;

public:
  enum class Tainted
  {
    YES,  // performs unicode sanitize should be done for every external value
    NO    // performs no unicode sanitize should only be used for internal usage
  };
  String() : BObjectImp( OTString ), value_( "" ) {}
  String( const char* str, int nchars, Tainted san = Tainted::YES );
  explicit String( const char* str, Tainted san = Tainted::YES );
  explicit String( const std::string& str, Tainted san = Tainted::YES );
  explicit String( BObjectImp& objimp );
  String( const String& str ) : BObjectImp( OTString ), value_( str.value_ ) {}
  virtual ~String() {}

private:
  explicit String( const std::string& str, std::string::size_type pos, std::string::size_type n );

public:
  static bool isValidUnicode( const std::string& str );
  static bool sanitizeUnicode( std::string* str );

  static BObjectImp* unpack( const char* pstr );
  static BObjectImp* unpack( std::istream& is );
  static BObjectImp* unpackWithLen( std::istream& is );
  virtual std::string pack() const override;
  virtual void packonto( std::ostream& os ) const override;
  static void packonto( std::ostream& os, const std::string& value );
  virtual BObjectImp* copy() const override { return new String( *this ); }
  virtual size_t sizeEstimate() const override;

  // FIXME: Possibly eliminate this later and have [ ] operator support?
  // Or stick to functions, overload them with other substring getting methods.
  String* StrStr( int begin, int len );
  // FIXME: Possibly upgrade this later with overload functions in order to support different
  // trim methods, or add ELTrim, etc?
  String* ETrim( const char* CRSet, int type );
  void EStrReplace( String* str1, String* str2 );
  void ESubStrReplace( String* replace_with, unsigned int index, unsigned int len );

  const char* data() const { return value_.c_str(); }
  const std::string& value() const { return value_; }
  size_t length() const;
  void toUpper( void );
  void toLower( void );
  std::vector<unsigned int> toUTF32() const;
  std::vector<unsigned short> toUTF16() const;
  static std::string fromUTF32( unsigned int code );
  static std::string fromUTF16( std::vector<unsigned short> code );
  static std::string fromUTF16( unsigned short* code, size_t len );

  String& operator=( const char* s )
  {
    value_ = s;
    return *this;
  }
  String& operator=( const String& str )
  {
    value_ = str.value_;
    return *this;
  }
  void copyvalue( const String& str ) { value_ = str.value_; }

private:
  void remove( const char* s );
  virtual bool isTrue() const override { return !value_.empty(); }

public:
  virtual BObjectImp* selfPlusObjImp( const BObjectImp& objimp ) const override;
  virtual BObjectImp* selfPlusObj( const BObjectImp& objimp ) const override;
  virtual BObjectImp* selfPlusObj( const BLong& objimp ) const override;
  virtual BObjectImp* selfPlusObj( const Double& objimp ) const override;
  virtual BObjectImp* selfPlusObj( const String& objimp ) const override;
  virtual BObjectImp* selfPlusObj( const ObjArray& objimp ) const override;
  virtual void selfPlusObjImp( BObjectImp& objimp, BObject& obj ) override;
  virtual void selfPlusObj( BObjectImp& objimp, BObject& obj ) override;
  virtual void selfPlusObj( BLong& objimp, BObject& obj ) override;
  virtual void selfPlusObj( Double& objimp, BObject& obj ) override;
  virtual void selfPlusObj( String& objimp, BObject& obj ) override;
  virtual void selfPlusObj( ObjArray& objimp, BObject& obj ) override;

  virtual BObjectImp* selfMinusObjImp( const BObjectImp& objimp ) const override;
  virtual BObjectImp* selfMinusObj( const BObjectImp& objimp ) const override;
  virtual BObjectImp* selfMinusObj( const BLong& objimp ) const override;
  virtual BObjectImp* selfMinusObj( const Double& objimp ) const override;
  virtual BObjectImp* selfMinusObj( const String& objimp ) const override;
  virtual BObjectImp* selfMinusObj( const ObjArray& objimp ) const override;
  virtual void selfMinusObjImp( BObjectImp& objimp, BObject& obj ) override;
  virtual void selfMinusObj( BObjectImp& objimp, BObject& obj ) override;
  virtual void selfMinusObj( BLong& objimp, BObject& obj ) override;
  virtual void selfMinusObj( Double& objimp, BObject& obj ) override;
  virtual void selfMinusObj( String& objimp, BObject& obj ) override;
  virtual void selfMinusObj( ObjArray& objimp, BObject& obj ) override;

  virtual BObjectRef OperSubscript( const BObject& obj ) override;
  virtual BObjectRef OperMultiSubscript( std::stack<BObjectRef>& indices ) override;
  virtual BObjectRef OperMultiSubscriptAssign( std::stack<BObjectRef>& indices,
                                               BObjectImp* target ) override;

  int find( int begin, const char* target );
  unsigned int SafeCharAmt() const;


  virtual BObjectImp* array_assign( BObjectImp* idx, BObjectImp* target, bool copy ) override;
  int find( char* s, int* posn );

  virtual std::string getStringRep() const override { return value_; }
  virtual std::string getFormattedStringRep() const override { return "\"" + value_ + "\""; }
  virtual void printOn( std::ostream& ) const override;

  bool compare( const String& str ) const;
  bool compare( size_t pos1, size_t len1, const String& str ) const;
  bool compare( size_t pos1, size_t len1, const String& str, size_t pos2, size_t len2 ) const;

protected:
  virtual bool operator==( const BObjectImp& objimp ) const override;
  virtual bool operator<( const BObjectImp& objimp ) const override;

  virtual BObjectImp* call_method( const char* methodname, Executor& ex ) override;
  virtual BObjectImp* call_method_id( const int id, Executor& ex,
                                      bool forcebuiltin = false ) override;

private:
  size_t getBytePosition( std::string::const_iterator& itr, size_t codeindex ) const;

  std::string value_;
  friend class SubString;
};

class ConstString : public String
{
public:
  explicit ConstString( const std::string& str ) : String( str, Tainted::YES ) {}
};
}
}
#endif
