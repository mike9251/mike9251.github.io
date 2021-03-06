Smart pointers

shared_ptr is a smart pointer implementation, which allows sharing a raw pointer among other shared_ptr's instances.

std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>();
or
std::shared_ptr<MyClass> ptr1;
ptr1 = std::make_shared<MyClass>();
or 
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
or std::shared_ptr<MyClass> ptr1 = std::make_shared(*obj);

But don't do that:
MyClass obj;
std::shared_ptr<MyClass> ptr1(&obj);
Because in this case the object obj is allocated in stack and when ptr1 pointer will be deleting this object the program will crush. The object should be allocated in heap.
The smart pointer expects that the object which pointer it holds is allocated in the heap.

If you want to share raw pointer, the safe method is:
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
std::shared_ptr<MyClass> ptr2(ptr1);
In this case two smart pointers will be holding address of the object (ref count = 2). If one of the shared pointers exits it's local space it will be destroyied,
reference counter will be decresed by 1, but the object obj will exist and the other shared pointer will be holding it's address.

Not safe method is to let several shared pointers hold object's adress through the raw pointer obj: 
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
std::shared_ptr<MyClass> ptr2(obj);

In this case reference counters will be unique for each shared pointers and if one of the shared pointers leaves it's locas space it will trigger dectructor for obj object.
But the other pointer still thinks that it's holding adress of some object and any attempt to do something with it will cause a program crush.



weak_ptr is a smart pointer implementation, which allows sharing a raw pointer among other shared_ptr's instances and it doesn't increment reference counter.
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
std::weak_ptr<MyClass> wptr1(ptr1); // number of references doesn't change

To get acces to the object weak_ptr should be transformed into shared_ptr:
std::shared_ptr<MyClass> ptr2 = wptr1.lock() or auto ptr2 = wptr1.lock()



unique_ptr is a smart pointer implementation, which solely holds a raw pointer of an object!
std::unique_ptr<int> uptr1(new int(13));
std::cout << "uptr1 = " << *uptr1 << std::endl;
We can get a shared pointer from a unique one:
std::shared_ptr<int> shared = std::move(uptr1);
After that uptr1 will be empty and posession of the objects adres went to shared.

We can't get a weak_ptr from unique_ptr.


One possible use case:
int main(){
	MyClass* obj = new MyChild();
    std::shared_ptr<MyClass> p(obj1);
    return 0;
}

Both MyChild and MyClass object will be deleted (even without virtual destructors).
