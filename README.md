# **Custom vector**
## *Educational project*

Vector is an analogue of the standard vector container, with a similar structure and functionality. It is based on an array in dynamic memory. The container itself stores a pointer to the memory area in which the elements of the dynamic array are placed in a continuous block, the capacity of the vector and its size.


**The vector class implements the following functionality:**
- An auxiliary template class RawMemory, responsible for storing a buffer that can hold a specified number of elements and providing access to elements by index.
- Designers.
   - Default constructor. Initializes a vector of zero size and capacity. Does not throw exceptions. Algorithmic complexity: O(1).
   - A constructor that creates a vector of a given size. The capacity of the created vector is equal to its size, and the elements are initialized with the default value for type T. Exception tolerant. Algorithmic complexity: O(vector size).
   - Copy constructor. Creates a copy of the elements of the original vector. It has a capacity equal to the size of the original vector, that is, it allocates memory without reserve. Exception tolerant. Algorithmic complexity: O(size of original vector).
   - Moving constructor. Runs in O(1) and does not throw exceptions.
- Destructor. Destroys the elements contained in the vector and frees the memory they occupy. Algorithmic complexity: O(vector size).
- Helper method Size to get the number of elements in the vector. Does not throw exceptions.
- Capacity auxiliary method for obtaining the capacity of a vector. Does not throw exceptions.
- Helper static methods Allocate and Deallocate to allocate and deallocate raw memory.
- Operator [] to access a vector element by its index. It has two versions - constant and non-constant. Does not throw exceptions. For the operator to work correctly, the index of an array element must not go beyond the array.
- The Reserve method sets the vector capacity. This method improves code efficiency when the user knows in advance at least the approximate number of elements in the vector. Reserve immediately allocates the required amount of memory. When adding new elements to a vector, copying will occur either much less frequently or not at all.
- Copy assignment operator. Performs in O(N), where N is the maximum of the sizes of the vectors involved in the operation.
- Moving assignment operator. Runs in O(1) and does not throw exceptions.
- Swap method, which exchanges the contents of a vector with another vector. The operation has O(1) complexity and does not throw exceptions.
- The Resize method changes the number of elements in the vector.
- The PushBack method adds a new value to the end of the vector. When there is a shortage of memory, Vector increases the capacity by a multiple of times.
- The PopBack method destroys the last element of the vector and reduces the size of the vector by one.
- EmplaceBack method. This is a more efficient method to insert elements into a vector. Will provide a strong exception safety guarantee when any of the following conditions are true:
   - the move constructor of type T is declared as noexcept;
   - type T has a public copy constructor.
If type T does not have a copy constructor and the move constructor can throw exceptions, the EmplaceBack method provides a basic guarantee of exception safety. Complexity is an amortized constant.
- The Insert method inserts an element at a given vector position. Like EmplaceBack and PushBack, the Insert method allows a vector element to be inserted inside the same vector.
- The Emplace method operates according to the same algorithm as the Insert method. The main difference from Insert is that Emplace uses perfect forwarding to pass its parameters to the element constructor.
- The Erase method removes the element pointed to by the passed iterator.

## Assembly and installation
Build using any IDE or build from the command line

## System requirements
C++ compiler supporting C++17 standard or later
