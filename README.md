# c++并发编程

c++ 11新特性之一，这使得c++多线程的编写无需依赖平台专属的扩展，因此我们也可以写出可移植，行为确定的多线程代码。

## 1.你好，c++并发世界

### 1.1什么是并发

​	并发就是两个或者多个同时独立进行的活动。比如边走路边说话。

#### 1.1.1计算机系统里的并发

​	我们把表面并发理解为计算机原理的“并发”，也就是说这里的表面并发是计算机只有一个处理器靠着快速的切换任务来达成表面的并行。（这里任务切换其实会有很多编程上的问题，我们到后面再聊）。

​	现在多核处理器已经日渐普及，所以我们把这种能够真正实现运行多个任务，我们称之为“硬件并发”或者说叫“真并发”。

​	**这里我想到一个问题就是，那和我们学的线程有什么关系**，这里有回答：尽管我们的cpu多核可以真正支持并行，但是实际上任务数会远大于核数，所以我们还是用得到任务切换的。

#### 1.1.2并发的方式	

​	第一种：多进程（进程中单线程，第二种：单进程（进程里单线程。也可以随意组合这两种方式。比如我们可以多进程（进程里也包含多个线程，也可以其中有些进程只包含单个进程

**1.多进程并发**

​	把一个软件拆成多个进程同时运行，他们都只含单个线程，因为进程间通信比较慢（但是进程间通信可以提供额外的保护和高级通信机制，这意味着，比起线程，采用进程更容易编写出安全的并发代码。）某些编程环境也是用进程作为基础构建单元的。比如Erlang。通过独立进程实现并发还有一个额外的优势，就是能够通过网络连接，独立进程能够在不同的计算机上运行。

**2.多线程并发**

​	线程间的通信就比进程间的通信要简单的多,所以我们后文主要说的还是多线程并发，后文提及的并发，便假定采用多线程实现

#### 1.1.3并发与并行

​	并行更强调性能上的，我们谈及并行的时候，主要关心的是利用可调配的硬件资源处理的性能;当谈及并发的时候，我们更多关注的是分离关注点和响应能力。

### 1.2为什么使用并发技术

#### 1.2.1为分离关注点而并发

​	归类相关代码，分离无关代码（分离关注点），使程序更加容易理解和调试，这在编写软件的时候是一种很不错的办法。而并发技术可以用于隔离不同领域的技术，即使这些领域的操作需要同时进行，若不直接使用并发技术，我们不得不编写任务切换，或者不得不在某个步骤里频繁调用无关代码。

​	并发技术可以大幅度简化各线程的内部逻辑，原因是==线程间交互得以限定于代码中可明确辨识的切入点，而无需将不同任务的逻辑交错散置==。

​		这样我们线程实际数量便与cpu所有的内核数量无关，因为线程分离关注的依据是设计理念，而不是以增加吞吐量为目的的。

#### 1.2.2为性能而并发：任务并行和数据并行

​	增强性能的方式两种。第一种就是任务并行：即将单一任务分解为多个部分，各自并行运作，从而节省总运行耗时。这种并行还可以用于数据部分，我们可以对数据的不同部分进行相同的操作，我们把这成为数据并行。

​	易于采用上面并行的算法，我们把它叫做==-尴尬算法==。其含义就是把上面算法并行属实简单，简单得让人有些尴尬。尴尬并行有个天然的优势就是按==规模收缩==，他的性能随着机器线程数的提升而提升。

​	第二种就是利用并行资源解决规模更大的问题。例如，只要条件合适，便同时处理2个文件，甚至很多文件。其实就是数据并行。例如我们若能并行处理影视图像的各个部分，就会提升视频处理的解析度。

#### 1.2.3什么时候避免并发

​	并不是什么时候我们采用并发技术都是好的，所以我们要知道什么时候该采用并发技术，什么时候不该采用。

​	多数情况并发技术的代码更难理解，编写和维护代码更费心神，并且复杂度可能导致更多错误。编写正确的多线程代码需要额外的开发时间和维护成本，除非潜在的性能提升和分离关注点值得值得这些开销，否则不要使用。

​	此外，线程启动也有固定开销，若是线程执行任务时间还要短于线程启动的时间，那就有点舍本逐末了。所以说这时候还不如不采用此技术。

​	再者线程也是一种有限资源。若一次运行的线程太多，便会消耗系统资源，可能令整个系统变慢。由于每个线程都需要独立的栈空间，如果线程太多，就可能耗尽进程所属的地址空间。虽然线程池可以有效控制线程数量，但也非万能妙法。就比如服务器和客户端的链接，若是太多连接就会耗尽系统资源。针对这一情形，需谨慎使用线程池。

​	最后运行的线程越多，系统做上下文切换就越频繁，当线程数达到一定程度时，再增加新线程只会降低应用软件的整体性能。

### 1.3并发与c++多线程

#### 1.3.1c++多线程简史

​	一开始c++依赖的是平台扩展的线程库，例如符合Posix规范的c语言多线程接口（pthread)和微软windows系统（MFC）的多线程API。这会带来诸多弊端，这也不是我所要过多了解的地方，所以不多阐述。

#### 1.3.2新标准对并发的支持

​	C++11发布了新的标准，规定了内存的模型，支持并发等特性。

#### 1.3.3C++14和C++17进一步支持并发和并行

​	加了什么自己去看手册

#### 1.3.4标准C++线程库的效率

​	大概的意思就是C++新标准，提供了许多效率很高的类来满足用户对性能的需求，但是这些代码很多并不设计直接操控底层，它们更加高级，有时候高级抽象的并不代表一定比直接调用底层效率更低，有许多原因。这里有一组概念叫抽象损失，意思就是我们用更高级的类相比直接调用底层操作之间的效率差。假如这两者之间的差值很小，我们把它称为低抽象损失。

#### 1.3.5平台专属工具

​	C++也提供了底层直接运用平台API的类，但这些超出了这本书要讲的范畴。

### 1.4启程
管控线程的函数和类在 \<thread> 头文件中,有关共享数据保护的声明则在别的头文件当中。

​	主线程是从main起始函数开始的，别的线程的起始函数需要从std::thread对象的构造函数中指明。本例中是以hello（）作为起始函数。

e g:

```c++
#include <thread>
#include <iostream>

void hello(){
    std::cout << "Hello Concurrent World\n";
}

int main(void){
    std::thread t(hello);
    t.join();
    return 0;
}
```

​	新线程启动后，主线程继续执行，如果==起始线程==不等待新线程结束，就会一路执行下去，直到main()函数结束，很可能结束整个程序，甚至新线程都来不及启动。所以我们得调用一个join函数去等待子线程。主线程负责main函数的执行，子线程则和std::thread对象 t 所关联。

## 2.线程管控

​	对于线程，我们只是用C++标准库就可以管控该线程的几乎每个细节。对于复杂的任务，C++标准库还提供了基础构建单元来满足需求，我们可以灵活选择。

​	这章主要是讲的发起线程，等他结束，或者让他在后台运行。然后我们会学习如何在启动时向线程传递参数，以及如何把某个线程的归属权交接到另一个thread对象上。最后我们学习如何选择合适的线程数量，以及怎么识别特定的线程。

### 2.1线程的基本管控

​	我们的C++程序至少有一个线程，它是以main函数为入口函数，由系统自动启动的线程。而其他线程由其他函数为入口启动。当main函数返回时，程序就会退出，其他也是如此，当其他函数结束时，对应的线程也会结束。

#### 2.1.1发起线程

​	我们想要发起一个线程，我们总会构造一个thread对象，它的参数只要是可调用对象即可（比如lamb表达式或者是可调用对象（重载了operator的类）。

​	当我们调用thread构造函数的时候，把参数传递给对象，对象会把此参数的副本完全复制到新线程当中去，并在那里被调用

​	**值得注意的是**：当我们传递给thread参数的时候要避免造成“c++最麻烦的解释”（即可能存在二义性的问题，整个调用语句可能解释为函数声明，==对于编译器来说，只要可能吧语句解释为函数声明，编译器总会把语句解释成函数声明==）假设我们传入的是一个临时变量请看如下代码：

```c++
std::thread my_thread(background_task())//background_task为函数对象
```

这句代码很有可能解释为这样，返回值为thread对象的名为my_thread的函数，这个函数的形参是返回值为backgound_task类，形参为空的函数指针！！！

​	所以这非常值得小心。

​	有三种可以避免这种情况的办法：为函数对象命名或者用lam表达式

1. 我们可以在参数外再加一对括号

   ```c++
   std::thread my_thread((background_task()))
   ```

2. 使用初始化列表的方式（新统一初始化方式

   ```c++
   std::thread my_thread{background_task()}
   ```

3. 用lamb表达式

   ```c++
   std::thread my_thread([]{
   	dosomething();
   	do_something_else();
   })
   ```

​	一旦启动了线程，我们就要明确是要等待它结束还是任由它运行。所以我们现在要做的就是确定我们新创建的线程应该汇合还是分离，即使有异常抛出也照样如此。后面也会有处理异常的办法。

​	**这里有个问题**：假定我们让新线程分离，那我们必须保证数据的有效，比新线程的启动函数的参数是指针或者引用，如果这个指针或者引用指向的值已经销毁了，那我们就出现了一个悬空指针的问题。我们在使用多线程的时候会经常面临这种==生存期的问题==。

比如：

```c++
#include <iostream>
#include <thread>
//测试会报错
class Sum {
public:
	Sum(int& _a):a(_a) {}
	void operator()() {
		for (int i = 0; i < 100000000; i++) {
			sum += a;
			//std::cout << a << std::endl;
		}
	}
private:
	int a;
	int sum = 0;
};

int main() {
	if (1) {
		int a = 1;//引用局部变量
		Sum sum(a);
		std::thread t(sum);
		t.detach();//不等待
	}
	while(1){
		;
	}
	return 0;
}
```

这里使用的detach()是分离，明确主线程不等待。这种错误也不是立刻发生，也不是一定发生，所以非常值得注意，因为c++是允许指针或者引用指向局部变量的。

​	==上述情况的处理方法通常是：令线程函数完全自含，将数据复制到新线程内部，而不是共享数据==。

#### 2.1.2等待线程完成

​	join（）函数简单而粗暴，使用它我们会一直等待或者干脆不等待，所以要更精细控制线程等待，我们得改用其他方式，如futrue和条件变量。

​	另外调用join（）意味着隶属于该线程的任何存储空间即会因此消除。std::thread对象不再关联到已经结束的线程。它与任何线程均无关系。意思是每个对象只能调用一次join()；只要thread对象调用一次join（）后线程就不再汇合，成员函数joinable()将返回false。

#### 2.1.3在出现异常的情况下等待

​	在std::thread对象销毁前，我们需要确保调用join()和detach()。假如线程启动之后，有异常抛出，如果这时候还没有调用join(),则该join()会被忽略。

​	防止程序因线程抛出异常而终止，我们需要处理这种情况。一般来说我们调用join()是为了对付没有异常的情况，而对于有异常抛出的线程我们仍要调用join()来防止生存期的问题发生，如下面代码:

```c++
//伪代码 pseudocode
struct func;
void f(){
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try{//假如发生异常抛出，先找到catch
        do_something_in_current_thread();
    }
    catch(...){//发生异常抛出先调用join()再抛出异常
        t.join();
        throw;
    }
    t.join();
}
```

这份代码主要就是为了保证局部变量不在被销毁后被线程使用，
