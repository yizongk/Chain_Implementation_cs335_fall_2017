// Yi Zong Kuang
// A Chain object simple stores a sequence of a certain type. It uses an array as its underlying data storage.
//It has simple constructor to make small chains, but can also read input to make larger chains, the input must be in this format:
//[size: element_1 element_2 ...]

#ifndef CSCI335_HOMEWORK1_CHAIN_
#define CSCI335_HOMEWORK1_CHAIN_

#include <iostream>
#include <cstddef>
#include <sstream>

namespace teaching_project {

template<typename Object>
class Chain {
 public:
  // Zero-parameter constructor.
  Chain() {
    size_ = 0;
    array_ = nullptr; 
  }

  // Copy-constructor.
  Chain(const Chain &rhs) {
    this->size_ = rhs.size_;
    this->array_ = new Object[rhs.size_]; 
  
    for (unsigned int i = 0; i < rhs.size_; ++i) {
      this->array_[i] = rhs.array_[i];
    } 
  }

  // Copy-assignment.
  Chain& operator=(const Chain &rhs){			
    if (this != &rhs) {
      Chain copy = rhs;	
      std::swap(*this, copy);
      return *this;
    }
    return *this;
  }

  // Move-constructor. 
  Chain(Chain &&rhs) : size_(rhs.size_),
                       array_(rhs.array_) {	
    rhs.array_ = nullptr;
    rhs.size_ = 0;
  }

  // Move-assignment.
  // Just use std::swap() for all variables.
  Chain& operator=(Chain &&rhs) {
    std::swap(size_, rhs.size_);
    std::swap(array_, rhs.array_);
    rhs.size_ = 0;
    delete[] rhs.array_;
    rhs.array_ = nullptr;
    return *this;
  }

  ~Chain() {
    size_ = 0;
    delete[] array_; //array version of delete...This gave me 3 day worth of headache, trying to figure out what's causing invalid pointer error...  
  }

  // End of big-five.

  // One parameter constructor.
  Chain(const Object& item) {
    size_ = 1;
    array_ = new Object[1];
    array_[0] = item;
  }

  //constructor w/ 2 chain argument helping operator+ to concatenating .
  Chain(const Chain& a, const Chain& b) {
    this->size_ = a.size_ + b.size_;
    this->array_ = new Object[this->size_];
 
    for (unsigned int i = 0; i < a.size_; ++i) { 
      this->array_[i] = a.array_[i];
    }
    for (unsigned int i = a.size_; i < this->size_; ++i) {
      this->array_[i] = b.array_[i - a.size_];	
    }
  }

  //helps ReadChain() decide on what type to store into array_.
  Object setCorrectDataType(const std::string& data) {
    Object temp;
    std::istringstream(data) >> temp;
    return temp;
  }
  
  // Read a chain from standard input.
  void ReadChain() {	
    //Read one line from input.
    std::string input = "";
    std::getline(std::cin, input);

    //Find out size of chain from input.
    std::string intFromInput = "";
    for (unsigned int i = 1; input[i] != ':'; ++i) {
      intFromInput += input[i];
    }
    //c++11 way of parsing string into int.
    size_ = std::stoull(intFromInput);   
  
    //clearing array just in case, and resetting the whole array.
    delete[] array_;
    array_ = new Object[size_];

    //set an iterator to the input's next char after ':', 
    std::string::iterator itr = input.begin();
    unsigned long pos = input.find_first_of(":");
    //advancing the iterator
    if (pos != std::string::npos) { //if pos returned from find_first_of is not end of string
      std::advance(itr, pos);
      ++itr;  //move it past ':'.
    } else {
      std::cout << "Wrongly formated input, the input in quesiton was not read..." << std::endl;
      return;
    }
    //the for loop uses the iterator that was just created.
    //unsigned int i stores the current index to be placed into the array.
    unsigned int i = 0;  
    //string data store value parsed from input.
    for (std::string data = ""; itr != input.end(); ++itr) {  
      //empty space means end of one element.
      //']' means it's the last element in the input.
      if ( *itr == ' ' or *itr == ']') {  
        //this to prevent empty data to be inserted into array, 
        if (data != "")  { 
          array_[i] = setCorrectDataType(data);
          ++i;  
        }
        data = "";
        continue;  //this will take care of the first " " in the input format and any other " " from being added to data variable.
      }
      data += *itr;
    } 
  }

  size_t size() const { 
    return size_;
  }

  // @location: an index to a location in the chain.
  // @returns the Object at @location.
  // const version.
  // abort() if out-of-range.
  const Object& operator[](size_t location) const { 
    if ((location + 1) > size_) {
      std::abort(); 
    }
 
    return array_[location];
  }

 // @location: an index to a location in the range.
 // @returns the Object at @location.
 // non-const version.
 // abort() if out-of-range.
  Object& operator[](size_t location) { 
    if ((location + 1) > size_) {
      std::abort(); 
    }

    return array_[location];
  }

 //  @c1: A chain.
 //  @c2: A second chain.
 //  @return the concatenation of the two chains.
  friend Chain operator+(const Chain &c1, const Chain &c2) {
    Chain<Object> toBeReturn(c1,c2);
    return toBeReturn;
  }

  //returns a Chain with the argumented element to the end of the returned chain.
  friend Chain operator+(const Chain& c1, const Object a) {
    Chain<Object> temp;
    temp.size_ = c1.size_ + 1;
    temp.array_ = new Object[temp.size_];
    
    for (size_t i = 0; i < c1.size_; ++i) {
      temp.array_[i] = c1.array_[i];
    }
    temp.array_[temp.size_ - 1] = a;
    return temp;
  }

 // Overloading the << operator.
   friend std::ostream &operator<<(std::ostream &out, const Chain &a_chain) {
     out << '[';
     for (size_t i = 0; i < a_chain.size_; ++i) {
       out << a_chain[i];
       if ((i + 1) != a_chain.size_)  
         out << ' ';
     }
     out << ']' << std::endl;

     return out;
  }

 private:
  size_t size_;
  Object *array_;
};

}  // namespace teaching_project
#endif // CSCI_335_HOMEWORK1_CHAIN_
