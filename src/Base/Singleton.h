#ifndef BASE_SINGLETON_
#define BASE_SINGLETON_

// Templated class for singletons.
// This class Singleton<Typename> provides singleton utility for any type. It
// manages an instance of Type.
//
// The typical behavior is that ClassA is defined with private or protected
// constructors so that they can't be instantiated normally. Instead,
// Singleton<ClassA> is declared by ClassA as a friend class and contains the
// singleton object of ClassA. Note that classA must at least provide default
// constructor.
//
// The Singleton class also provides method CreateInstance() which can be
// specialized so that user can define more specific and complex initializations
// for their class. Of course, it also requires Singleton<ClassA> being friend
// class of ClassA.
//
//   class ClassA {
//    public:
//     Foo() {}
//     Bar(int) {}
//    private:
//     ClassA() {}
//     // Disallow copy and assign.
//     ClassA(const ClassA&) = delete;
//     ClassA& operator=(const ClassA&) = delete;
//     friend class Singleton<ClassA>;  // DO NOT FORGET THIS.
//   };
//
//   ClassA* ptr = Singleton<ClassA>::get();
//   ptr->func();
//
template<typename T>
class Singleton {
 public:
  static T* get() {
    if (!initialized_) {
      Init();
      initialized_ = true;
    }
    return instance_;
  }

 private:
  static T* CreateInstance() {
    return new T();
  }

  static void Init() {
    instance_ = CreateInstance();
  }

  static bool initialized_;
  static T* instance_;
};

template <typename T>
bool Singleton<T>::initialized_ = false;

template <typename T>
T* Singleton<T>::instance_ = nullptr;


#endif  /* BASE_SINGLETON_ */
