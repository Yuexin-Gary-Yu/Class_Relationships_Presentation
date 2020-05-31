#ifndef RELATIONSHIPS_H
#define RELATIONSHIPS_H
///////////////////////////////////////////////////////////////
//  relatonships.h - demonstrate basic class relationships   //
///////////////////////////////////////////////////////////////
//                                                           //
//  This is a demonstration program so it does not have all  //
//  the features of a module, e.g.:                          //
//    - full prologue                                        //
//    - manual page                                          //
//    - maintenance page                                     //
//                                                           //
///////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>

//----< formatting helper global function >--------------------

void formatOut() {
  std::cout << std::setfill('.'); 
  std::cout.setf(std::ios::left,std::ios::adjustfield);
}

//
///////////////////////////////////////////////////////////////
//  class C will be composed by class B                      //
///////////////////////////////////////////////////////////////
//                                                           //
//  class operations:                                        //
//    construct object                                       //
//    destroy object                                         //
//    show value of its integer state                        //
//    get and set value of integer state through reference   //
//                                                           //
///////////////////////////////////////////////////////////////

class C
{
  public:
    C(int value);
    C(const C& c) = default;
    C& operator=(const C& c) = default;
    ~C();
    void show();
    int& value();
    
  private:
    int val;
};

//----< promotion constructor >--------------------------------

inline C::C(int value=0) : val(value) {}

//----< destructor >-------------------------------------------

inline C::~C() {}

//----< display object state >---------------------------------

inline void C::show()
{
  long save = std::cout.flags();
  formatOut();
  std::cout << "\n  " << std::setw(30) 
            << "Composed object speaking." << "My value is " 
            << val; 
  std::cout.flags(save);
}

//----< return composed object >-------------------------------

inline int& C::value() { return val; }

//
///////////////////////////////////////////////////////////////
//  class U is used by derived class D                       //
///////////////////////////////////////////////////////////////
//                                                           //
//  class operations:                                        //
//    construct object                                       //
//    destroy object                                         //
//    show value of its string state                         //
//    get and set value of string state through reference    //
//                                                           //
///////////////////////////////////////////////////////////////

class U
{
  public:
    U(const std::string &s);
    U(const U& u) = default;
    U& operator=(const U& u) = default;
    ~U();
    void show();
    std::string& value();

  private:
    std::string val;  
};

//----< promotion constructor >--------------------------------

inline U::U(const std::string &s) : val(s) {}    // note Ctor initialization sequence

//----< destructor >-------------------------------------------

inline U::~U() {}

//----< display object state >---------------------------------

inline void U::show()
{ 
  long save = std::cout.flags();
  formatOut();
  std::cout << "\n  " << std::setw(30) 
            << "Used object speaking." << "My value is \"" 
            << val.c_str() << "\""; 
  std::cout.flags(save);
}

//----< return state value >-----------------------------------

inline std::string& U::value() { return val; }

//
///////////////////////////////////////////////////////////////
//  class B composes C and is the base for D                 //
///////////////////////////////////////////////////////////////
//                                                           //
//  class operations:                                        //
//    construct object                                       //
//    destroy object                                         //
//    show value of its C object state                       //
//    get and set value of C state through reference         //
//                                                           //
///////////////////////////////////////////////////////////////

class B
{
  public:
    B(const C &arg);
    B(const B& b) = default;
    B& operator=(const B& b) = default;
    virtual ~B();
    virtual void show();
    C& value();

  private:
    C c;    // here is composition
};

//----< promotion constructor >--------------------------------

inline B::B(const C &arg) : c(arg) {}            // note Ctor initialization sequence

//----< destructor >-------------------------------------------

inline B::~B() {}

//----< display object state >---------------------------------

inline void B::show()
{ 
  long save = std::cout.flags();
  formatOut();
  std::cout << "\n  " << std::setw(30) 
            << "Base object speaking." << "My value is " 
            << value().value(); 
  std::cout.flags(save);
}

//----< return reference to composed object >------------------

inline C& B::value() { return c; }

//
///////////////////////////////////////////////////////////////
//  class D is derived from B and uses U                     //
///////////////////////////////////////////////////////////////
//                                                           //
//  class operations:                                        //
//    construct object                                       //
//    destroy object                                         //
//    accept a pointer to a U object                         //
//    show value of its inherited A state                    //
//    get and set value of inherited state through reference //
//                                                           //
///////////////////////////////////////////////////////////////

class D : public B    // here is where D derives from B
{
  public:
    D(const C &arg);
    D(const D &d);
    virtual ~D();
    D& operator=(const D &d);
    void setU(U *ptr);
    virtual void show();
    void showParts();
  private:
    U *pU;   // pU points to a used object, e.g., D did not
};           // create it.  Supports using relationship

//----< promotion constructor >--------------------------------

inline D::D(const C &arg) : B(arg), pU(nullptr) { }    // note Ctor initialization sequence

//----< copy constructor >-------------------------------------

inline D::D(const D &d) : B(d), pU(d.pU)         // note Ctor initialization sequence
{ 
  // copying d's pointer to U may or may not be what you want this
  // constructor to do.  You might want to null the new object's
  // U pointer.  The copy action used here means that this object
  // shares the U object with d.  
}
//----< assignment operator >----------------------------------

inline D& D::operator=(const D& d)
{
  // we don't need to implement this, because it does
  // the same operations as the default assignment operator

  if (this != &d)
  {
    B::operator=(d);
    pU = d.pU;  // see note above
  }
  return *this;
}

//----< destructor >-------------------------------------------

inline D::~D() { /* D instances use, but don't own *pU so don't delete */ }
    
//
///////////////////////////////////////////////////////////////
//  class Client aggregates D                                //
///////////////////////////////////////////////////////////////
//                                                           //
//  class operations:                                        //
//    construct object                                       //
//    destroy object                                         //
//    accept a pointer to a U object                         //
//    show value of its inherited A state                    //
//    get and set value of inherited state through reference //
//                                                           //
///////////////////////////////////////////////////////////////

class Client
{
public:
  Client();
  Client(const C& c) = delete;
  Client& operator=(const C& c) = delete;
  void show();
  ~Client();
private:
  D* pD;  // aggregation relationship
};

inline Client::Client()
{
  C c(42);
  pD = new D(c);
}

inline Client::~Client()
{
  delete pD;
}

#endif
