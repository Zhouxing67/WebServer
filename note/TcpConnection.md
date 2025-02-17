# TcpConnection生命周期问题 
tag为v2.1的提交解决了TcpConnection对象的生命周期问题，本文档对此做了记录

### 生命周期问题从何而来：
在之前的一次主要工作，我们是通过unique_ptr来管理TcpServer中TcpConnection对象；然而：使用unique_ptr很容易导致内存泄漏；即：由于TcpConnection是被动关闭，当我们channel在handleEvent时，发现客户端传入了关闭连接的信息后（TcpConnecton::Read()），直接对onClose进行了调用。因此如果使用unqiue_ptr时，在调用onclose后，TcpConnection便被销毁，而其拥有的channel成员也会被销毁。但是此时其channel成员正在调用的handle_event并没有结束，因此发生了内存泄漏。

### 解决办法：使用shared_ptr延长TcpConnection对象的生命周期
具体实现：
1. Channel类增加成员tie_ : weak_ptr<void> (void类型实现类型擦除)。通过在Channel::handle_event中调用tie_.lock（） 可以锁定持有本Channel的shanre_ptr对象，防止其提前析构。
2. TcpConnecton继承enable_shared_from_this<TcpConnection>，使得其可以在成员函数中构造自身的shared_ptr(注意：不可以通过make_shared<TcpConnection>（this）来返回自身的shared_ptr,这会使得引用计数发生错乱)
3. 将关闭TcpConnection的操作移交给main_reactor_进行（原来是在sub_reactor中进行）；这样有两个好处：I. TcpConnection对象的构造是由main_reactor_完成的，再将关闭操作交给main_reactor_，这样更加自然（有始有终）ii. 避免了主线程和子线程的条件竞争问题（TcpConnection的close是在sub_reactor中的loop()中进行，而sub_reactor的loop()被分配到了线程池里的某个子线程里；在TcpConnection的建立（new_conn）与 关闭（close）的过程中，涉及到对unordered_map的插入与删除，因此存在条件竞争）